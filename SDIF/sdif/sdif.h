/* $Id: sdif.h,v 3.6.2.2 2000-08-21 21:35:59 tisseran Exp $
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
 * sdif.h
 *
 * Sound Description Interchange Format
 *
 * sdif library for management.
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.6.2.1  2000/08/21  14:04:27  tisseran
 * *** empty log message ***
 *
 * Revision 3.6  2000/07/18  15:08:42  tisseran
 * This release implements the New SDIF Specification (june 1999):
 * - Name Values Table are written in a 1NVT frame which contains a 1NVT matrix
 * - Frame and matrix type declaration are written in a 1TYP frame which contains a 1TYP matrix.
 * - Stream ID are written in a 1IDS frame which contains a 1IDS matrix.
 *
 * Read function accept the previous version of the specification (read a text frame without matrix) to be compatible with older SDIF files.
 *
 * SdifString.h and SdifString.c implements some string mangement (creation, destruction, append, test of end of string, getc, ungetc).
 *
 * WATCH OUT:
 *      We don't care about the old SDIF Specification (_SdifFormatVersion < 3)
 * To use _SdifFormatVersion < 3, get the previous release.
 *
 * Revision 3.5  2000/07/06  19:01:50  tisseran
 * Add function for frame and matrix type declaration
 * Remove string size limitation for NameValueTable
 * TODO: 1TYP and 1IDS frame must contain an 1NVT (text) matrix
 *       Actually, data are written with fprintf.
 *
 * Revision 3.4  2000/05/04  15:03:43  schwarz
 * Added modules SdifCheck and SdifSignatureTab.
 *
 * Revision 3.3  2000/03/01  11:20:20  schwarz
 * Added preliminary sketch of SdifHighLevel
 *
 * Revision 3.2  1999/08/31  10:02:58  schwarz
 * Added module SdifSelect which parses an access specification to a
 * chosen part of SDIF data.  Can be added to a file name.
 *
 * Revision 3.1  1999/03/14  10:57:29  virolle
 * SdifStdErr add
 *
 * Revision 2.1  1998/12/21  18:27:46  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:42:14  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/11/10  15:31:58  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */

#ifndef _sdif_
#define _sdif_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "config.h" /* For autoconf */
  
#include "SdifHash.h"
#include "SdifError.h"
#include "SdifGlobals.h"
#include "SdifRWLowLevel.h"

#include "SdifNameValue.h"
#include "SdifStreamID.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"

#include "SdifMatrix.h"
#include "SdifFrame.h"

#include "SdifTimePosition.h"
#include "SdifSignatureTab.h"
#include "SdifFileStruct.h"
#include "SdifTest.h"
#include "SdifFile.h"

#include "SdifPrint.h"

#include "SdifFGet.h"
#include "SdifFRead.h"
#include "SdifFScan.h"

#include "SdifFPut.h"
#include "SdifFPrint.h"
#include "SdifFWrite.h"
#include "SdifHighLevel.h"

#include "SdifTextConv.h"
#include "SdifConvToText.h"

#include "SdifPreTypes.h"
#include "SdifSelect.h"
#include "SdifCheck.h"

#include "SdifString.h"
  
#ifdef __cplusplus
}
#endif

#endif /* _sdif_ */
