/* $Id: SdifTest.h,v 2.0 1998-11-29 11:42:09 virolle Exp $
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
 * Revision 1.4  1998/11/10  15:31:56  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */

#ifndef _SdifTest_
#define _SdifTest_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"

/*
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



void
SdifInterpretationError(SdifInterpretationErrorET Error, SdifFileT* SdifF, const void *ErrorMess);

#define _SdifFileMess(sdiff, error, mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifInterpretationError((error), (sdiff),(mess)))

*/

#define _SdifFileMess(sdiff, error, mess) 

SdifMatrixTypeT* SdifTestMatrixType (SdifFileT *SdifF, SdifSignature Signature);
SdifFrameTypeT*  SdifTestFrameType  (SdifFileT *SdifF, SdifSignature Signature);



short SdifFTestMatrixWithFrameHeader (SdifFileT* SdifF);
short SdifFTestDataType              (SdifFileT* SdifF);
short SdifFTestNbColumns             (SdifFileT* SdifF);
short SdifFTestNotEmptyMatrix        (SdifFileT* SdifF);
short SdifFTestMatrixHeader          (SdifFileT* SdifF);



SdifColumnDefT*  SdifTestColumnDef (SdifFileT *SdifF, SdifMatrixTypeT *MtrxT, char *NameCD);
SdifComponentT*  SdifTestComponent (SdifFileT* SdifF, SdifFrameTypeT *FramT, char *NameCD);

short SdifTestSignature            (SdifFileT *SdifF, int CharEnd, SdifSignature Signature, char *Mess);
short SdifTestCharEnd              (SdifFileT *SdifF, int CharEnd, char MustBe,
					   char *StringRead, short ErrCondition, char *Mess);

short SdifTestMatrixTypeModifMode  (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
short SdifTestFrameTypeModifMode   (SdifFileT *SdifF, SdifFrameTypeT *FrameType);

#endif /* _ SdifTest_ */
