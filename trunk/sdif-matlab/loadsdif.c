/* $Id: loadsdif.c,v 1.1 2000-05-04 13:24:05 schwarz Exp $

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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <mex.h>
#include "loadsdif.h"


#define Stringize(x) #x



void 
mexFunction (int nlhs, mxArray *plhs [], int nrhs, const mxArray *prhs [])
{
    /* This static variable has to be here, since if they were
       file global (auto or static), they won't survive through the
       next invocations of mexFunction! */
    static void *input = NULL;
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

	      endread (input);
	  }
	  else
	  {
	      /* check output parameters */
	      if (nlhs > 2)
		  mexErrMsgTxt("Wrong number of output arguments (at most 2)");

	      if (!(input = beginread (nlhs, plhs, filename, types)))
		  mexErrMsgTxt ("Can't open SDIF input file");
	  }
      break;

      default:
	mexErrMsgTxt ("Wrong number of input arguments (want 0 or 1)");
    }
}
