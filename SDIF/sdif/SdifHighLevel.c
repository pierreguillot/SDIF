/* $Id: SdifHighLevel.c,v 3.2 2000-08-22 13:38:33 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 *
 * SdifHighLevel.c	8.12.1999	Diemo Schwarz
 *
 * $Log: not supported by cvs2svn $
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


