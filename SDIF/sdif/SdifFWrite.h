/* $Id: SdifFWrite.h,v 3.1 1999-03-14 10:56:48 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifFWrite.h
 *
 * F : SdifFileT* SdifF, Write : sdif file write (SdifF->Stream)
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.3  1999/01/23  15:55:47  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:31  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:14  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:39  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.2  1998/11/10  15:31:44  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifFWrite_
#define _SdifFWrite_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"

#include <stdio.h>
#include "SdifNameValue.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifMatrix.h"
#include "SdifFrame.h"





void    SdifUpdateChunkSize         (SdifFileT *SdifF, size_t ChunkSize);
size_t  SdifFWritePadding           (SdifFileT *SdifF, size_t Padding);
size_t  SdifFWriteGeneralHeader     (SdifFileT *SdifF);
size_t  SdifFWriteChunkHeader       (SdifFileT *SdifF, SdifSignature ChunkSignature, size_t ChunkSize);
size_t  SdifFWriteNameValueLCurrNVT (SdifFileT *SdifF);
size_t  SdifFWriteAllNameValueNVT   (SdifFileT *SdifF);
size_t  SdifFWriteAllMatrixType     (SdifFileT *SdifF);
size_t  SdifFWriteAllFrameType      (SdifFileT *SdifF);
size_t  SdifFWriteAllType           (SdifFileT *SdifF);
size_t  SdifFWriteAllStreamID       (SdifFileT *SdifF);
size_t  SdifFWriteAllASCIIChunks    (SdifFileT *SdifF);
size_t  SdifFWriteMatrixHeader      (SdifFileT *SdifF);
size_t  SdifFWriteOneRow            (SdifFileT *SdifF);
size_t  SdifFWriteFrameHeader       (SdifFileT *SdifF);


size_t  SdifFWriteOneNameValue      (SdifFileT *SdifF, SdifNameValueT  *NameValue);
size_t  SdifFWriteOneMatrixType     (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
size_t  SdifFWriteOneComponent      (SdifFileT *SdifF, SdifComponentT  *Component);
size_t  SdifFWriteOneFrameType      (SdifFileT *SdifF, SdifFrameTypeT  *FrameType);
size_t  SdifFWriteOneStreamID       (SdifFileT *SdifF, SdifStreamIDT   *StreamID);

/*
 * obsolete
 */
size_t  SdifFWriteNameValueCurrHT   (SdifFileT *SdifF);
size_t  SdifFWriteAllNameValueHT    (SdifFileT *SdifF);

#endif /* _SdifFWrite_ */
