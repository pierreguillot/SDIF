/* $id$
 *
 * SdifTest.h
 *
 * Tests with message on errors apply on a SdifFileT*
 * exit(1) is exceptional
 *
 *
 * author: Dominique Virolle 1997
 *
 * $log$
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



extern void
SdifInterpretationError(SdifInterpretationErrorET Error, SdifFileT* SdifF, const void *ErrorMess);

#define _SdifFileMess(sdiff, error, mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifInterpretationError((error), (sdiff),(mess)))




extern SdifMatrixTypeT* SdifTestMatrixType(SdifFileT *SdifF, SdifSignature Signature);
extern short            SdifTestDataType  (SdifFileT* SdifF);
extern short            SdifTestNbColumns (SdifFileT* SdifF, SdifUInt4 NbData, SdifMatrixTypeT *MtrxT);
extern short            SdifTestMatrixHeader(SdifFileT* SdifF);

extern SdifColumnDefT*  SdifTestColumnDef (SdifFileT *SdifF, SdifMatrixTypeT *MtrxT, char *NameCD);
extern SdifFrameTypeT*  SdifTestFrameType (SdifFileT *SdifF, SdifSignature Signature);
extern SdifComponentT*  SdifTestComponent (SdifFileT* SdifF, SdifFrameTypeT *FramT, char *NameCD);
extern short SdifTestFrameWithMatrixHeader(SdifFileT* SdifF, int NumComponent);

extern short SdifTestSignature            (SdifFileT *SdifF, int CharEnd, SdifSignature Signature, char *Mess);
extern short SdifTestCharEnd              (SdifFileT *SdifF, int CharEnd, char MustBe,
					   char *StringRead, short ErrCondition, char *Mess);

extern short SdifTestMatrixTypeModifMode  (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
extern short SdifTestFrameTypeModifMode   (SdifFileT *SdifF, SdifFrameTypeT *FrameType);

#endif /* _ SdifTest_ */
