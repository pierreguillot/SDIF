/* $Id: SdifConvToText.h,v 3.5 2000-11-21 14:51:47 schwarz Exp $
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
 * SdifConvToText.h
 *
 * functions to convert an sdif file into a pseudo-sdif text file.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.4  2000/10/27 20:03:24  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.3.2.2  2000/08/21  21:34:58  tisseran
 * *** empty log message ***
 *
 * Revision 3.3.2.1  2000/08/21  14:04:02  tisseran
 * *** empty log message ***
 *
 * Revision 3.3  1999/09/28  13:08:49  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.2  1999/08/25  18:32:33  schwarz
 * Added cocoon-able comments with sentinel "DOC:" (on a single line).
 *
 * Revision 3.1  1999/03/14  10:56:32  virolle
 * SdifStdErr add
 *
 * Revision 2.1  1998/12/21  18:27:00  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:22  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/11/10  15:31:39  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */

#ifndef _SDIFCONVTOTEXT_H
#define _SDIFCONVTOTEXT_H 1

#include "SdifGlobals.h"


size_t SdifFConvToTextMatrixData     (SdifFileT *SdifF);
size_t SdifFConvToTextMatrixHeader   (SdifFileT *SdifF);
size_t SdifFConvToTextMatrix         (SdifFileT *SdifF);
size_t SdifFConvToTextFrameData      (SdifFileT *SdifF);
size_t SdifFConvToTextFrameHeader    (SdifFileT *SdifF);
size_t SdifFConvToTextFrame          (SdifFileT *SdifF);
size_t SdifFConvToTextAllFrame       (SdifFileT *SdifF);
size_t SdifFConvToText               (SdifFileT *SdifF);


/*DOC: 
  Converti un fichier texte pseudo-SDIF de nom TextStreamName en un
  fichier SDIF binaire de non SdifF->Name. Le fichier doit avoir été
  ouvert en écriture (eWriteFile).  */
size_t SdifToText (SdifFileT *SdifF, char *TextStreamName);

#endif /* _SdifConvToText_ */
