/* $Id: SdifFPrint.h,v 3.1.2.2 2000-08-21 21:35:06 tisseran Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
 *
 *
 * SdifFPrint.h
 *
 * F : SdifFileT* SdifF, Print : write only into the pseudo-sdif text file, SdifF->TextStream.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.1.2.1  2000/08/21  14:04:08  tisseran
 * *** empty log message ***
 *
 * Revision 3.1  1999/03/14  10:56:40  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:41  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:24  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:07  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:30  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.2  1998/11/10  15:31:41  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SDIFFPRINT_H
#define _SDIFFPRINT_H 1


#include "SdifGlobals.h"
#include <stdio.h>
#include "SdifFileStruct.h"
#include "SdifMatrix.h"
#include "SdifFrame.h"


size_t SdifFPrintGeneralHeader      (SdifFileT *SdifF);
size_t SdifFPrintNameValueLCurrNVT  (SdifFileT *SdifF);
size_t SdifFPrintAllNameValueNVT    (SdifFileT *SdifF);
size_t SdifFPrintAllType            (SdifFileT *SdifF);
size_t SdifFPrintAllStreamID        (SdifFileT *SdifF);
size_t SdifFPrintAllASCIIChunks     (SdifFileT *SdifF);
size_t SdifFPrintMatrixHeader       (SdifFileT *SdifF);
size_t SdifFPrintFrameHeader        (SdifFileT *SdifF);
size_t SdifFPrintOneRow             (SdifFileT *SdifF);

size_t SdifFPrintMatrixType         (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType);
size_t SdifFPrintFrameType          (SdifFileT *SdifF, SdifFrameTypeT  *FrameType);


/*
 * obsolete
 */
size_t SdifFPrintNameValueCurrHT (SdifFileT *SdifF);
size_t SdifFPrintAllNameValueHT  (SdifFileT *SdifF);

#endif /* _SdifFPrint_ */
