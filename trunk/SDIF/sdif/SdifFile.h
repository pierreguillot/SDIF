/* $Id: SdifFile.h,v 1.4 1998-05-14 09:50:35 schwarz Exp $
 *
 * SdifFile.h
 *
 * Sdif Files management
 * High Level.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */



#ifndef _SdifFile_
#define _SdifFile_

#include "SdifFileStruct.h"
#include "SdifGlobals.h"

extern SdifFileT*         SdifOpenFile                  (const char* Name, SdifFileModeET Mode);
extern void               SdifCloseFile                 (SdifFileT* SdifF);
extern SdifFrameHeaderT*  SdifFileCreateCurrFramH       (SdifFileT* SdifF, SdifSignature Signature);
extern SdifMatrixHeaderT* SdifFileCreateCurrMtrxH       (SdifFileT* SdifF);
extern FILE*              SdifFileGetFILE_SwitchVerbose (SdifFileT* SdifF, int Verbose);
extern void               SdifTakeCodedPredefinedTypes  (SdifFileT *SdifF);
extern void               SdifFLoadPredefinedTypes      (SdifFileT *SdifF, char *TypesFileName);

extern SdifFileT *gSdifPredefinedTypes;
extern void SdifGenInit (char *PredefinedTypesFile);
extern void SdifGenKill (void);
extern void SdifPrintVersion(void);



extern SdifFrameHeaderT* SdifSetCurrFrameHeader   (SdifFileT *SdifF, SdifSignature Signature, SdifUInt4 Size,
						   SdifUInt4 NbMatrix, SdifUInt4 NumID, SdifFloat8 Time);
extern SdifMatrixHeaderT* SdifSetCurrMatrixHeader  (SdifFileT *SdifF, SdifSignature Signature,
						    SdifDataTypeET DataType, SdifUInt4 NbRow, SdifUInt4 NbCol);

extern SdifOneRowT*  SdifSetCurrOneRow       (SdifFileT *SdifF, void *Values);
extern SdifOneRowT*  SdifSetCurrOneRowCol    (SdifFileT *SdifF, SdifUInt4 numCol, SdifFloat8 Value);
extern SdifFloat8    SdifCurrOneRowCol       (SdifFileT *SdifF, SdifUInt4 numCol);
extern SdifFloat8    SdifCurrOneRowColName   (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType, char *NameCD);
extern SdifSignature SdifCurrSignature       (SdifFileT *SdifF);
extern SdifSignature SdifCleanCurrSignature  (SdifFileT *SdifF);
extern SdifSignature SdifCurrFrameSignature  (SdifFileT *SdifF);
extern SdifSignature SdifCurrMatrixSignature (SdifFileT *SdifF);
extern SdifOneRowT*  SdifCurrOneRow          (SdifFileT *SdifF);
extern void*	     SdifCurrOneRowData	     (SdifFileT *SdifF);
extern SdifUInt4     SdifCurrNbCol           (SdifFileT *SdifF);
extern SdifUInt4     SdifCurrNbRow           (SdifFileT *SdifF);
extern SdifUInt4     SdifCurrNbMatrix        (SdifFileT *SdifF);
extern SdifUInt4     SdifCurrID              (SdifFileT *SdifF);
extern SdifFloat8    SdifCurrTime            (SdifFileT *SdifF);

#endif /* _SdifFile_ */



