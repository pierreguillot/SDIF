/* SdifFPrint.h
 *
 * F : SdifFileT* SdifF, Print : write only into the pseudo-sdif text file, SdifF->TextStream.
 *
 * author: Dominique Virolle 1997
 *
 *
 */


#ifndef _SdifFPrint_
#define _SdifFPrint_


#include "SdifGlobals.h"
#include <stdio.h>
#include "SdifFileStruct.h"
#include "SdifMatrix.h"
#include "SdifFrame.h"


extern size_t SdifFPrintGeneralHeader   (SdifFileT *SdifF);
extern size_t SdifFPrintNameValueCurrHT (SdifFileT *SdifF);
extern size_t SdifFPrintAllNameValueHT  (SdifFileT *SdifF);
extern size_t SdifFPrintAllType         (SdifFileT *SdifF);
extern size_t SdifFPrintAllStreamID     (SdifFileT *SdifF);
extern size_t SdifFPrintAllASCIIChunks  (SdifFileT *SdifF);
extern size_t SdifFPrintMatrixHeader    (SdifFileT *SdifF);
extern size_t SdifFPrintFrameHeader     (SdifFileT *SdifF);
extern size_t SdifFPrintOneRow          (SdifFileT *SdifF);

size_t SdifFPrintMatrixType      (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
size_t SdifFPrintFrameType       (SdifFileT *SdifF, SdifFrameTypeT  *FrameType);

#endif /* _SdifFPrint_ */
