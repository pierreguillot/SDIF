/* $Id: SdifPrint.h,v 3.2 2000-10-27 20:03:41 roebel Exp $
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
 * SdifPrint.h
 *
 * for message at sdif interpretation
 * print of structures with some reference
 * FILE* is usually stdout or stderr
 *
 * author: Dominique Virolle 1997
 * $Log: not supported by cvs2svn $
 * Revision 3.1.2.1  2000/08/21  21:35:43  tisseran
 * *** empty log message ***
 *
 * Revision 3.1  1999/03/14  10:57:17  virolle
 * SdifStdErr add
 *
 * Revision 2.1  1998/12/21  18:27:36  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:42:02  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/11/10  15:31:54  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */

#ifndef _SdifPrint_
#define _SdifPrint_

#include "SdifGlobals.h"
#include <stdio.h>
#include "SdifNameValue.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifMatrix.h"
#include "SdifFrame.h"
#include "SdifFileStruct.h"



/*************** Matrix Type ***************/

void SdifPrintMatrixType(FILE *fw, SdifMatrixTypeT *MatrixType);
void SdifPrintAllMatrixType(FILE *fw, SdifFileT *SdifF);

/*************** Frame Type ***************/

void SdifPrintFrameType(FILE *fw, SdifFrameTypeT *FrameType);
void SdifPrintAllFrameType(FILE *fw, SdifFileT *SdifF);

/********** Matrix **********/

void SdifPrintMatrixHeader(FILE *f, SdifMatrixHeaderT *MatrixHeader);
void SdifPrintOneRow(FILE *f, SdifOneRowT *OneRow);
void SdifPrintMatrixRows(FILE* f, SdifMatrixDataT *MatrixData);

/********** Frame ***********/

void SdifPrintFrameHeader(FILE *f, SdifFrameHeaderT* FrameHeader);

/************ High ***********/

void SdifPrintAllType(FILE *fw, SdifFileT *SdifF);

#endif /* _SdifPrint_ */
