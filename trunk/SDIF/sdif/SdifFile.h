/* $Id: SdifFile.h,v 2.0 1998-11-29 11:41:41 virolle Exp $
 *
 * SdifFile.h
 *
 * Sdif Files management
 * High Level.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
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
 *
 */



#ifndef _SdifFile_
#define _SdifFile_

#include "SdifFileStruct.h"
#include "SdifGlobals.h"

SdifFileT*         SdifFOpen                    (const char *Name, SdifFileModeET Mode);
SdifFileT*         SdifOpenFile                 (const char *Name, SdifFileModeET Mode);
SdifFileT*         SdifFOpenText                (SdifFileT *SdifF, const char* Name, SdifFileModeET Mode);
void               SdifFClose                   (SdifFileT *SdifF);
void               SdifCloseFile                (SdifFileT *SdifF);
SdifFrameHeaderT*  SdifFCreateCurrFramH         (SdifFileT *SdifF, SdifSignature Signature);
SdifMatrixHeaderT* SdifFCreateCurrMtrxH         (SdifFileT *SdifF);
FILE*              SdifFGetFILE_SwitchVerbose   (SdifFileT *SdifF, int Verbose);
void               SdifTakeCodedPredefinedTypes (SdifFileT *SdifF);
void               SdifFLoadPredefinedTypes     (SdifFileT *SdifF, char *TypesFileName);

extern SdifFileT *gSdifPredefinedTypes;
void SdifGenInit (char *PredefinedTypesFile);
void SdifGenKill (void);
void SdifPrintVersion(void);



SdifFrameHeaderT* SdifFSetCurrFrameHeader (SdifFileT *SdifF, SdifSignature Signature, SdifUInt4 Size,
						   SdifUInt4 NbMatrix, SdifUInt4 NumID, SdifFloat8 Time);
SdifMatrixHeaderT* SdifFSetCurrMatrixHeader (SdifFileT *SdifF, SdifSignature Signature,
						    SdifDataTypeET DataType, SdifUInt4 NbRow, SdifUInt4 NbCol);

SdifOneRowT*  SdifFSetCurrOneRow       (SdifFileT *SdifF, void *Values);
SdifOneRowT*  SdifFSetCurrOneRowCol    (SdifFileT *SdifF, SdifUInt4 numCol, SdifFloat8 Value);
SdifFloat8    SdifFCurrOneRowCol       (SdifFileT *SdifF, SdifUInt4 numCol);
SdifFloat8    SdifFCurrOneRowColName   (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType, char *NameCD);
SdifSignature SdifFCurrSignature       (SdifFileT *SdifF);
SdifSignature SdifFCleanCurrSignature  (SdifFileT *SdifF);
SdifSignature SdifFCurrFrameSignature  (SdifFileT *SdifF);
SdifSignature SdifFCurrMatrixSignature (SdifFileT *SdifF);
SdifOneRowT*  SdifFCurrOneRow          (SdifFileT *SdifF);
void*	     SdifFCurrOneRowData	  (SdifFileT *SdifF);
SdifUInt4     SdifFCurrNbCol           (SdifFileT *SdifF);
SdifUInt4     SdifFCurrNbRow           (SdifFileT *SdifF);
SdifUInt4     SdifFCurrNbMatrix        (SdifFileT *SdifF);
SdifUInt4     SdifFCurrID              (SdifFileT *SdifF);
SdifFloat8    SdifFCurrTime            (SdifFileT *SdifF);


SdifSignatureTabT* SdifCreateSignatureTab (SdifUInt4 NbSignMax);
SdifSignatureTabT* SdifReInitSignatureTab (SdifSignatureTabT *SignTab, SdifUInt4 NewNbSignMax);
SdifSignatureTabT* SdifPutInSignatureTab  (SdifSignatureTabT *SignTab, SdifSignature Sign);
SdifSignature      SdifIsInSignatureTab   (SdifSignatureTabT *SignTab, SdifSignature Sign);
SdifFileT*    SdifFReInitMtrxUsed (SdifFileT *SdifF);
SdifFileT*    SdifFPutInMtrxUsed  (SdifFileT *SdifF, SdifSignature Sign);
SdifSignature SdifFIsInMtrxUsed   (SdifFileT *SdifF, SdifSignature Sign);


SdifErrorT*     SdifFLastError    (SdifFileT *SdifF);
SdifErrorTagET  SdifFLastErrorTag (SdifFileT *SdifF);

#endif /* _SdifFile_ */



