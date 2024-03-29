/* $Id: writesdif.c,v 1.6 2008-09-18 13:18:29 diemo Exp $

   writesdif.c       12. May 2000      Patrice Tisserand

   Function to write data in a SDIF file.
   No SDIF depencies here! (-->writesdif-subs.c)
   
   $Log: not supported by cvs2svn $
   Revision 1.5  2003/04/22 16:08:56  roebel
   Added possibility for writing local type declaration to
   sdif file.

   Revision 1.4  2001/05/29 10:02:26  roebel
   Added cleanup rountine that close the sdif file in case of
   matlab clear function commands. Added automatic file closing in case
   of opening a new file while old sdif file is still open.
   moved static variables outside mex function to have access from
   cleanup function.

   Revision 1.3  2000/08/04 14:42:34  schwarz
   Added reset of file variable, prevents crash on double-close.
   Version number is written in NVTs, and is used for distribution,
   defined in makefile (major.minor.release).
   Types file now included in distribution and documentation.

 * Revision 1.2  2000/05/15  13:07:47  tisseran
 * Added test for input arguments:
 *    Is Matrix Signature a char?
 * Removed debugging message (mexPrintf("Matrix Loop") ...)
 * Removed unused variable
 * Change comment style for writesdif.m
 *
 * Revision 1.1  2000/05/12  16:14:13  tisseran
 * Mexfile to write sdif files in matlab.
 * TODO: add possibility to use several file at same time.
 *       add test on arguments
 *

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>	/* For alpha bzero defintion */
#include <limits.h>	/* For PATH_MAX definition */

#include <errno.h>
#include <mex.h>
#include "writesdif.h"
#include "matrix.h"

#define Stringize(x) #x


static void *input = NULL;
static int  initatexit =0;

void cleanup(void) {
    if(input) 
      SdifFClose (input);

    input = 0;
    return;
}

#define LOCALTYPEMAX 65536

void
mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  char        filename [PATH_MAX],
              types    [PATH_MAX] = "";
  char        framesig [PATH_MAX],
              matrixsig[PATH_MAX];
  char        localtypes [LOCALTYPEMAX] = "";

  int         i, nbMatrixData, nbMatrixSignature;

  /* Switch to appropriate subroutine according to number of input
     parameters */
  switch (nrhs)
    {
    case 0:
      /* Check output parameters */
      mexErrMsgTxt("Wrong number of input argument");
      
      break;

      
    case 2:
      if ((!mxIsChar(prhs[0]))||(!mxIsChar(prhs[1])))
	mexErrMsgTxt("Wrong type of input argument: want char");

      mxGetString (prhs[1], types, PATH_MAX);


    case 1:

      if (!mxIsChar(prhs[0]))
	mexErrMsgTxt("Wrong type of input argument: want char");

      mxGetString (prhs[0], filename, PATH_MAX);

      if (strcmp (filename, "close") == 0)
	{
	  if (!input)
	    mexErrMsgTxt("No file opened. Use 'writesdif (filename)'.");
	  
	  input = endwrite(input);
	}
      else
	{

	  if (!initatexit){
	    mexAtExit(cleanup);
	    initatexit =1;
	  }
	  if (input) {
	    printf ("Closing previous output file.\n");
	    input = endwrite (input);
	  }

	  if (!(input = beginwrite(nrhs, prhs, filename, types,localtypes)))
	    mexErrMsgTxt ("Can't open SDIF input file");
	}
      break;

    case 3:
      if ((!mxIsChar(prhs[0]))||(!mxIsChar(prhs[1])))
	mexErrMsgTxt("Wrong type of input argument: want char");

      if (mxIsChar(prhs[2]))
	mxGetString (prhs[2], localtypes, LOCALTYPEMAX);

      mxGetString (prhs[0], filename, PATH_MAX);
      mxGetString (prhs[1], types, PATH_MAX);

      if (!(input = beginwrite(nrhs, prhs, filename, types,localtypes)))
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






