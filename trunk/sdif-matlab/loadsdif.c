/* $Id: loadsdif.c,v 1.3 2001-05-29 10:02:19 roebel Exp $

   loadsdif.c	25. January 2000	Diemo Schwarz

   Function to load data from an SDIF file.
   No SDIF depencies here!  (-->loadsdif_subs.c)

   Usage (all output parameters are optional):

   begin:				  [NVTs, ...] = loadsdif (filename)
   read frame:  
	[matrix, time, stream, frametype, matrixtype] = loadsdif
	Returns an empty matrix on end-of-file.
   end:							loadsdif ('close')
  
   $Log: not supported by cvs2svn $
   Revision 1.2  2000/08/04 14:42:29  schwarz
   Added reset of file variable, prevents crash on double-close.
   Version number is written in NVTs, and is used for distribution,
   defined in makefile (major.minor.release).
   Types file now included in distribution and documentation.

 * Revision 1.1  2000/05/04  13:24:05  schwarz
 * Matlab mex extension and support functions to load SDIF files.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <mex.h>
#include "loadsdif.h"


#define Stringize(x) #x


static void *input = NULL;
static int  initatexit =0;

void cleanup(void) {
    if(input) 
      SdifFClose (input);

    input = 0;
    return;
}



void 
mexFunction (int nlhs, mxArray *plhs [], int nrhs, const mxArray *prhs [])
{
    char	 filename [PATH_MAX],
	         types    [PATH_MAX] = "";
    int		 i; 			      


    /* switch to appropriate subroutine according to number of input
       parameters */
    switch (nrhs)
    {
      case 0:
    	  /* check output parameters */
    	  if (nlhs < 1  ||  nlhs > MaxNumOut)
	      mexErrMsgTxt ("Wrong number of output arguments (at least 1, "
			    "at most " Stringize(MaxNumOut) ")");
	  if (!input)
	      mexErrMsgTxt ("No file opened.  Use 'loadsdif (filename)'.");

	  if (readframe (nlhs, plhs, input) != 1)
	  {   /* no more frames or error: return empty variables */
	      for (i = 0; i < nlhs; i++)
	      {
		  plhs [i] = mxCreateDoubleMatrix (0, 0, mxREAL);
	      }
	  }
      break;

      case 2:
	if (!mxIsChar(prhs[1]))
	  mexErrMsgTxt("Wrong type of input argument: want char");
	mxGetString (prhs [1], types, PATH_MAX);
	/* FALLTHROUGH */
      case 1:
	  if (!mxIsChar (prhs [0]))
	      mexErrMsgTxt ("Wrong type of input argument: want char");

	  mxGetString (prhs [0], filename, PATH_MAX);

	  if (strcmp (filename, "close") == 0)
	  {
	      if (!input)
		  mexErrMsgTxt ("No file opened.  Use 'loadsdif (filename)'.");

	      input = endread (input);
	  }
	  else
	  {
	      /* check output parameters */
	      if (nlhs > 2)
		  mexErrMsgTxt("Wrong number of output arguments (at most 2)");

	      if (!initatexit){
		mexAtExit(cleanup);
		initatexit =1;
	      }


	      if (input) {
		printf ("Closing previous input file.\n");
		input = endread (input);
	      }

	      if (!(input = beginread (nlhs, plhs, filename, types)))
		  mexErrMsgTxt ("Can't open SDIF input file");
	  }
      break;

      default:
	mexErrMsgTxt ("Wrong number of input arguments (want 0 or 1)");
    }
}
