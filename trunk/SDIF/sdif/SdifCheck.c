/* $Id: SdifCheck.c,v 3.2 2000-05-15 16:22:02 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifCheck.c		2. May 2000		Diemo Schwarz
 * 
 * File test functions.
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.1  2000/05/04  14:59:34  schwarz
 * Added modules SdifCheck and SdifSignatureTab, containing functions
 * formerly in SdifFile.
 *
 */

#include "SdifCheck.h"
#include "SdifFRead.h"
#include "SdifFGet.h"
#include "SdifFile.h"
#include "SdifSelect.h"


int
SdifCheckFileFormat (const char *name)
{
    int		ret = 0;
    size_t	size;
    SdifFileT	*file;

    SdifDisableErrorOutput ();
    if ((file = SdifFOpen (name, eReadFile)))
    {
        SdifFGetSignature (file, &size);
        ret = file->CurrSignature == eSDIF;
	SdifFClose (file);
    }
    SdifEnableErrorOutput ();
    return (ret);
}


SdifSignature
SdifCheckFileFramesTab (const char *name, const SdifSignatureTabT *frames)
{
    SdifSignature  ret = eEmptySignature;
    size_t	   bytesread;
    SdifFileT	   *in;

    if ((in = SdifFOpen (name, eReadFile)))
    {
	/* Read file header and first signature (ASCII chunks are
           treated as normal frames, here) */
        SdifFReadGeneralHeader  (in);
        SdifFGetSignature (in, &bytesread);
	ret = SdifCheckNextFrame (in, frames, NULL);
    }
    SdifFClose (in);
    return (ret);
}


int 
SdifCheckFileFramesIndex (const char *name, const SdifSignature *frames)
{
    int			ret = -1;
    size_t		bytesread;
    SdifFileT	       *in;
    SdifSignatureTabT  *sigs = SdifCreateSignatureTab (1);

    while (*frames)
	SdifAddToSignatureTab (sigs, *frames++);

    if ((in = SdifFOpen (name, eReadFile)))
    {
	/* Read file header and first signature (ASCII chunks are
           treated as normal frames, here) */
        SdifFReadGeneralHeader  (in);
        SdifFGetSignature (in, &bytesread);
	SdifCheckNextFrame (in, sigs, &ret);
    }
    SdifFClose (in);
    SdifKillSignatureTab (sigs);
    return (ret);
}


SdifSignature
SdifCheckNextFrame (/*in*/  SdifFileT *in, const SdifSignatureTabT *frames,
		    /*out*/ int *index)
{
    size_t	bytesread;
    int		ret = -1;

    /* Loop while not end-of-file (signature is 0) and nothing found yet */
    while (SdifFCurrSignature (in)  &&  ret < 0)
    {
	/* Read frame header.  Current signature has already been read */
	SdifFReadFrameHeader (in);
	
	/* Check frame time, type, and stream id */
	if (SdifFCurrFrameIsSelected (in))
	{   /* Test if frame header is in list */
	    ret = SdifFindInSignatureTab (frames, SdifFCurrSignature (in));
	}
	
	/* Skip frame and read next sig in any case */
	SdifSkipFrameData (in);
	SdifFGetSignature (in, &bytesread);
    }   /* end while frames */
    if (index)	*index = ret;
    return (SdifGetFromSignatureTab (frames, ret));
}
