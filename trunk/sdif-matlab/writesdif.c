/* $Id: writesdif.c,v 1.2 2000-05-15 13:07:47 tisseran Exp $

   writesdif.c       12. May 2000      Patrice Tisserand

   Function to write data in a SDIF file.
   No SDIF depencies here! (-->writesdif-subs.c)
   
   $Log: not supported by cvs2svn $
 * Revision 1.1  2000/05/12  16:14:13  tisseran
 * Mexfile to write sdif files in matlab.
 * TODO: add possibility to use several file at same time.
 *       add test on arguments
 *

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> /* For alpha bzero defintion */

#if defined(__i386)  /* For PATH_MAX definition */
#include <linux/limits.h> /* File is located in /usr/src/linux/include/linux/limits.h */
#else
#include <limits.h>
#endif

#include <errno.h>
#include <mex.h>
#include "writesdif.h"
#include "matrix.h"

#define Stringize(x) #x

void
mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  /* This static variable has to be here, since if they were
     file global (auto or static), they won't survive through the
     next invocations of mexFunction! */
  static void *input = NULL;
  char        filename [PATH_MAX],
              types    [PATH_MAX] = "";
  char        framesig [PATH_MAX],
              matrixsig[PATH_MAX];
  int         i, nbMatrixData, nbMatrixSignature;

  /* Switch to appropriate subroutine according to number of input
     parameters */
  switch (nrhs)
    {
    case 0:
      /* Check output parameters */
      mexErrMsgTxt("Wrong number of input argument");
      
      break;

    case 1:
      if (!mxIsChar(prhs[0]))
	mexErrMsgTxt("Wrong type of input argument: want char");
      
      mxGetString (prhs[0], filename, PATH_MAX);

      if (strcmp (filename, "close") == 0)
	{
	  if (!input)
	    mexErrMsgTxt("No file opened. Use 'writesdif (filename)'.");

	  endwrite(input);
	}
      else
	{
	  if (!(input = beginwrite(nrhs, prhs, filename, types)))
	    mexErrMsgTxt ("Can't open SDIF input file");
	}
      break;
      
    case 2:
      if ((!mxIsChar(prhs[0]))||(!mxIsChar(prhs[1])))
	mexErrMsgTxt("Wrong type of input argument: want char");

      mxGetString (prhs[0], filename, PATH_MAX);
      mxGetString (prhs[1], types, PATH_MAX);
      if (strcmp (filename, "close") == 0)
	{
	  if (!input)
	    mexErrMsgTxt("No file opened. Use 'writesdif (filename)'.");
	  
	  endwrite(input);
	}
      else
	{
	  if (!(input = beginwrite(nrhs, prhs, filename, types)))
	    mexErrMsgTxt ("Can't open SDIF input file");
	}
      break;

    case 3:
      if ((!mxIsChar(prhs[0]))||(!mxIsChar(prhs[1])))
	mexErrMsgTxt("Wrong type of input argument: want char");

      if (!mxIsNumeric(prhs[2]))
	mexErrMsgTxt("Wrong type of input argument: want int");
      
      mxGetString (prhs[0], filename, PATH_MAX);
      mxGetString (prhs[1], types, PATH_MAX);

      if (!(input = beginwrite(nrhs, prhs, filename, types)))
	mexErrMsgTxt("Can't open SDIF input file");

      break;
      
    default:
      if (nrhs > 3)
	{
	  nbMatrixData = (nrhs - 3); /* To know how many argument after Frame signature */
	  if (!((nbMatrixData % 2) == 0))
	    /* Not as many matrix signature as matrix data */
	    mexErrMsgTxt("Not as many matrix data as matrix signatures");

	  /* We affect nbMatrixSignature */
	  nbMatrixSignature = nbMatrixData % 2;
	  
	  /* Test on frame signature */
	  if (!mxIsChar(prhs[2]))
	    mexErrMsgTxt("Wrong argument type: want char for frame signature");
	  
	  for (i=0; i < nbMatrixSignature; i = i+2)
	    if (!mxIsChar(prhs[nrhs+3+i]))
	      mexErrMsgTxt("Wrong argument type: want char for matrix signature number"Stringize(i)" ");

	  /* We must test each pairs of matrix signature and data */
	  writeframe(nrhs, prhs, input);

	}
      else
	mexErrMsgTxt("Wrong number of input argument");
    }
  
}






