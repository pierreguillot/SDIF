/* $Id: SdifFile.h,v 1.6 1998-11-10 15:31:45 schwarz Exp $
 *
 * SdifFile.h
 *
 * Sdif Files management
 * High Level.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
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


SdifFileT*         SdifFOpen                     (const char* Name, 
						  SdifFileModeET Mode);
SdifFileT*         SdifOpenFile                  (const char* Name, 
						  SdifFileModeET Mode);
SdifFileT*         SdifFOpenText                 (SdifFileT* SdifF, 
						  const char* Name, 
						  SdifFileModeET Mode);
void               SdifFClose                    (SdifFileT* SdifF);
void               SdifCloseFile                 (SdifFileT* SdifF);
SdifFrameHeaderT*  SdifFileCreateCurrFramH       (SdifFileT* SdifF, 
						  SdifSignature Signature);
SdifMatrixHeaderT* SdifFileCreateCurrMtrxH       (SdifFileT* SdifF);
FILE*              SdifFileGetFILE_SwitchVerbose (SdifFileT* SdifF, 
						  int Verbose);
void               SdifTakeCodedPredefinedTypes  (SdifFileT *SdifF);
void               SdifFLoadPredefinedTypes      (SdifFileT *SdifF, 
						  char *TypesFileName);

extern SdifFileT *gSdifPredefinedTypes;
void SdifGenInit (char *PredefinedTypesFile);
void SdifGenKill (void);
void SdifPrintVersion (void);


SdifFrameHeaderT* SdifSetCurrFrameHeader    (SdifFileT *SdifF,
					     SdifSignature Signature,
					     SdifUInt4 Size,
					     SdifUInt4 NbMatrix,
					     SdifUInt4 NumID,
					     SdifFloat8 Time);
SdifMatrixHeaderT* SdifSetCurrMatrixHeader  (SdifFileT *SdifF,
					     SdifSignature Signature,
					     SdifDataTypeET DataType,
					     SdifUInt4 NbRow,
					     SdifUInt4 NbCol);

SdifOneRowT*  SdifSetCurrOneRow       (SdifFileT *SdifF,
				       void *Values);
SdifOneRowT*  SdifSetCurrOneRowCol    (SdifFileT *SdifF,
				       SdifUInt4 numCol,
				       SdifFloat8 Value);
SdifFloat8    SdifCurrOneRowCol       (SdifFileT *SdifF,
				       SdifUInt4 numCol);
SdifFloat8    SdifCurrOneRowColName   (SdifFileT *SdifF,
				       SdifMatrixTypeT *MatrixType,
				       char *NameCD);
SdifSignature SdifCurrSignature       (SdifFileT *SdifF);
SdifSignature SdifCleanCurrSignature  (SdifFileT *SdifF);
SdifSignature SdifCurrFrameSignature  (SdifFileT *SdifF);
SdifSignature SdifCurrMatrixSignature (SdifFileT *SdifF);
SdifOneRowT*  SdifCurrOneRow          (SdifFileT *SdifF);
void*	      SdifCurrOneRowData      (SdifFileT *SdifF);
SdifUInt4     SdifCurrNbCol           (SdifFileT *SdifF);
SdifUInt4     SdifCurrNbRow           (SdifFileT *SdifF);
SdifUInt4     SdifCurrNbMatrix        (SdifFileT *SdifF);
SdifUInt4     SdifCurrID              (SdifFileT *SdifF);
SdifFloat8    SdifCurrTime            (SdifFileT *SdifF);

#endif /* _SdifFile_ */



