/* $Id: SdifTextConv.h,v 2.1 1998-12-21 18:27:44 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifTextConv.h
 *
 *
 * Convert a pseudo-sdif text file to a sdif file
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.0  1998/11/29  11:42:11  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/11/10  15:31:57  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifTextConv_
#define _SdifTextConv_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"


size_t SdifFTextConvMatrixData     (SdifFileT *SdifF);
size_t SdifFTextConvMatrix         (SdifFileT *SdifF);
size_t SdifFTextConvFrameData      (SdifFileT *SdifF);
size_t SdifFTextConvFrameHeader    (SdifFileT *SdifF);
size_t SdifFTextConvFrame          (SdifFileT *SdifF);
size_t SdifFTextConvAllFrame       (SdifFileT *SdifF);
size_t SdifFTextConvFramesChunk    (SdifFileT *SdifF);
size_t SdifFTextConv               (SdifFileT *SdifF);

/* upper level : open the text in read mode */
size_t SdifTextToSdif (SdifFileT *SdifF, char *TextStreamName);

#endif   /* _SdifTextConv_ */