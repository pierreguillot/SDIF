/* $Id: SdifFWrite.h,v 3.3 1999-09-28 13:08:56 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
LIBRARY
 * SdifFWrite.h
 *
 * F : SdifFileT* SdifF, Write : sdif file write (SdifF->Stream)
 *
 *
 * author: Dominique Virolle 1997
 *
EXAMPLE

L'exemple suivant essaye de montrer l'ordonnancement des appels
de fonction. Biensûr ce code devrait être plus modulaire. En effet,
il devrait y avoir une fonction par niveau structurel d'écriture:
une(plus) fonction(s) d'écriture de matrice, une(plus fonction(s)
d'écriture de frame...

<pre>
#include "sdif.h"

void main(void)
{
  SdifFileT   *SdifF;
  SdifUInt4    NbMatrix = 3;
  SdifUInt4    NumID = 0;
  SdifFloat8   Time = 0.0;
  SdifFloat4   TabValue[] = {1,2,3,4,5,6,7};
  SdifFloat4  *pTabValue;
  size_t       SizeFrameW;
  size_t       SizeMatrixW;

  pTabValue = TabValue; // pour permettre le cast par pointeur 

  SdifGenInit("SdifTypes.STYP");

  SdifF = SdifOpenFile("NewFile.sdif",      eWriteFile);


  // remplir les tables NameValues, MatrixTypesTable,
  // FrameTypesTable et StreamIDsTable.
   
  [ ..... ]

  // écriture de l'entête 
  SdifFWriteGeneralHeader (SdifF);
  // écriture des chunks ASCII 
  SdifFWriteAllASCIIChunks (SdifF)


  // ***FRAME HEADER****
  // Mise à jour le l'entête de frame à écrire 
  SdifSetCurrFrameHeader (SdifF, '1FOB', _SdifUnknownSize, NbMatrix, NumID, Time);
  // écriture de l'entête de frame 
  SizeFrameW = SdifFWriteFrameHeader (SdifF);


  // ***FIRST MATRIX**
  // Mise à jour le l'entête de matrice à écrire : 1 ligne, 1 colonne
  SdifSetCurrMatrixHeader (SdifF, '1FQ0', eFloat4, 1, 1);
  // écriture de l'entête de frame 
  SizeMatrixW = SdifFWriteMatrixHeader (SdifF);

  // Mise à jour de la ligne-buffer de SdifF
  // La largeur des données est conservée par le eFloat4 de l'entête de matrice
  SdifSetCurrOneRow (SdifF, (void*) pTabValue);
  // écriture de la ligne 
  SizeMatrixW += SdifFWriteOneRow (SdifF);
  // Si on a d'autres lignes à écrire alors
  // on répette SdifSetCurrOneRow et SizeMatrixW += SdifFWriteOneRow...

  // écriture du Padding en fin de matrice et ajout de la taille de la matrice écrite
  // à la taile du frame.
   
  SizeMatrixW += SdifFWritePadding(SdifF,
                       SdifFPaddingCalculate(SdifF->Stream, SizeMatrixW))  
  SizeFrameW += SizeMatrixW;


  // * MATRIX 2 & 3
  [. 2 matrices à écrire
   .
   .]

   
  // pas de padding en fin de frame car on est déjà aligné 

  // la taille écrite ne doit pas compter la signature et la taille===> -8 
  SizeFrameW -= 8;
  SdifFUpdateChunkSize(SdifF, SizeFrameW);

  SdifCloseFile(SdifF);

  SdifGenKill();
}
</pre>

LOG
 * $Log: not supported by cvs2svn $
 * Revision 3.2  1999/08/25  18:32:35  schwarz
 * Added cocoon-able comments with sentinel "DOC:" (on a single line).
 *
 * Revision 3.1  1999/03/14  10:56:48  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:47  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:31  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:14  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:39  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.2  1998/11/10  15:31:44  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifFWrite_
#define _SdifFWrite_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"

#include <stdio.h>
#include "SdifNameValue.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifMatrix.h"
#include "SdifFrame.h"





/*DOC: 
  Execute un retour fichier de ChunkSize bytes et l'écrit, donc on
  écrase la taille du chunk ou du frame.  Dans le cas où le fichier
  est stderr ou stdout, l'action n'est pas réalisée.  */
void    SdifUpdateChunkSize       (SdifFileT *SdifF, size_t ChunkSize);

/*DOC: 
  Cette fonction permet en fin d'écriture de matrice d'ajouter le
  Padding nécessaire. Il faut cependant avoir la taille de ce
  Padding. On utilise SdifFPaddingCalculate(SdifF->Stream,
  SizeSinceAlignement) où SizeSinceAllignement est un
  <code>size_t</code> désignant le nombre de bytes qui sépare la
  position actuelle d'écriture avec une position connue où le fichier
  est aligné sur 64 bits (en général, c'est la taille de la matrice en
  cours d'écriture: NbRow*NbCol*DatWitdh).  */
size_t  SdifFWritePadding         (SdifFileT *SdifF, size_t Padding);

/*DOC: 
  écrit sur le fichier 'SDIF' puis 4 bytes à 0.  */
size_t  SdifFWriteGeneralHeader   (SdifFileT *SdifF);

size_t  SdifFWriteChunkHeader     (SdifFileT *SdifF, SdifSignature ChunkSignature, size_t ChunkSize);
size_t  SdifFWriteNameValueLCurrNVT (SdifFileT *SdifF);
size_t  SdifFWriteAllNameValueNVT   (SdifFileT *SdifF);

/*
 * obsolete
 */
size_t  SdifFWriteNameValueCurrHT (SdifFileT *SdifF);
size_t  SdifFWriteAllNameValueHT  (SdifFileT *SdifF);

size_t  SdifFWriteAllMatrixType   (SdifFileT* SdifF);
size_t  SdifFWriteAllFrameType    (SdifFileT *SdifF);
size_t  SdifFWriteAllType         (SdifFileT *SdifF);
size_t  SdifFWriteAllStreamID     (SdifFileT *SdifF);

/*DOC: 
  écrit tous les chunks ASCII. C'est à dire: les tables de names
  values, les types créés ou complétés, et les Stream ID. Il faut donc
  au préalable avoir rempli complétement les tables avant de la
  lancer. Cette fonction de peut donc pas être executer une 2nd fois
  durant une écriture.  */
size_t  SdifFWriteAllASCIIChunks  (SdifFileT *SdifF);

/*DOC: 
  Après avoir donner une valeur à chaque champ de SdifF->CurrMtrxH
  gràce à la fonction SdifFSetCurrMatrixHeader, SdifFWriteMatrixHeader
  écrit toute l'entête de la matrice.  Cette fonction réalise aussi
  une mise à jour de SdifF->CurrOneRow, tant au niveau de l'allocation
  mémoire que du type de données.  */
size_t  SdifFWriteMatrixHeader    (SdifFileT *SdifF);

/*DOC: 
  Après avoir donner les valeurs à chaque case de SdifF->CurrOneRow à
  l'aide de SdifFSetCurrOneRow ou de SdifFSetCurrOneRowCol (suivant
  que l'on possède déjà un tableau flottant ou respectivement une
  méthode pour retrouver une valeur de colonne), SdifFWriteOneRow
  écrit 1 ligne de matrice suivant les paramètres de SdifF->CurrMtrxH.  */
size_t  SdifFWriteOneRow          (SdifFileT *SdifF);

/*DOC: 
  Après avoir donner une valueur à chaque champ de SdifF->CurrFramH
  gràce à la fonction SdifFSetCurrFrameHeader, SdifFWriteFrameHeader
  écrit toute l'entête de frame.  Lorsque la taille est inconnue au
  moment de l'écriture, donner la valeur _SdifUnknownSize. Ensuite,
  compter le nombre de bytes écrit dans le frame et réaliser un
  SdifUpdateChunkSize avec la taille calculée.  */
size_t  SdifFWriteFrameHeader     (SdifFileT *SdifF);


size_t  SdifFWriteOneNameValue    (SdifFileT *SdifF, SdifNameValueT  *NameValue);
size_t  SdifFWriteOneMatrixType   (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
size_t  SdifFWriteOneComponent    (SdifFileT *SdifF, SdifComponentT  *Component);
size_t  SdifFWriteOneFrameType    (SdifFileT *SdifF, SdifFrameTypeT  *FrameType);
size_t  SdifFWriteOneStreamID     (SdifFileT *SdifF, SdifStreamIDT   *StreamID);


#endif /* _SdifFWrite_ */
