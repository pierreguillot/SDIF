/* $Id: SdifFPut.h,v 2.0 1998-11-29 11:41:32 virolle Exp $
 *
 * SdifFPut.h
 *
 *
 * F : SdifFileT* SdifF, Put : write ascii frames into pseudo-sdif text file,
 * SdifF->TextStream or sdif file SdifF->Stream.
 * switch 'verbose'
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.2  1998/11/10  15:31:42  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifFPut_
#define _SdifFPut_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include <stdio.h>
#include "SdifNameValue.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"

size_t SdifFPutOneNameValue    (SdifFileT* SdifF, int Verbose, SdifNameValueT *NameValue);
size_t SdifFPutNameValueCurrHT (SdifFileT *SdifF, int Verbose);
size_t SdifFPutOneMatrixType   (SdifFileT* SdifF, int Verbose, SdifMatrixTypeT *MatrixType);
size_t SdifFPutAllMatrixType   (SdifFileT* SdifF, int Verbose);
size_t SdifFPutOneComponent    (SdifFileT *SdifF, int Verbose, SdifComponentT *Component);
size_t SdifFPutOneFrameType    (SdifFileT *SdifF, int Verbose, SdifFrameTypeT *FrameType);
size_t SdifFPutAllFrameType    (SdifFileT *SdifF, int Verbose);
size_t SdifFPutAllType         (SdifFileT *SdifF, int Verbose);
size_t SdifFPutOneStreamID     (SdifFileT *SdifF, int Verbose, SdifStreamIDT *StreamID);
size_t SdifFPutAllStreamID     (SdifFileT *SdifF, int Verbose);


#endif /* _SdifFPut_ */
