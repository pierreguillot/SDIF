/* SdifFScan.h
 *
 * F : SdifFileT* SdifF, Scan : pseudo-sdif text file read (SdifF->TextStream)
 *
 *
 * author: Dominique Virolle 1997
 *
 *
 */


#ifndef _SdifFScan_
#define _SdifFScan_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include <stdio.h>
#include "SdifMatrix.h"
#include "SdifFrame.h"

extern size_t  SdifFScanGeneralHeader   (SdifFileT *SdifF);
extern size_t  SdifFScanNameValueCurrHT (SdifFileT *SdifF);
extern size_t  SdifFScanAllType         (SdifFileT *SdifF);
extern size_t  SdifFScanAllStreamID     (SdifFileT *SdifF);
extern size_t  SdifFScanAllASCIIChunks  (SdifFileT *SdifF);
extern void    SdifFScanMatrixHeader    (SdifFileT *SdifF);
extern void    SdifFScanFrameHeader     (SdifFileT *SdifF);
extern void    SdifFScanOneRow          (SdifFileT *SdifF);

size_t  SdifFScanMatrixType      (SdifFileT *SdifF);
size_t  SdifFScanFrameType       (SdifFileT *SdifF);

#endif /* _SdifFScan_ */
