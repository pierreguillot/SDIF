/* $Id: tutorial.c,v 1.1 2008-09-10 16:14:50 diemo Exp $
 * Code copied from tutorial-doc.html and compiled to test for errors.
 */

#include <sdif.h>

int main()
{
    SdifFileT *file;
    SdifSignature mysig  = SdifSignatureConst ('1FQ0');
    char*         sigstr = SdifSignatureToString (mysig);
    
    int mystream = 1;
    int mytime = 99.;
    int nrows = 2, ncols = 1, nmatrix = 1;
    int row, col;
    float value = 66, data[] = {3, 5};
    
    SdifUInt4 framesize;
    SdifUInt4 SizeFrameW;

    size_t  bytesread = 0;
    int     eof       = 0;  /* End-of-file flag */


    SdifGenInit ("");
   
    
    file = SdifFOpen("filename.sdif", eWriteFile);  /* Open file for writing */
    /* Here, you could add some text data to the name-value tables */
    SdifFWriteGeneralHeader  (file);    /* Write file header information */
    SdifFWriteAllASCIIChunks (file);    /* Write ASCII header information */
    
	    
     /* Writing can be done in different flavours: For the common case
	of a frame with one matrix, use: */
    SdifFWriteFrameAndOneMatrix (file, mysig, mystream, mytime,             /* frame header */
				       mysig, eFloat4, nrows, ncols, data); /* matrix */
    
	    
     /* If you have one or more matrices ready in row-major order, use: */
    framesize = SdifSizeOfFrameHeader () 
	      + SdifSizeOfMatrix (eFloat4, nrows, ncols); /* as many as you like... */
    SdifFSetCurrFrameHeader (file, mysig, framesize, 1, mystream, mytime);
    SdifFWriteFrameHeader (file);
    SdifFWriteMatrix (file, mysig, eFloat4, nrows, ncols, data);
    
	    
     /* If you want to calculate your data on-the-fly, you can write
        it the nitty-gritty way: */
    SizeFrameW = 0;
    
    SdifFSetCurrFrameHeader (file, mysig, _SdifUnknownSize, nmatrix, mystream, mytime);
    SizeFrameW += SdifFWriteFrameHeader (file);
    
    /* Write matrix header */
    SdifFSetCurrMatrixHeader (file, mysig, eFloat4, nrows, ncols);
    SizeFrameW += SdifFWriteMatrixHeader (file);
    
    /* Write matrix data */
    for (row = 0; row < nrows; row++)
    {
	for (col = 1; col <= ncols; col++)
	{
	    SdifFSetCurrOneRowCol (file, col, value);
	    SizeFrameW += SdifFWriteOneRow (file);
	}
    }
    /* Write matrix padding */
    SizeFrameW += SdifFWritePadding (file, SdifFPaddingCalculate (file->Stream, SizeFrameW));
    
    SdifUpdateChunkSize (file, SizeFrameW - 8);  /* Set frame size */

    SdifFClose (file);




    /* Open file for reading and parse selection. (See Utilities for
       selection syntax.) */
    file = SdifFOpen ("filename.sdif", eReadFile);
    SdifFReadGeneralHeader  (file); /* Read file header */
    SdifFReadAllASCIIChunks (file); /* Read ASCII header info, such as name-value tables */
    
	    
    /* Read all frames matching the file selection. */
    while (!eof  &&  SdifFLastError (file) == NULL)
    {
	bytesread += SdifFReadFrameHeader(file);
    
	/* search for a frame we're interested in */
	while (!SdifFCurrFrameIsSelected (file)
	       ||  SdifFCurrSignature (file) != mysig)
	{
	    SdifFSkipFrameData (file);
	    if ((eof = SdifFGetSignature(file, &bytesread) == eEof))
		break;                                         /* eof */
	    bytesread += SdifFReadFrameHeader(file);
	}
    
	if (!eof)
	{    /* Access frame header information */
	    SdifFloat8      time     = SdifFCurrTime (file);
	    SdifSignature   sig      = SdifFCurrFrameSignature (file);
	    SdifUInt4       streamid = SdifFCurrID (file);
	    SdifUInt4       nmatrix  = SdifFCurrNbMatrix (file);
	    int             m;
	
	    /* Read all matrices in this frame matching the selection. */
	    for (m = 0; m < nmatrix; m++)
	    {
		bytesread += SdifFReadMatrixHeader (file);
	
		if (SdifFCurrMatrixIsSelected (file))
		{    /* Access matrix header information */
		    SdifSignature   sig   = SdifFCurrMatrixSignature (file);
		    SdifInt4        nrows = SdifFCurrNbRow (file);
		    SdifInt4        ncols = SdifFCurrNbCol (file);
		    SdifDataTypeET  type  = SdifFCurrDataType (file);
	
		    int             row, col;
		    SdifFloat8      value;
		    
		    for (row = 0; row < nrows; row++)
		    {
			bytesread += SdifFReadOneRow (file);
	
			for (col = 1; col <= ncols; col++)
			{
			    /* Access value by value... */
			    value = SdifFCurrOneRowCol (file, col);
	
			    /*  Do something with the data... */
			    printf ("time %g stream %d row %d col %d value %g\n",
				    time, streamid, row, col, value);
			}
		    }
		}
		else
		{
		    bytesread += SdifFSkipMatrixData(file);
		}
		
		bytesread += SdifFReadPadding(file, SdifFPaddingCalculate(file->Stream, bytesread));
	    } 
	
	    /* read next signature */
	    eof = SdifFGetSignature(file, &bytesread) == eEof;
	}
    }
    
    if (SdifFLastError (file))   /* Check for errors */
    {
	exit (1);
    }
    
    SdifFClose (file);    
    
        
    SdifGenKill ();
}
