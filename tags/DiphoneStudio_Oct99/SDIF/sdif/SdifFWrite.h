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
de fonction. Biens�r ce code devrait �tre plus modulaire. En effet,
il devrait y avoir une fonction par niveau structurel d'�criture:
une(plus) fonction(s) d'�criture de matrice, une(plus fonction(s)
d'�criture de frame...

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

  // �criture de l'ent�te 
  SdifFWriteGeneralHeader (SdifF);
  // �criture des chunks ASCII 
  SdifFWriteAllASCIIChunks (SdifF)


  // ***FRAME HEADER****
  // Mise � jour le l'ent�te de frame � �crire 
  SdifSetCurrFrameHeader (SdifF, '1FOB', _SdifUnknownSize, NbMatrix, NumID, Time);
  // �criture de l'ent�te de frame 
  SizeFrameW = SdifFWriteFrameHeader (SdifF);


  // ***FIRST MATRIX**
  // Mise � jour le l'ent�te de matrice � �crire : 1 ligne, 1 colonne
  SdifSetCurrMatrixHeader (SdifF, '1FQ0', eFloat4, 1, 1);
  // �criture de l'ent�te de frame 
  SizeMatrixW = SdifFWriteMatrixHeader (SdifF);

  // Mise � jour de la ligne-buffer de SdifF
  // La largeur des donn�es est conserv�e par le eFloat4 de l'ent�te de matrice
  SdifSetCurrOneRow (SdifF, (void*) pTabValue);
  // �criture de la ligne 
  SizeMatrixW += SdifFWriteOneRow (SdifF);
  // Si on a d'autres lignes � �crire alors
  // on r�pette SdifSetCurrOneRow et SizeMatrixW += SdifFWriteOneRow...

  // �criture du Padding en fin de matrice et ajout de la taille de la matrice �crite
  // � la taile du frame.
   
  SizeMatrixW += SdifFWritePadding(SdifF,
                       SdifFPaddingCalculate(SdifF->Stream, SizeMatrixW))  
  SizeFrameW += SizeMatrixW;


  // * MATRIX 2 & 3
  [. 2 matrices � �crire
   .
   .]

   
  // pas de padding en fin de frame car on est d�j� align� 

  // la taille �crite ne doit pas compter la signature et la taille===> -8 
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
  Execute un retour fichier de ChunkSize bytes et l'�crit, donc on
  �crase la taille du chunk ou du frame.  Dans le cas o� le fichier
  est stderr ou stdout, l'action n'est pas r�alis�e.  */
void    SdifUpdateChunkSize       (SdifFileT *SdifF, size_t ChunkSize);

/*DOC: 
  Cette fonction permet en fin d'�criture de matrice d'ajouter le
  Padding n�cessaire. Il faut cependant avoir la taille de ce
  Padding. On utilise SdifFPaddingCalculate(SdifF->Stream,
  SizeSinceAlignement) o� SizeSinceAllignement est un
  <code>size_t</code> d�signant le nombre de bytes qui s�pare la
  position actuelle d'�criture avec une position connue o� le fichier
  est align� sur 64 bits (en g�n�ral, c'est la taille de la matrice en
  cours d'�criture: NbRow*NbCol*DatWitdh).  */
size_t  SdifFWritePadding         (SdifFileT *SdifF, size_t Padding);

/*DOC: 
  �crit sur le fichier 'SDIF' puis 4 bytes � 0.  */
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
  �crit tous les chunks ASCII. C'est � dire: les tables de names
  values, les types cr��s ou compl�t�s, et les Stream ID. Il faut donc
  au pr�alable avoir rempli compl�tement les tables avant de la
  lancer. Cette fonction de peut donc pas �tre executer une 2nd fois
  durant une �criture.  */
size_t  SdifFWriteAllASCIIChunks  (SdifFileT *SdifF);

/*DOC: 
  Apr�s avoir donner une valeur � chaque champ de SdifF->CurrMtrxH
  gr�ce � la fonction SdifFSetCurrMatrixHeader, SdifFWriteMatrixHeader
  �crit toute l'ent�te de la matrice.  Cette fonction r�alise aussi
  une mise � jour de SdifF->CurrOneRow, tant au niveau de l'allocation
  m�moire que du type de donn�es.  */
size_t  SdifFWriteMatrixHeader    (SdifFileT *SdifF);

/*DOC: 
  Apr�s avoir donner les valeurs � chaque case de SdifF->CurrOneRow �
  l'aide de SdifFSetCurrOneRow ou de SdifFSetCurrOneRowCol (suivant
  que l'on poss�de d�j� un tableau flottant ou respectivement une
  m�thode pour retrouver une valeur de colonne), SdifFWriteOneRow
  �crit 1 ligne de matrice suivant les param�tres de SdifF->CurrMtrxH.  */
size_t  SdifFWriteOneRow          (SdifFileT *SdifF);

/*DOC: 
  Apr�s avoir donner une valueur � chaque champ de SdifF->CurrFramH
  gr�ce � la fonction SdifFSetCurrFrameHeader, SdifFWriteFrameHeader
  �crit toute l'ent�te de frame.  Lorsque la taille est inconnue au
  moment de l'�criture, donner la valeur _SdifUnknownSize. Ensuite,
  compter le nombre de bytes �crit dans le frame et r�aliser un
  SdifUpdateChunkSize avec la taille calcul�e.  */
size_t  SdifFWriteFrameHeader     (SdifFileT *SdifF);


size_t  SdifFWriteOneNameValue    (SdifFileT *SdifF, SdifNameValueT  *NameValue);
size_t  SdifFWriteOneMatrixType   (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
size_t  SdifFWriteOneComponent    (SdifFileT *SdifF, SdifComponentT  *Component);
size_t  SdifFWriteOneFrameType    (SdifFileT *SdifF, SdifFrameTypeT  *FrameType);
size_t  SdifFWriteOneStreamID     (SdifFileT *SdifF, SdifStreamIDT   *StreamID);


#endif /* _SdifFWrite_ */
