/* $Id: SdifFWrite.h,v 1.2 1998-11-10 15:31:44 schwarz Exp $
 *
 * SdifFWrite.h
 *
 * F : SdifFileT* SdifF, Write : sdif file write (SdifF->Stream)
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
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





void   SdifUpdateChunkSize      (SdifFileT *SdifF, size_t ChunkSize);
size_t SdifFWritePadding        (SdifFileT *SdifF, size_t Padding);
size_t SdifFWriteGeneralHeader  (SdifFileT *SdifF);
size_t SdifFWriteChunkHeader    (SdifFileT *SdifF, 
				 SdifSignature ChunkSignature, 
				 size_t ChunkSize);
size_t SdifFWriteNameValueCurrHT(SdifFileT *SdifF);
size_t SdifFWriteAllNameValueHT (SdifFileT *SdifF);
size_t SdifFWriteAllMatrixType  (SdifFileT* SdifF);
size_t SdifFWriteAllFrameType   (SdifFileT *SdifF);
size_t SdifFWriteAllType        (SdifFileT *SdifF);
size_t SdifFWriteAllStreamID    (SdifFileT *SdifF);
size_t SdifFWriteAllASCIIChunks (SdifFileT *SdifF);
size_t SdifFWriteMatrixHeader   (SdifFileT *SdifF);
size_t SdifFWriteOneRow         (SdifFileT *SdifF);
size_t SdifFWriteFrameHeader    (SdifFileT *SdifF);
       				
       				
size_t SdifFWriteOneNameValue   (SdifFileT *SdifF, SdifNameValueT  *NameValue);
size_t SdifFWriteOneMatrixType  (SdifFileT *SdifF, SdifMatrixTypeT *MatrixTyp);
size_t SdifFWriteOneComponent   (SdifFileT *SdifF, SdifComponentT  *Component);
size_t SdifFWriteOneFrameType   (SdifFileT *SdifF, SdifFrameTypeT  *FrameType);
size_t SdifFWriteOneStreamID    (SdifFileT *SdifF, SdifStreamIDT   *StreamID);

#endif /* _SdifFWrite_ */
