/* $Id: loadsdif-subs.c,v 1.10 2001-05-29 09:55:21 roebel Exp $

   loadsdif_subs.c	25. January 2000	Diemo Schwarz

   Subroutines for loadsdif, function to load data from an SDIF file.

   All SDIF depencies are here!

   Usage from matlab (all output parameters are optional):

   [NVTs, ...] = begin (filename)
   [matrix, time, stream, frametype, matrixtype] = readframe
	Returns an empty matrix on end-of-file.
   endread ('close')

  $Log: not supported by cvs2svn $
  Revision 1.9  2001/05/28 16:32:32  roebel
  Added support for reading char data and 1NVT frames
  and matrices. The initial loadsdif call now returns
  the ASCII chunks of type 1NVT and 1IDS that make up
  the header of the SDIF file.

  Revision 1.8  2001/04/19 18:28:29  roebel
  Changed error handling in readframe to be consistent with the
  behavior of sdifextract

  Revision 1.7  2000/12/19 16:44:10  roebel
  Fixed Bug in loadsdif - crash when last matrix was not selected
  Moved test file sequence4seg1.energy.sdif into repository
  Corrected sgi mex extension to mexsg such that sgi is now in
  distribution

  Revision 1.6  2000/08/08 17:26:58  schwarz
  Fixed 'last matrix not returned' bug.
  More test cases t0, t1.

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
	  SdifNameValueTableT *tt;
	  int count=0,numstreamid;
	  SdifStringT *string;
	  char *localstr;
	  SdifHashNT     *pNV;
	  char idnumasstring[30];

	  string = SdifStringNew();

	  if(SdifNameValuesLIsNotEmpty(&(input->NameValues[count]))){
	    SdifHashTableT *HTable;
	    SdifListInitLoop(input->NameValues[count].NVTList);

	    while(  SdifListIsNext(input->NameValues[count].NVTList)){
	      input->NameValues[count].CurrNVT = 
		SdifListGetNext (input->NameValues[count].NVTList);
	      SdifFNameValueLCurrNVTtoSdifString(
			      input, string);	    
	    }
	    count ++;

	  }

	  if((numstreamid=SdifStreamIDTableGetNbData  (input->StreamIDsTable))
	     > 0){
	    int iID;
	    SdifHashNT* pID;
	    SdifStreamIDT *sd;

	    for(iID=0; iID<input->StreamIDsTable->SIDHT->HashSize; iID++)
	      for (pID = input->StreamIDsTable->SIDHT->Table[iID]; pID; pID = pID->Next) {

		SdifStringAppend(string,"IDS ");
		sd = ((SdifStreamIDT * )(pID->Data));
		sprintf(idnumasstring,"%d ",sd->NumID);
		SdifStringAppend(string,idnumasstring);
		SdifStringAppend(string,sd->Source);
		SdifStringAppend(string,":");
		SdifStringAppend(string,sd->TreeWay);
		SdifStringAppend(string,"\n");
		
	      }
	  }
	  localstr = calloc(string->SizeW+1,1);
	  strcpy(localstr,string->str);
	  plhs[0] = mxCreateString(localstr);
	  SdifStringFree(string);
	  free(localstr);
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

    /* Changed to be
       consistent with the behavior of sdifextract

       if (eof  ||  SdifFLastError(f))   return (0);	*/

    if (eof)   return (0);	/*  eof */

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
		SdifFSkipFrameData (f);
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
		bytesread   += SdifFSkipMatrixData(f);
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
    if(matrixfound) {
      for (m = 0; m < nlhs; m++)	{
	plhs [m] = mxarray [m];
      }
    }
    else{
      for (m = 0; m < nlhs; m++)	{
	  plhs [m] = mxCreateDoubleMatrix (0, 0, mxREAL);
      }
    }
    /* return true even on eof: we want to return the last matrix read */

    /* Changed to be
       consistent with the behavior of sdifextract
       return (SdifFLastError (f) == NULL); */

    return (1);
}


static size_t readmatrix (SdifFileT *f, mxArray *mxarray [MaxNumOut])
{
    size_t	 bytesread = 0;
    double	*prmtx;
    int		 row, col,
		 ncol = SdifFCurrNbCol(f),
		 nrow = SdifFCurrNbRow(f);

    SdifDataTypeET DataType = SdifFCurrDataType(f);

    switch (DataType){
    case eText:{
      char *str = calloc(nrow*ncol+1,1);
      double matData;

      mxarray [1] = mxCreateDoubleMatrix (1, 1, mxREAL);
      *(mxGetPr (mxarray [1])) = SdifFCurrTime (f);
      mxarray [2] = mxCreateDoubleMatrix (1, 1, mxREAL);
      *(mxGetPr (mxarray [2])) = SdifFCurrID (f);
      mxarray [3] = mxCreateString(
				   SdifSignatureToString (SdifFCurrFrameSignature(f)));
      mxarray [4] = mxCreateString(
				   SdifSignatureToString (SdifFCurrMatrixSignature(f)));
    
      for (row = 0; row < nrow; row++)
	{
	  bytesread += SdifFReadOneRow (f);
	  for (col = 0; col < ncol; col++){
	    /* transpose to matlab column-major order */
	     matData = SdifFCurrOneRowCol(f, col+1);
	     str [row + col * nrow] = (char) matData;
	  }
	}
      
      bytesread += SdifFReadPadding (f, SdifFPaddingCalculate (f->Stream, 
							     bytesread));

      mxarray [0] = mxCreateString(str);
      free(str);
    }
    break;

    case eFloat4:
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
      break;

    default :
      printf("Unknown type of matrix data skipped !!\n");
      mxarray [0] = mxCreateDoubleMatrix (0, 0, mxREAL);
      mxarray [1] = mxCreateDoubleMatrix (1, 1, mxREAL);
      *(mxGetPr (mxarray [1])) = SdifFCurrTime (f);
      mxarray [2] = mxCreateDoubleMatrix (1, 1, mxREAL);
      *(mxGetPr (mxarray [2])) = SdifFCurrID (f);
      mxarray [3] = mxCreateString (
				    SdifSignatureToString (SdifFCurrFrameSignature(f)));
      mxarray [4] = mxCreateString (
				    SdifSignatureToString (SdifFCurrMatrixSignature(f)));
      bytesread   += SdifFSkipMatrixData(f);

      break;
    }
    return (bytesread);
}


static void exitread (void)
{
    mexErrMsgTxt ("SDIF error: exiting loadsdif!");
}
