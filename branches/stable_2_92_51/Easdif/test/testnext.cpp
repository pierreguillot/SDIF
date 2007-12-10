#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sdif.h>
#include "sdifframe.h"

int main(int argc, char** argv)
{
    size_t  bytesread = 0;
    int     eof       = 0;  /* End-of-file flag */
    
    /*for reading*/
    size_t generalHeader;
    size_t asciiChunks;
    SdifFileT *file;

    /*for writing*/
    size_t generalHeaderw;
    size_t asciiChunksw;
    SdifFileT *filew;

    SdifSignature sign_file;
    
    SdifGenInit("");
    /*for the reading*/
    file = SdifFOpen (argv[1], eReadFile);
    /* BEGIN: SHOULD BE CHANGED */
    generalHeader = SdifFReadGeneralHeader(file);
    asciiChunks = SdifFReadAllASCIIChunks(file);
    /* END */

    /*for the writing*/
    filew = SdifFOpen ("ecr.sdif", eWriteFile);
    /* BEGIN: SHOULD BE CHANGED */
    generalHeaderw = SdifFWriteGeneralHeader(filew);
    asciiChunksw = SdifFWriteAllASCIIChunks(filew);


    /*signature of the file*/
    sign_file = SdifFCurrSignature (file);
    /* END */

    while (!eof  &&  SdifFLastError(file) == NULL)
    {
	SDIFFrame frame;
	SDIFFrame frametowrite;
	/*******  Reading the frame  *******/
	frame.Read(file);
	/* Print to stdout the content of the frame */
	frame.View();

	for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
	{
	    /*take the matrix number "i" and put it in tmpMatrix */
	    SDIFMatrix tmpMatrix = frame.GetMatrix(i);		
  
	    /*add a matrix in the matrix vector of the frame*/
	    frametowrite.AddMatrix(tmpMatrix);
	}
	frametowrite.SetStreamID(frame.GetStreamID());
	frametowrite.SetTime(frame.GetTime());
	frametowrite.SetSignature(frame.GetSignature());
	frametowrite.Write(filew);
	frametowrite.ClearData();
	/* read next signature */
	eof = SdifFGetSignature(file, &bytesread) == eEof;	
    }

    if (SdifFLastError(file))   /* Check for errors */
	exit (1);

    SdifFClose(file);
    SdifFClose(filew);
    
    SdifGenKill();
    return 0;
}
     



