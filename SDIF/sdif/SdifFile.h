/* $Id: SdifFile.h,v 3.4 1999-09-28 10:37:00 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr

LIBRARY
 * SdifFile.h
 *
 * Sdif Files management
 * High Level.
 *
 * author: Dominique Virolle 1997

LOG
 * $Log: not supported by cvs2svn $
 * Revision 3.3  1999/09/20  13:21:58  schwarz
 * Introduced user data and access functions SdifFAddUserData/GetUserData.
 *
 * Revision 3.2  1999/08/25  18:32:35  schwarz
 * Added cocoon-able comments with sentinel "DOC:" (on a single line).
 *
 * Revision 3.1  1999/03/14  10:56:51  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/02/28  12:16:42  virolle
 * memory report
 *
 * Revision 2.2  1999/01/23  13:57:33  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:16  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:41  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.6  1998/11/10  15:31:45  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 * Revision 1.5  1998/07/23  17:02:49  virolle
 * *** empty log message ***
 *
 * Revision 1.4  1998/05/14  09:50:35  schwarz
 * Added SdifCurrOneRowData to return a pointer to the raw data.
 * This can subsequently be used for SdifSetCurrOneRow.
 */

#ifndef _SdifFile_
#define _SdifFile_

#include "SdifFileStruct.h"
#include "SdifGlobals.h"


/*DOC: 
  Test if file is an SDIF file.

  [] Returns:	0 if not an SDIF file (the first 4 chars are not "SDIF"),
		or file can not be opened, else 1.  

  Warning: This function doesn't work with stdio.
*/
int SdifCheckFileFormat (char *name);

/*DOC: 
  todo: Test if file is an SDIF file (only when opening for read or
  append) and open it.

  [Return] NULL if not an SDIF file (the first 4 chars are not "SDIF"),
  or file can not be opened.  */
SdifFileT*	   SdifFTryOpen			(const char *Name, SdifFileModeET Mode);

/*DOC:
 */
SdifFileT*         SdifFOpen                    (const char *Name, SdifFileModeET Mode);
SdifFileT*         SdifOpenFile                 (const char *Name, SdifFileModeET Mode);
SdifFileT*         SdifFOpenText                (SdifFileT *SdifF, const char* Name, SdifFileModeET Mode);
/*DOC:
 */
void               SdifFClose                   (SdifFileT *SdifF);
void               SdifCloseFile                (SdifFileT *SdifF);
SdifFrameHeaderT*  SdifFCreateCurrFramH         (SdifFileT *SdifF, SdifSignature Signature);
SdifMatrixHeaderT* SdifFCreateCurrMtrxH         (SdifFileT *SdifF);
FILE*              SdifFGetFILE_SwitchVerbose   (SdifFileT *SdifF, int Verbose);
void               SdifTakeCodedPredefinedTypes (SdifFileT *SdifF);
void               SdifFLoadPredefinedTypes     (SdifFileT *SdifF, char *TypesFileName);

extern int	  gSdifInitialised;
extern SdifFileT *gSdifPredefinedTypes;

/*DOC: 
  Initialise the SDIF library, providing a name for an optional additional
  file with type definitions or "". */
void SdifGenInit (char *PredefinedTypesFile); 

/*DOC:
  Deinitialise the SDIF library */
void SdifGenKill (void); 

/*DOC:
  Print version information to standard error. */
void SdifPrintVersion(void);


/*DOC: 
  Permet de donner des valeurs à chaque champ de l'entête de frame
  temporaire de SdifF.<p> 

  Exemple:
  <code>SdifSetCurrFrameHeader(SdifF, '1FOB', 3, NumID, 1.0);</code> */
SdifFrameHeaderT* SdifFSetCurrFrameHeader (SdifFileT *SdifF, 
					   SdifSignature Signature, 
					   SdifUInt4 Size,
					   SdifUInt4 NbMatrix, 
					   SdifUInt4 NumID, 
					   SdifFloat8 Time);

/*DOC: 
  Permet de donner des valeurs à chaque champ de l'entête de matice
  temporaire de SdifF.<p>

  Exemple:
  <code>SdifSetCurrMatrixHeader(SdifF, '1FOF', eFloat4, NbFofs, 7);</code> */
SdifMatrixHeaderT* SdifFSetCurrMatrixHeader (SdifFileT *SdifF, 
					     SdifSignature Signature,
					     SdifDataTypeET DataType, 
					     SdifUInt4 NbRow, SdifUInt4 NbCol);


/*DOC: 
  Recopie la mémoire pointée par Values en fonction de l'entête de
  matrice courante.<p> 

  Exemple:<br>
<pre>
  #define NbCols = 10;<br>

  float t[NbCols] = { 1., 2., 3., 4., 5., 6., 7., 8., 9., 0.};<br>

  SdifFSetCurrMatrixHeader(SdifF, 'mtrx', eFloat4, 1, NbCols);<br>
  SdifFSetCurrOneRow      (SdifF, (void*) t);<br>
</pre>

  On connait la taille de la mémoire à recopier par le type de donnée
  (ici: eFloat4) et le nombre de colonnes (ici: NbCols). Il faut que
  le type de donnée de la matrice courante corresponde avec la taille
  d'un élément de t. Si t est composé de float sur 4 bytes, alors on
  doit avoir eFloat4. Si t est composé de double float sur 8 bytes,
  alors c'est eFloat8.<br>

  En général, les données d'un programme ne se présente pas sous cette
  forme et il faut réaliser une transposition lors des transfert de
  Sdif à un programme. Le programme Diphone Ircam a un bon exemple de
  lecture avec transposition automatique, généralisée pour tout type
  de matrice. */
SdifOneRowT*  SdifFSetCurrOneRow       (SdifFileT *SdifF, void *Values);


/*DOC: 
  Permet de donner la valeur Value dans la ligbe de matrice temporaire
  de SdifF à la colonne numCol (0<numCol<=SdifF->CurrMtrxH->NbCol).  */
SdifOneRowT* SdifFSetCurrOneRowCol (SdifFileT *SdifF, SdifUInt4
numCol, SdifFloat8 Value);


/*DOC: 
  Recupère la valeur stockée à la colonne numCol de la ligne
  temporaire.  C'est un SdifFloat8 donc un double!!  */ 
SdifFloat8 SdifFCurrOneRowCol (SdifFileT *SdifF, SdifUInt4 numCol);


/*DOC: 
  Idem que la fonction précédente mais en utilisant le type de la
  matrice et le nom de la colonne.  */
SdifFloat8    SdifFCurrOneRowColName   (SdifFileT *SdifF, 
					SdifMatrixTypeT *MatrixType, 
					char *NameCD);


/*DOC: 
  Renvoie la signature temporaire de Chunk ou de Frame.  */
SdifSignature SdifFCurrSignature       (SdifFileT *SdifF);


/*DOC: 
  Met à 0 tous les bits de la signature temporaire.  */
SdifSignature SdifFCleanCurrSignature  (SdifFileT *SdifF);

/*DOC: 
  Renvoie la signature temporaire du dernier Frame lu ou du prochain à
  écrire.  */
SdifSignature SdifFCurrFrameSignature  (SdifFileT *SdifF);

/*DOC: 
  Renvoie la signature temporaire de la dernier matrice lue ou de la
  prochaine à écrire.  */
SdifSignature SdifFCurrMatrixSignature (SdifFileT *SdifF);

/*DOC: 
  Renvoie la ligne temporaire de SdifF.  */
SdifOneRowT*  SdifFCurrOneRow          (SdifFileT *SdifF);

/*DOC:
  Returns a pointer to the data of the current matrix row.  According to the matrix data type, it can be a pointer to float or double. */
void*	     SdifFCurrOneRowData	  (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrMtrx->NbCol, nombre de colonnes de la matrice en
  cours de traitement.  */
SdifUInt4     SdifFCurrNbCol           (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrMtrx->NbRow, nombre de lignes de la matrice en
  cours de traitement.  */
SdifUInt4     SdifFCurrNbRow           (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrFramH->NbMatrix, mombre de matrices du frame
  courant.  */
SdifUInt4     SdifFCurrNbMatrix        (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrFramH->NumID, index de l'objet du frame courant.  */
SdifUInt4     SdifFCurrID              (SdifFileT *SdifF);

/*DOC: 
  Renvoie SdifF->CurrFramH->Time.  */
SdifFloat8    SdifFCurrTime            (SdifFileT *SdifF);

/*DOC:
  Add user data, return index added 
*/
int SdifFAddUserData (SdifFileT *file, void *data);

/*DOC:
  Get user data by index 
*/
void *SdifFGetUserData (SdifFileT *file, int index);


SdifSignatureTabT* SdifCreateSignatureTab (SdifUInt4 NbSignMax);
void		   SdifKillSignatureTab   (SdifSignatureTabT *SignTab);
SdifSignatureTabT* SdifReInitSignatureTab (SdifSignatureTabT *SignTab, SdifUInt4 NewNbSignMax);
SdifSignatureTabT* SdifPutInSignatureTab  (SdifSignatureTabT *SignTab, SdifSignature Sign);
SdifSignature      SdifIsInSignatureTab   (SdifSignatureTabT *SignTab, SdifSignature Sign);
SdifFileT*    SdifFReInitMtrxUsed (SdifFileT *SdifF);
SdifFileT*    SdifFPutInMtrxUsed  (SdifFileT *SdifF, SdifSignature Sign);
SdifSignature SdifFIsInMtrxUsed   (SdifFileT *SdifF, SdifSignature Sign);


SdifErrorT*     SdifFLastError    (SdifFileT *SdifF);
SdifErrorTagET  SdifFLastErrorTag (SdifFileT *SdifF);

#endif /* _SdifFile_ */



