/* $Id: SdifFPrint.h,v 2.0 1998-11-29 11:41:30 virolle Exp $
 *
 * SdifFPrint.h
 *
 * F : SdifFileT* SdifF, Print : write only into the pseudo-sdif text file, SdifF->TextStream.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.2  1998/11/10  15:31:41  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifFPrint_
#define _SdifFPrint_


#include "SdifGlobals.h"
#include <stdio.h>
#include "SdifFileStruct.h"
#include "SdifMatrix.h"
#include "SdifFrame.h"


size_t SdifFPrintGeneralHeader   (SdifFileT *SdifF);
size_t SdifFPrintNameValueCurrHT (SdifFileT *SdifF);
size_t SdifFPrintAllNameValueHT  (SdifFileT *SdifF);
size_t SdifFPrintAllType         (SdifFileT *SdifF);
size_t SdifFPrintAllStreamID     (SdifFileT *SdifF);
size_t SdifFPrintAllASCIIChunks  (SdifFileT *SdifF);
size_t SdifFPrintMatrixHeader    (SdifFileT *SdifF);
size_t SdifFPrintFrameHeader     (SdifFileT *SdifF);
size_t SdifFPrintOneRow          (SdifFileT *SdifF);

size_t SdifFPrintMatrixType      (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
size_t SdifFPrintFrameType       (SdifFileT *SdifF, SdifFrameTypeT  *FrameType);

#endif /* _SdifFPrint_ */
