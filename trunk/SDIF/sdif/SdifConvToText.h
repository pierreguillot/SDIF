/* SdifConvToText.h
 *
 * functions to convert an sdif file into a pseudo-sdif text file.
 *
 * author: Dominique Virolle 1997
 *
 *
 */

#ifndef _SdifConvToText_
#define _SdifConvToText_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"



extern size_t SdifFConvToTextMatrixData     (SdifFileT *SdifF);
extern size_t SdifFConvToTextMatrixHeader   (SdifFileT *SdifF);
extern size_t SdifFConvToTextMatrix         (SdifFileT *SdifF);
extern size_t SdifFConvToTextFrameData      (SdifFileT *SdifF);
extern size_t SdifFConvToTextFrameHeader    (SdifFileT *SdifF);
extern size_t SdifFConvToTextFrame          (SdifFileT *SdifF);
extern size_t SdifFConvToTextAllFrame       (SdifFileT *SdifF);
extern size_t SdifFConvToText               (SdifFileT *SdifF);

extern size_t SdifToText (SdifFileT *SdifF, char *TextStreamName);

#endif /* _SdifConvToText_ */
