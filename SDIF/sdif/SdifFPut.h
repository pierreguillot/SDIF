/* $Id: SdifFPut.h,v 3.2 2000-04-11 14:31:22 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
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
 * Revision 3.1  1999/03/14  10:56:42  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:42  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:26  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:09  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:32  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
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

size_t SdifFPutOneNameValue         (SdifFileT *SdifF, int Verbose, SdifNameValueT *NameValue);
size_t SdifFPutNameValueLCurrNVT    (SdifFileT *SdifF, int Verbose);
size_t SdifFNameValueLCurrNVTtoString(SdifFileT *SdifF, char *str, int maxlen);
size_t SdifFPutOneMatrixType        (SdifFileT *SdifF, int Verbose, SdifMatrixTypeT *MatrixType);
size_t SdifFPutAllMatrixType        (SdifFileT *SdifF, int Verbose);
size_t SdifFPutOneComponent         (SdifFileT *SdifF, int Verbose, SdifComponentT *Component);
size_t SdifFPutOneFrameType         (SdifFileT *SdifF, int Verbose, SdifFrameTypeT *FrameType);
size_t SdifFPutAllFrameType         (SdifFileT *SdifF, int Verbose);
size_t SdifFPutAllType              (SdifFileT *SdifF, int Verbose);
size_t SdifFPutOneStreamID          (SdifFileT *SdifF, int Verbose, SdifStreamIDT *StreamID);
size_t SdifFPutAllStreamID          (SdifFileT *SdifF, int Verbose);

/*
 * obsolete
 */
size_t SdifFPutNameValueCurrHT      (SdifFileT *SdifF, int Verbose);

#endif /* _SdifFPut_ */
