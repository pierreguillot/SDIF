/* $Id: SdifConvToText.h,v 2.0 1998-11-29 11:41:22 virolle Exp $
 *
 * SdifConvToText.h
 *
 * functions to convert an sdif file into a pseudo-sdif text file.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.3  1998/11/10  15:31:39  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */

#ifndef _SdifConvToText_
#define _SdifConvToText_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"



size_t SdifFConvToTextMatrixData     (SdifFileT *SdifF);
size_t SdifFConvToTextMatrixHeader   (SdifFileT *SdifF);
size_t SdifFConvToTextMatrix         (SdifFileT *SdifF);
size_t SdifFConvToTextFrameData      (SdifFileT *SdifF);
size_t SdifFConvToTextFrameHeader    (SdifFileT *SdifF);
size_t SdifFConvToTextFrame          (SdifFileT *SdifF);
size_t SdifFConvToTextAllFrame       (SdifFileT *SdifF);
size_t SdifFConvToText               (SdifFileT *SdifF);

size_t SdifToText (SdifFileT *SdifF, char *TextStreamName);

#endif /* _SdifConvToText_ */
