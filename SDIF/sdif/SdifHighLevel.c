/* $Id: SdifHighLevel.c,v 3.3 2000-10-27 20:03:36 roebel Exp $
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
 *
 * SdifHighLevel.c	8.12.1999	Diemo Schwarz
 *
 * $Log: not supported by cvs2svn $
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
#include "SdifGlobals.h"
#include "SdifFileStruct.h"
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
	if (SdifFCurrSignature(f) == eEof)
	{
	    bytesread = 0;
	    break;	/* EXIT RETURN EOF */
	}

	/* TODO: 
	   first check: if frame sig not selected, don't read frame header */
	bytesread += SdifFReadFrameHeader(f) + numread;

	if (SdifFCurrFrameIsSelected (f))
	    break;	/* EXIT RETURN TRUE */

	bytesread += SdifFSkipFrameData (f);
	SdifFGetSignature(f, &numread);
    }

    return bytesread;
}


