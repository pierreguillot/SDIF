/* $Id: SdifFRead.h,v 1.2 1998-11-10 15:31:42 schwarz Exp $
 *
 * SdifFRead.h
 *
 * F : SdifFileT* SdifF, Read : binary read (SdifF->Stream)
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */


#ifndef _SdifFRead_
#define _SdifFRead_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include <stdio.h>
#include "SdifMatrix.h"
#include "SdifFrame.h"
#include "SdifFGet.h"

size_t SdifFReadGeneralHeader    (SdifFileT *SdifF);
size_t SdifFReadChunkSize        (SdifFileT *SdifF);
size_t SdifFReadNameValueCurrHT  (SdifFileT *SdifF);
size_t SdifFReadAllType          (SdifFileT *SdifF);
size_t SdifFReadAllStreamID      (SdifFileT *SdifF);
size_t SdifFReadAllASCIIChunks   (SdifFileT *SdifF);
size_t SdifFReadMatrixHeader     (SdifFileT *SdifF);
size_t SdifFReadOneRow           (SdifFileT *SdifF);
size_t SdifFReadFrameHeader      (SdifFileT *SdifF);
size_t SdifSkipMatrix            (SdifFileT *SdifF);
size_t SdifSkipMatrixData        (SdifFileT *SdifF);
size_t SdifSkipFrameData         (SdifFileT *SdifF);
size_t SdifFReadPadding          (SdifFileT *SdifF, size_t Padding);
size_t SdifFReadUndeterminatedPadding (SdifFileT *SdifF);

size_t SdifFReadOneMatrixType    (SdifFileT *SdifF);
size_t SdifFReadOneFrameType     (SdifFileT *SdifF);


#endif /* _SdifFRead_ */
