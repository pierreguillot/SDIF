/* $Id: SdifFScan.h,v 1.2 1998-11-10 15:31:43 schwarz Exp $
 *
 * SdifFScan.h
 *
 * F : SdifFileT* SdifF, Scan : pseudo-sdif text file read (SdifF->TextStream)
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */


#ifndef _SdifFScan_
#define _SdifFScan_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include <stdio.h>
#include "SdifMatrix.h"
#include "SdifFrame.h"

size_t  SdifFScanGeneralHeader   (SdifFileT *SdifF);
size_t  SdifFScanNameValueCurrHT (SdifFileT *SdifF);
size_t  SdifFScanAllType         (SdifFileT *SdifF);
size_t  SdifFScanAllStreamID     (SdifFileT *SdifF);
size_t  SdifFScanAllASCIIChunks  (SdifFileT *SdifF);
void    SdifFScanMatrixHeader    (SdifFileT *SdifF);
void    SdifFScanFrameHeader     (SdifFileT *SdifF);
void    SdifFScanOneRow          (SdifFileT *SdifF);

size_t  SdifFScanMatrixType      (SdifFileT *SdifF);
size_t  SdifFScanFrameType       (SdifFileT *SdifF);

#endif /* _SdifFScan_ */
