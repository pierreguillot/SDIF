/* $Id: issdiffile.c,v 1.1 2004-07-20 14:48:19 roebel Exp $

   test whether given file is an sdif file
  
   $Log: not supported by cvs2svn $
 * 
 */

#include <limits.h>
#include "sdif.h"
#include <mex.h>


#define Stringize(x) #x

static void exitSDIF (void)
{
    mexErrMsgTxt ("SDIF error: exiting !");
}


void 
mexFunction (int nlhs, mxArray *plhs [], int nrhs, const mxArray *prhs [])
{
  char	 filename [PATH_MAX];
  mxArray	  *mxarray;

  if (!gSdifInitialised)
    {
      SdifGenInit (0);
      SdifSetExitFunc (exitSDIF);
    }

  switch (nrhs)
    {
    default:
      /* check output parameters */
      mexErrMsgTxt ("Wrong number of input arguments: supply name of file to test for sdif");
      break;
      
    case 1:
      if (!mxIsChar (prhs [0]))
	mexErrMsgTxt ("Wrong type of input argument: want char");

      mxGetString (prhs [0], filename, PATH_MAX);
      if(nlhs>0) {
	plhs[0] = mxCreateDoubleMatrix (1, 1, mxREAL);
	if(SdifCheckFileFormat(filename))
	  *mxGetPr(plhs[0]) = 1.;	
	else
	  *mxGetPr(plhs[0]) = 0.;
      }
      break;
    }

  return;
}

