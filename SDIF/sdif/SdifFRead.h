/* SdifFRead.h
 *
 * F : SdifFileT* SdifF, Read : binary read (SdifF->Stream)
 *
 * author: Dominique Virolle 1997
 *
 *
 */


#ifndef _SdifFRead_
#define _SdifFRead_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include <stdio.h>
#include "SdifMatrix.h"
#include "SdifFrame.h"
#include "SdifFGet.h"

extern size_t SdifFReadGeneralHeader    (SdifFileT *SdifF);
extern size_t SdifFReadChunkSize        (SdifFileT *SdifF);
extern size_t SdifFReadNameValueCurrHT  (SdifFileT *SdifF);
extern size_t SdifFReadAllType          (SdifFileT *SdifF);
extern size_t SdifFReadAllStreamID      (SdifFileT *SdifF);
extern size_t SdifFReadAllASCIIChunks   (SdifFileT *SdifF);
extern size_t SdifFReadMatrixHeader     (SdifFileT *SdifF);
extern size_t SdifFReadOneRow           (SdifFileT *SdifF);
extern size_t SdifFReadFrameHeader      (SdifFileT *SdifF);
extern size_t SdifSkipMatrix            (SdifFileT *SdifF);
extern size_t SdifSkipMatrixData        (SdifFileT *SdifF);
extern size_t SdifSkipFrameData         (SdifFileT *SdifF);
extern size_t SdifFReadPadding          (SdifFileT *SdifF, size_t Padding);
extern size_t SdifFReadUndeterminatedPadding (SdifFileT *SdifF);

size_t SdifFReadOneMatrixType    (SdifFileT *SdifF);
size_t SdifFReadOneFrameType     (SdifFileT *SdifF);


#endif /* _SdifFRead_ */
