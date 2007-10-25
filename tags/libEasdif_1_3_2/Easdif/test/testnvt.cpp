#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sdif.h>
#include "sdifframe.h"
#include "sdifentity.h"
//#include "sdifnamevaluetable.h"



int main(int argc, char** argv)
{  
    size_t  bytesread = 0;
    int     eof       = 0;  /* End-of-file flag */
    
    /*for reading*/
/*
    size_t generalHeader;
    size_t asciiChunks;

    size_t taille;

    SdifFileT *file;
    std::string m_nvt;
    char* signature;
*/
    /*for writing*/
/*  
    size_t generalHeaderw;
    size_t asciiChunksw;
    SdifFileT *filew;

    SdifNameValueT* NV;
    SdifNameValuesLT* NVlist;
    SdifNameValueTableT* NVtable;
    const char* value;
    const char* StreamID;
*/    
    std::string stri, strin;
    int z;
    SdifGenInit("");
    /*for the reading*/
    // file = SdifFOpen (argv[1], eReadFile);
    /* BEGIN: SHOULD BE CHANGED */
    //generalHeader = SdifFReadGeneralHeader(file);
    //asciiChunks = SdifFReadAllASCIIChunks(file);
    SDIFEntity readentity;
    readentity.OpenRead(argv[1]);

    z = SdifFNameValueNum(readentity.GetFile());
    std::cout << " nombre de NVT : " << z <<std::endl;

    std::cout << "before viewing NVTs" << std::endl;
    readentity.ViewNVTs();
    std::cout << "after viewing NVTs" << std::endl;
    // int z = SdifFNameValueNum(file);
    // std::cout << " nombre de NVT : " << z <<std::endl;
    /* END */

    /******************for the writing*/
    //  filew = SdifFOpen ("ecr.sdif", eWriteFile);
    /* BEGIN: SHOULD BE CHANGED */
    //  generalHeaderw = SdifFWriteGeneralHeader(filew);
    //   asciiChunksw = SdifFWriteAllASCIIChunks(filew);

    SDIFEntity entity;

    SDIFNameValueTable nm;
    nm.AddNameValue("hop", "la");
    nm.AddNameValue("dat","ta");
    nm.AddNameValue("na","bel");
    
    // entity.PutNVT(file);

    SDIFNameValueTable nma;
    nma.AddNameValue("hopa", "lfa");
    nma.AddNameValue("dgat","tsa");
    nma.AddNameValue("nga","bdel");

    SDIFNameValueTable nmo;
    nmo.AddNameValue("et", "hop!");
    nmo.AddNameValue("ddfgat","tsdfsdfsdfa");
    nmo.AddNameValue("vector","method");

    // entity.AddNVT(entity.GetNVT(),4);

    entity.AddNVT(nm, 3);
    entity.AddNVT(nma, 2);
    entity.AddNVT(nmo, 3);

    entity.OpenWrite("ecrentityt.sdif");

    entity.ViewNVTs();

    z = SdifFNameValueNum(entity.GetFile());
    std::cout << " nombre de NVT : " << z <<std::endl;
/*
    SDIFNameValueTable nm;
    nm.AddNameValue("hop", "la");
    nm.AddNameValue("dat","ta");
    nm.AddNameValue("na","bel");
    
    // entity.PutNVT(file);

    SDIFNameValueTable nma;
    nma.AddNameValue("hopa", "lfa");
    nma.AddNameValue("dgat","tsa");
    nma.AddNameValue("nga","bdel");

    SDIFNameValueTable nmo;
    nmo.AddNameValue("hopadf", "ldfdfsdfffa");
    nmo.AddNameValue("ddfgat","tsdfsdfsdfa");
    nmo.AddNameValue("ngdfa","bsdsdfsdfdel");

    // entity.AddNVT(entity.GetNVT(),4);

    entity.AddNVT(nm, 3);
    entity.AddNVT(nma, 2);
    entity.AddLastNVT(nmo, 3);
*/
    //  filew = entity.GetFile();

/*****************************************************************/
/*****************************************************************/




    while (!eof  &&  SdifFLastError(readentity.GetFile()) == NULL)
    {

	SDIFFrame frame;
	/*******  Reading the frame  *******/
	frame.Read(readentity);
	/* Print to stdout the content of the frame */
	frame.View();

	/*******  Creating a frame  *******/
	SDIFFrame frameToWrite;	   
	for (unsigned int i=0 ; i < frame.GetNbMatrix() ; i++)
	{
	    /*take the matrix number "i" and put it in tmpMatrix */
	    SDIFMatrix tmpMatrix = frame.GetMatrix(i);		


	    /*if we want to see the type of the matrix :*/
	    std::cout << "view the matrix type  before transformation" << std::endl;
	    std::cout << tmpMatrix.GetType() <<std::endl;

	    /*if we want to see the signature of the matrix :*/
	    std::cout << "view the matrix string signature  before transformation" << std::endl;
	    std::cout << tmpMatrix.GetStringSignature() <<std::endl;
;

	    /*if we want to see the matrix :*/
	    std::cout << "view the matrix before transformation" << std::endl;
	    tmpMatrix.View();
	       		  
	    /*add a matrix in the matrix vector of the frame*/
	    frameToWrite.AddMatrix(tmpMatrix);
	    /*how access to the data : an example, if we want to multiply with 2 the av last column of a matrix  matrix :*/
	    double dou;
	    int ncols = tmpMatrix.GetNbCols();
	    for(int i = 0 ; i < tmpMatrix.GetNbRows() ; i++)
	    {
		//dou = tmpMatrix.GetDouble(i, ncols-1);
//#else
		tmpMatrix.Get(i, ncols-2,dou);
//#endif
		tmpMatrix.Set(i, ncols-2,2.*dou);
	    }
	    std::cout << "view the matrix after transformation" << std::endl;
	    tmpMatrix.View();
	    /*end of example*/
	}
	/*set the header of the frame*/
	frameToWrite.SetStreamID(frame.GetStreamID());
	frameToWrite.SetTime(frame.GetTime());
	frameToWrite.SetSignature(frame.GetSignature());

	/*if we want to  set all the informations :
	  frameToWrite.SetInfo(Sig, StreamID, Time, NbMatrix);
	*/
	/*write the header and the data*/
	//frameToWrite.Write(filew);
frameToWrite.Write(entity);
	/* Print to stdout the content of the frame */
	frameToWrite.View();
	/*if we want to clear the data of the frame :*/
	frameToWrite.ClearData();
	/* read next signature */
	eof = SdifFGetSignature(readentity.GetFile(), &bytesread) == eEof;	
    }

    if (SdifFLastError(readentity.GetFile()))   /* Check for errors */
	exit (1);





/*****************************************************************/
/*****************************************************************/

    //entity.SetFile(filew);
    // SdifFClose(filew);
    // entity.Close();
//    SdifFClose(file);
     //    readentity.Close();  
    SdifGenKill();
    return 0;
}
   
