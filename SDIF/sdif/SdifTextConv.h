/* SdifTextConv.h
 *
 *
 * Convert a pseudo-sdif text file to a sdif file
 *
 *
 * author: Dominique Virolle 1997
 *
 *
 */


#ifndef _SdifTextConv_
#define _SdifTextConv_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"


extern size_t SdifFTextConvMatrixData     (SdifFileT *SdifF);
extern size_t SdifFTextConvMatrix         (SdifFileT *SdifF);
extern size_t SdifFTextConvFrameData      (SdifFileT *SdifF);
extern size_t SdifFTextConvFrameHeader    (SdifFileT *SdifF);
extern size_t SdifFTextConvFrame          (SdifFileT *SdifF);
extern size_t SdifFTextConvAllFrame       (SdifFileT *SdifF);
extern size_t SdifFTextConvFramesChunk    (SdifFileT *SdifF);
extern size_t SdifFTextConv               (SdifFileT *SdifF);

/* upper level : open the text in read mode */
extern size_t SdifTextToSdif (SdifFileT *SdifF, char *TextStreamName);

#endif   /* _SdifTextConv_ */
