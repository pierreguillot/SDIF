/* SdifFWrite.h
 *
 * F : SdifFileT* SdifF, Write : sdif file write (SdifF->Stream)
 *
 *
 * author: Dominique Virolle 1997
 *
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





extern void    SdifUpdateChunkSize       (SdifFileT *SdifF, size_t ChunkSize);
extern size_t  SdifFWritePadding         (SdifFileT *SdifF, size_t Padding);
extern size_t  SdifFWriteGeneralHeader   (SdifFileT *SdifF);
extern size_t  SdifFWriteChunkHeader     (SdifFileT *SdifF, SdifSignature ChunkSignature, size_t ChunkSize);
extern size_t  SdifFWriteNameValueCurrHT (SdifFileT *SdifF);
extern size_t  SdifFWriteAllNameValueHT  (SdifFileT *SdifF);
extern size_t  SdifFWriteAllMatrixType   (SdifFileT* SdifF);
extern size_t  SdifFWriteAllFrameType    (SdifFileT *SdifF);
extern size_t  SdifFWriteAllType         (SdifFileT *SdifF);
extern size_t  SdifFWriteAllStreamID     (SdifFileT *SdifF);
extern size_t  SdifFWriteAllASCIIChunks  (SdifFileT *SdifF);
extern size_t  SdifFWriteMatrixHeader    (SdifFileT *SdifF);
extern size_t  SdifFWriteOneRow          (SdifFileT *SdifF);
extern size_t  SdifFWriteFrameHeader     (SdifFileT *SdifF);


size_t  SdifFWriteOneNameValue    (SdifFileT *SdifF, SdifNameValueT  *NameValue);
size_t  SdifFWriteOneMatrixType   (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
size_t  SdifFWriteOneComponent    (SdifFileT *SdifF, SdifComponentT  *Component);
size_t  SdifFWriteOneFrameType    (SdifFileT *SdifF, SdifFrameTypeT  *FrameType);
size_t  SdifFWriteOneStreamID     (SdifFileT *SdifF, SdifStreamIDT   *StreamID);

#endif /* _SdifFWrite_ */
