/* $Id: SdifTest.h,v 1.4 1998-11-10 15:31:56 schwarz Exp $
 *
 * SdifTest.h
 *
 * Tests with message on errors apply on a SdifFileT*
 * exit(1) is exceptional
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */

#ifndef _SdifTest_
#define _SdifTest_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"

typedef enum SdifInterpretationErrorE
{
  eTypeDataNotSupported= 300,
  eNameLength,
  eReDefined,
  eUnDefined,
  eSyntax,
  eRecursiveDetect,
  eBadTypesFile,
  eBadType,
  eBadHeader,
  eOnlyOneChunkOf,
  eUnInterpreted,
  eUserDefInFileYet,
  eBadMode,
  eBadStdFile,
  eBadNbData,
  eReadWriteOnSameFile
} SdifInterpretationErrorET;



void SdifInterpretationError (SdifInterpretationErrorET Error, 
			      SdifFileT* SdifF, const void *ErrorMess);

#define _SdifFileMess(sdiff, error, mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifInterpretationError((error), (sdiff),(mess)))



SdifMatrixTypeT* SdifTestMatrixType(SdifFileT *SdifF, SdifSignature Signature);
short            SdifTestDataType  (SdifFileT* SdifF);
short            SdifTestNbColumns (SdifFileT* SdifF, SdifUInt4 NbData, 
				    SdifMatrixTypeT *MtrxT);
short            SdifTestMatrixHeader(SdifFileT* SdifF);

SdifColumnDefT*  SdifTestColumnDef (SdifFileT *SdifF, SdifMatrixTypeT *MtrxT, 
				    char *NameCD);
SdifFrameTypeT*  SdifTestFrameType (SdifFileT *SdifF, SdifSignature Signature);
SdifComponentT*  SdifTestComponent (SdifFileT* SdifF, SdifFrameTypeT *FramT,
				    char *NameCD);
short SdifTestFrameWithMatrixHeader(SdifFileT* SdifF, int NumComponent);

short SdifTestSignature            (SdifFileT *SdifF, int CharEnd, 
				    SdifSignature Signature, char *Mess);
short SdifTestCharEnd              (SdifFileT *SdifF, int CharEnd, char MustBe,
				    char *StringRead, short ErrCondition, 
				    char *Mess);

short SdifTestMatrixTypeModifMode  (SdifFileT *SdifF, SdifMatrixTypeT *MatrxT);
short SdifTestFrameTypeModifMode   (SdifFileT *SdifF, SdifFrameTypeT *FrmType);

#endif /* _ SdifTest_ */
