/* $Id: loadsdif-subs.c,v 1.6 2000-08-08 17:26:58 schwarz Exp $

   loadsdif_subs.c	25. January 2000	Diemo Schwarz

   Subroutines for loadsdif, function to load data from an SDIF file.

   All SDIF depencies are here!

   Usage from matlab (all output parameters are optional):

   [NVTs, ...] = begin (filename)
   [matrix, time, stream, frametype, matrixtype] = readframe
	Returns an empty matrix on end-of-file.
   endread ('close')

  $Log: not supported by cvs2svn $
  Revision 1.5  2000/08/04  14:42:28  schwarz
  Added reset of file variable, prevents crash on double-close.
  Version number is written in NVTs, and is used for distribution,
  defined in makefile (major.minor.release).
  Types file now included in distribution and documentation.

  Revision 1.4  2000/07/19  16:32:08  schwarz
  Updated to new SDIF selection API.
  Proper handling of matrix selection now.

  Revision 1.3  2000/05/12  14:03:53  schwarz
  Oops-style errors.

  Revision 1.2  2000/05/11  12:39:24  schwarz
  With new SDIF lib, selection is automatically parsed on open, and
  stored in input->Selection.

 * Revision 1.1  2000/05/04  13:24:04  schwarz
 * Matlab mex extension and support functions to load SDIF files.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sdif/sdif.h>
#include "loadsdif.h"


static size_t readmatrix (SdifFileT *input, mxArray *mxarray [MaxNumOut]);
static void   exitread (void);

static int    matricesleft    = 0, /* determines action in readframe */
	      matricesread    = 0, /* counters for reporting, */
	      matricesskipped = 0; /* reset by beginread */
static int    eof = 1;		   /* reset by beginread */


SdifFileT *beginread (int nlhs, mxArray *plhs [], char *filename, char *types)
{
    SdifFileT	   *input = NULL;
    
    if (!gSdifInitialised)
    {
        SdifGenInit (types);
	SdifSetExitFunc (exitread);
    }

    if ((input    = SdifFOpen (filename, eReadFile)))
    {
	if (!(SdifListIsEmpty(input->Selection->row)  &&  
	      SdifListIsEmpty(input->Selection->column)))
	    mexErrMsgTxt ("Can't handle row or column selection yet.");
    
	SdifFReadGeneralHeader  (input);
	SdifFReadAllASCIIChunks (input);
	eof = SdifFCurrSignature(input) == eEmptySignature;
	  
	/* return some header data */
	if (nlhs > 0)
	{
	    plhs [0] = mxCreateString ("Not yet!");
	    /*plhs [0] = mxCreateCellArray (1, &1);*/
	}
    }

    matricesleft    = 0;
    matricesread    = 0;
    matricesskipped = 0;
    return (input);
}


SdifFileT *endread (SdifFileT *input)
{
    if (!matricesread  &&  matricesskipped)
        mexWarnMsgTxt ("No Matrices selected!");
    SdifFClose (input);
    return NULL;
}


/* [matrix, time, stream, frametype, matrixtype] = readframe */
int readframe (int nlhs, mxArray *plhs [], SdifFileT *f)
{
    static size_t bytesread = 0;
    mxArray	  *mxarray [MaxNumOut];
    int		  matrixfound = 0, m;

    if (eof  ||  SdifFLastError(f))   return (0);	/* error or eof */

    while (!matrixfound  &&  !eof)
    {
	/* are there any matrices left in the frame to read? */
	if (matricesleft == 0)
	{   /* no: read next frame header */
	    bytesread = SdifFReadFrameHeader(f);

	    /* search for a frame we're interested in 
	       TODO: heed max time */
	    while (!SdifFCurrFrameIsSelected (f))
	    {
		size_t   numread;
		SdifSkipFrameData (f);
		if ((eof = SdifFGetSignature(f, &numread) == eEof))
		    return 0;
		bytesread += numread + SdifFReadFrameHeader(f);
	    }
	    
	    /* re-initialise matrices left to read */
	    matricesleft = SdifFCurrNbMatrix (f);
	}
	
	/* re-test number of matrices left to read because we might
           have read a new frame header */
	if (matricesleft > 0)
	{   /* go through one matrix */
	    bytesread += SdifFReadMatrixHeader (f);
	    if (SdifFCurrMatrixIsSelected (f))
	    {
		int mret     = readmatrix (f, mxarray);
		if (mret    == -1)   return (-1);
		bytesread   += mret;
		matricesread++;		/* count for reporting */
		matrixfound  = 1;	/* set success flag -> exit loop */
	    }
	    else
	    {
		bytesread   += SdifSkipMatrixData(f);
		matricesskipped++;	/* count for reporting */
	    }
	    matricesleft--;
	}

	if (matricesleft == 0)
	{   /* no matrices left in frame to read, read next frame signature */
	    eof = SdifFGetSignature(f, &bytesread) == eEof;
	}
    }

    /* assign to output parameters */
    for (m = 0; m < nlhs; m++)
    {
	plhs [m] = mxarray [m];
    }

    /* return true even on eof: we want to return the last matrix read */
    return (SdifFLastError (f) == NULL);
}


static size_t readmatrix (SdifFileT *f, mxArray *mxarray [MaxNumOut])
{
    size_t	 bytesread = 0;
    double	*prmtx;
    int		 row, col,
		 ncol = SdifFCurrNbCol(f),
		 nrow = SdifFCurrNbRow(f);

    /* alloc output array and scalars */
    mxarray [0] = mxCreateDoubleMatrix (nrow, ncol, mxREAL);
    mxarray [1] = mxCreateDoubleMatrix (1, 1, mxREAL);
    mxarray [2] = mxCreateDoubleMatrix (1, 1, mxREAL);
    mxarray [3] = mxCreateString (
			SdifSignatureToString (SdifFCurrFrameSignature(f)));
    mxarray [4] = mxCreateString (
			SdifSignatureToString (SdifFCurrMatrixSignature(f)));
    
    prmtx		     = mxGetPr (mxarray [0]);
    *(mxGetPr (mxarray [1])) = SdifFCurrTime (f);
    *(mxGetPr (mxarray [2])) = SdifFCurrID (f);
    
    for (row = 0; row < nrow; row++)
    {
	bytesread += SdifFReadOneRow (f);
	for (col = 0; col < ncol; col++)
	    /* transpose to matlab column-major order */
	    prmtx [row + col * nrow] = SdifFCurrOneRowCol(f, col+1);
    }
    
    bytesread += SdifFReadPadding (f, SdifFPaddingCalculate (f->Stream, 
							     bytesread));
    return (bytesread);
}


static void exitread (void)
{
    mexErrMsgTxt ("SDIF error: exiting loadsdif!");
}
