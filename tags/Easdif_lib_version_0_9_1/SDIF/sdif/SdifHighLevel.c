/* $Id: SdifHighLevel.c,v 3.9 2004-06-03 11:18:00 schwarz Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
 *
 *
 *
 * SdifHighLevel.c	8.12.1999	Diemo Schwarz
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.8  2003/11/07 21:47:18  roebel
 * removed XpGuiCalls.h and replaced preinclude.h  by local files
 *
 * Revision 3.7  2002/08/28 16:52:39  schwarz
 * Fixed bug that end of file was not recognised:
 * signature is then eEmptySignature, and not eEof.
 * Error check for SdifFReadFrameHeader.
 *
 * Revision 3.6  2001/05/02 09:34:44  tisseran
 * Change License from GNU Public License to GNU Lesser Public License.
 *
 * Revision 3.5  2000/11/21 14:51:50  schwarz
 * - sdif.h is now included by all sdif/Sdif*.c files.
 * - Removed all public typedefs, enums, structs, and defines from the
 *   individual sdif/Sdif*.h files, because they were duplicated in sdif.h.
 * - Todo: Do the same for the function prototypes, decide which types and
 *   prototypes really need to be exported.
 * - Removed SdifFileStruct.h.
 * - Preliminary new version of SdiffGetPos, SdiffSetPos.  They used the
 *   type fpos_t, which is no longer a long on RedHat 7 Linux.
 *
 * Revision 3.4  2000/11/15 14:53:31  lefevre
 * no message
 *
 * Revision 3.3  2000/10/27  20:03:36  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.2  2000/08/22  13:38:33  schwarz
 * First alpha version of SdifFReadNextSelectedFrameHeader.
 *
 * Revision 3.1.2.1  2000/08/21  21:35:29  tisseran
 * *** empty log message ***
 *
 * Revision 3.1  2000/03/01  11:20:20  schwarz
 * Added preliminary sketch of SdifHighLevel
 *
 */


/* include */		 /* to get these definitions: */
#include "sdif_portability.h"

#include "SdifGlobals.h"
#include "SdifFile.h"
#include "SdifFRead.h"
#include "SdifSelect.h"



/* Read frame headers until a frame matching the file selection has
   been found or the end of the file has been reached.  Return false
   if end of file was reached, true if data has been read. */
int SdifFReadNextSelectedFrameHeader (SdifFileT *f)
{
    size_t bytesread = 0, numread = 0;

    /* search for a frame we're interested in 
       TODO: heed max time */
    while (1)
    {
	if (SdifFCurrSignature(f) == eEmptySignature)
	    return 0;	/* EXIT RETURN EOF */

	/* TODO: 
	   first check: if frame sig not selected, don't read frame header
	   --> SdifFCurrSignatureIsSelected() */
	if (!(numread = SdifFReadFrameHeader(f)))
	    return 0;	/* EXIT RETURN ERROR */
	else
	    bytesread += numread;

	if (SdifFCurrFrameIsSelected (f))
	    break;	/* EXIT RETURN TRUE */

	bytesread += SdifFSkipFrameData (f);
	SdifFGetSignature(f, &numread);
	bytesread += numread;
    }

    return bytesread;
}
