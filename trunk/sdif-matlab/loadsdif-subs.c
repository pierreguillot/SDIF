/* $Id: loadsdif-subs.c,v 1.1 2000-05-04 13:24:04 schwarz Exp $

   loadsdif_subs.c	25. January 2000	Diemo Schwarz

   Subroutines for loadsdif, function to load data from an SDIF file.

   All SDIF depencies are here!

   Usage from matlab (all output parameters are optional):

   [NVTs, ...] = begin (filename)
   [matrix, time, stream, frametype, matrixtype] = readframe
	Returns an empty matrix on end-of-file.
   endread ('close')
   $Log: not supported by cvs2svn $ 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sdif/sdif.h>
#include "loadsdif.h"


static size_t readmatrix (SdifFileT *input, mxArray *mxarray [MaxNumOut]);
static void   exitread (void);

static int    matricesread    = 0, /* counters for reporting, */
	      matricesskipped = 0; /* reset by beginread */
static int    eof = 1;		   /* reset by beginread */


SdifFileT *
beginread (int nlhs, mxArray *plhs [], char *filename, char *types)
{
    SdifFileT	   *input = NULL;
    SdifSelectionT *sel;
    char	   *fileonly;
    
    if (!gSdifInitialised)
    {
        SdifGenInit (types);
	SdifSetExitFunc (exitread);
    }

    if ((sel	  = SdifCreateSelection ())  &&
	(fileonly = SdifGetFilenameAndSelection (filename, sel))  &&
	(input    = SdifFOpen (fileonly, eReadFile))
       )
    {
	input->Selection = sel;

	if (!(SdifListIsEmpty(sel->row)  &&  SdifListIsEmpty(sel->column)))
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

    matricesread    = 0;
    matricesskipped = 0;
    return (input);
}


void 
endread (SdifFileT *input)
{
    if (!matricesread  &&  matricesskipped)
        mexWarnMsgTxt ("No Matrices selected!");

    SdifFreeSelection (input->Selection);
    SdifFClose (input);
}


/* [matrix, time, stream, frametype, matrixtype] = readframe */
int
readframe (int nlhs, mxArray *plhs [], SdifFileT *f)
{
    size_t	bytesread = 0;
    mxArray	*mxarray [MaxNumOut];
    int		m;

    if (eof)   return (0);

    bytesread += SdifFReadFrameHeader(f);

    /* search for a frame we're interested in 
       TODO: heed max time
     */
    while (!SdifFCurrFrameIsSelected (f, f->Selection))
    {
	size_t   numread;
	SdifSkipFrameData (f);
	if ((eof = SdifFGetSignature(f, &numread) == eEof))
	    return 0;
	bytesread += numread + SdifFReadFrameHeader(f);
    }

    /* go through matrices */
    for (m = 0; m < SdifFCurrNbMatrix (f); m++)
    {
	bytesread += SdifFReadMatrixHeader (f);
	if (SdifFCurrMatrixIsSelected (f, f->Selection))
	{
	    int mret  = readmatrix (f, mxarray);
	    if (mret == -1)   return (-1);
	    bytesread += mret;
	    matricesread++;	/* count for reporting */
	}
	else
	{
	    bytesread += SdifSkipMatrixData(f);
	    bytesread += SdifFReadPadding(f, SdifFPaddingCalculate(f->Stream, 
								   bytesread));
	    matricesskipped++;	/* count for reporting */
	}
    }

    /* read next signature */
    eof = SdifFGetSignature(f, &bytesread) == eEof;

    /* assign to output parameters */
    for (m = 0; m < nlhs; m++)
    {
	plhs [m] = mxarray [m];
    }

    return (1);
}


static size_t 
readmatrix (SdifFileT *f, mxArray *mxarray [MaxNumOut])
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
