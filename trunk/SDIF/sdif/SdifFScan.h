/* $Id: SdifFScan.h,v 2.0 1998-11-29 11:41:36 virolle Exp $
 *
 * SdifFScan.h
 *
 * F : SdifFileT* SdifF, Scan : pseudo-sdif text file read (SdifF->TextStream)
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.2  1998/11/10  15:31:43  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
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
