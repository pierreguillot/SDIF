/* $Id: querysdif.c,v 1.2 2000-11-15 14:53:40 lefevre Exp $
 
                Copyright (c) 1998 by IRCAM - Centre Pompidou
                           All rights reserved.
 
   For any information regarding this and other IRCAM software, please
   send email to:
                             manager@ircam.fr
 

   querysdif.c		10.12.1998	Diemo Schwarz
   
   View summary of data in an SDIF-file.  
   
   $Log: not supported by cvs2svn $
 * Revision 1.1  2000/10/30  14:44:03  roebel
 * Moved all tool sources into central tools directory and added config.h to sources
 *
 * Revision 1.2  2000/10/27  20:04:18  roebel
 * autoconf merged back to main trunk
 *
 * Revision 1.1.2.3  2000/08/21  18:48:49  tisseran
 * Use SdifFSkipFrameData
 *
 * Revision 1.1.2.2  2000/08/21  16:42:12  tisseran
 * *** empty log message ***
 *
 * Revision 1.1.2.1  2000/08/21  13:42:24  tisseran
 * *** empty log message ***
 *
   Revision 3.2  1999/06/18  16:20:28  schwarz
   In SigEqual: SdifSignatureCmpNoVersion (s, sigs [i].sig) dropped LAST byte
   on alpha (this is fixed now), and anyway, we want to compare the whole sig.

   Revision 3.1  1999/03/14  10:56:24  virolle
   SdifStdErr add

   Revision 1.3  1999/02/28  12:16:31  virolle
   memory report

   Revision 1.2  1998/12/21  18:26:54  schwarz
   Inserted copyright message.

   Revision 1.1  1998/12/10  18:55:40  schwarz
   Added utility program querysdif to view the summary of data in
   an SDIF-file.

*/


#include <preincluded.h>
#include "XpGuiCalls.h"

#include "SdifGlobals.h"
#include "SdifFile.h"
#include "SdifFRead.h"
#include "SdifFPrint.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

void usage (void)
{
    fprintf (SdifStdErr, "\nquerysdif, %s\n\n", VERSION);
    SdifPrintVersion();
    fprintf (SdifStdErr, "\n"
"Usage: querysdif [options] [sdif-file]\n"
"\n"
"Options:\n"
"	-a	view ASCII chunks\n"
"	-d	view data\n"
/* todo:
"	-n	view NVTs (name value tables)\n"
"	-T	view type declarations in sdif-file\n"
"	-D	view all type declarations in effect\n"
"	-s	view stream id table\n"
*/
"	-t <sdif types file>  specify file with additional sdif types\n"
"	-h	this help\n"
"\n"
"View summary of data in an SDIF-file.  Per default, all ASCII chunks are\n"
"printed, followed by a count of the frames and the matrices occuring in\n"
"the file.\n"
"\n");
    XpExit(1);
}

/* Count occurence of signatures as frame or matrix under
   different parent frames. */
#define	MaxSignatures	1024
int	nsig	  = 0;

struct TwoLevelTree 
{
	SdifSignature sig;
	int	      parent;
	int	      stream;
	int	      count;
	float	      tmin, tmax;
	/* todo: more fields for statistics */
}	sigs [MaxSignatures];


int SigEqual (SdifSignature s, int parent, int stream, int i)
{
    return sigs [i].sig    == s
       &&  sigs [i].stream == stream
       &&  sigs [i].parent == parent;
}

int GetSigIndex (SdifSignature s, int parent, int stream)
{
    int i = 0;
    
    while (i < nsig  &&  !SigEqual (s, parent, stream, i))
	i++;

    if (i == nsig)
    {   /* add new signature */
	if (nsig >= MaxSignatures)
	{
	    fprintf (SdifStdErr, "Too many different signatures, "
		     "can't handle more than %d!\n", MaxSignatures);
	    XpExit (1);
	}

	sigs [i].sig    = s;
	sigs [i].parent = parent;
	sigs [i].stream = stream;
	sigs [i].count  = 0;
	sigs [i].tmin   = FLT_MAX;
	sigs [i].tmax   = FLT_MIN;
	nsig++;
    }

    return (i);
}

int CountSig (SdifSignature s, int parent, int stream, float time)
{
    int i = GetSigIndex (s, parent, stream);

    sigs [i].count++;
    if (time < sigs [i].tmin)   sigs [i].tmin = time;
    if (time > sigs [i].tmax)   sigs [i].tmax = time;
    /* todo: statistics */

    return (i);
}


/*--------------------------------------------------------------------------*/
/*	KERmain / main															*/
/*--------------------------------------------------------------------------*/

#if HOST_OS_MAC

int KERmain(int argc, char** argv);
int KERmain(int argc, char** argv)

#else

int main(int argc, char** argv);
int main(int argc, char** argv)

#endif
{
    int		i, m, eof = 0;
    size_t	bytesread = 0;
    SdifFileT	*in;
    int         result;

    /* arguments with default values */
    char	*infile   = NULL, 
		*types	  = NULL;
    int		vall	  = 1,
 		vascii	  = 0,
		vdata	  = 0,
		vnvt	  = 0,
		vtypes	  = 0,
		valltypes = 0,
		vstream	  = 0;


    SdifStdErr = stderr;
    for (i = 1; i < argc; i++)
    {
	if (argv [i][0] == '-')
	{
	    char *arg = argv [i] + 1;

	    while (*arg)
		switch (*arg++)
		{
		  case 'a': vall = 0;  vascii    = 1;  break;
		  case 'd': vall = 0;  vdata     = 1;  break;
		  case 'n': vall = 0;  vnvt      = 1;  break;
		  case 'T': vall = 0;  vtypes    = 1;  break;
		  case 'D': vall = 0;  valltypes = 1;  break;
		  case 's': vall = 0;  vstream   = 1;  break;
		  case 't': /* no arg after last option, complain */
			    if (i == argc - 1)   
				usage ();
			    types = argv [++i];	       break;
		  default :  usage();		       break;
		}
	}
	else if (!infile)
	    infile = argv [i];
	else
	    usage();
    }


    /* do inits, open files */
    SdifGenInit (types  ?  types  :  "");

    if (!infile)   
    	infile  = "stdin";

    if (!(in = SdifFOpen (infile, eReadFile)))
    {
	fprintf (SdifStdErr, "Can't open input file %s.\n", infile);
        SdifGenKill ();
        XpExit (1);
    }
    in->TextStream = stdout;	/* SdifFPrint* functions need this */

    bytesread += SdifFReadGeneralHeader  (in);
    bytesread += SdifFReadAllASCIIChunks (in);
    eof = SdifFCurrSignature(in) == eEmptySignature;

    if (vall || vascii)
    {
	printf ("Ascii chunks of file %s:\n\n", infile);
	SdifFPrintGeneralHeader(in);
	SdifFPrintAllASCIIChunks(in);
    }

    if (vall || vdata)
    {
	/* read, count frame loop */
	while (!eof)
	{
	    int frameidx;

	    /* Read frame header.  Current signature has already been read
	       by SdifFReadAllASCIIChunks or the last loop.) */
	    bytesread += SdifFReadFrameHeader (in);

	    /* count frame */
	    frameidx = CountSig (SdifFCurrSignature(in), -1, SdifFCurrID(in),
				 SdifFCurrTime(in));

	    /* for matrices loop */
	    for (m = 0; m < SdifFCurrNbMatrix (in); m++)
	    {
		int nbrows, nbcols;

		/* Read matrix header and count matrix */
		bytesread += SdifFReadMatrixHeader (in);
		CountSig (SdifFCurrMatrixSignature (in), frameidx, -1, 0);

		/* Todo: statistics about rows/columns */
		nbrows = SdifFCurrNbRow (in);
		nbcols = SdifFCurrNbCol (in);

		/* We're not actually interested in the matrix data, 
		   so we skip it.  */
		bytesread += SdifFSkipMatrixData (in);
		bytesread += SdifFReadPadding(in, 
				 SdifFPaddingCalculate(in->Stream, bytesread));
	    }   /* end for matrices */

	    eof = SdifFGetSignature (in, &bytesread) == eEof;
	}   /* end while frames */ 

	/* print results */
	printf ("Data in file %s (%d bytes):\n", infile, bytesread);
	/* useless 
	   printf ("Number of different signatures/streams: %d\n\n", nsig); */

	for (i = 0; i < nsig; i++)
	{
	    if (sigs [i].parent == -1)
	    {
		printf ("%5d %s frames in stream %d between time %f and %f containing\n", 
			sigs [i].count, 
			SdifSignatureToString (sigs [i].sig),
			sigs [i].stream,
			sigs [i].tmin,
			sigs [i].tmax);

		for (m = 0; m < nsig; m++)
		{
		    if (sigs [m].parent == i)
			printf ("  %5d %s matrices\n",
				sigs [m].count, 
				SdifSignatureToString (sigs [m].sig));
		    /* todo: rows/columns statistics */
		}
	    }
	}
    }
    printf ("\n");

    /* check for error */
    if (SdifFLastError(in) == NULL)
	result = 0;
    else
	result = 1;

    /* cleanup */
    SdifFClose (in);
    SdifGenKill ();


    return result;

}    
