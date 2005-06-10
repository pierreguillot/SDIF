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
    std::string stri, strin;



/**************************************/
/**************************************/

  int i;

  SdifUInt4    NbMatrix = 3;
  SdifUInt4    NumID = 0;
  SdifFloat8   Time = 0.0;

  SdifFloat4   TabValue[] = {1,2,3,4,5,6,7,8,9,10};
  SdifFloat8   TabValue2[] = {4,8,10,39, 72,99,101,201};
  SdifFloat4  *pTabValue;
  size_t       SizeFrameW;
  size_t       SizeMatrixW;

  char        *sentence;

/**************************************/
/**************************************/


    SdifGenInit("");

    SDIFEntity readentity;
    SDIFEntity entity;

/****************************************/


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

/***************************************/


    readentity.OpenRead(argv[1]);

    readentity.ViewString();
    

    //  entity.AddMtrxCols("windoxsid, wondowsSize, wind");

  
//    entity.AddFrameType("1POC { 1PIC PickedPeaks; IWoN SinusoidalTracks;  }\n");


    entity.AddMatrixType("APER", "windoxsid, wondowsSize, wind");
    entity.AddMatrixType("1TRC", "windoxsid, wondowsSize, wind");   
    entity.AddFrameType("1POC", "1PIC la_matrix_PIC; APER du_sonchat");


    //   entity.SetSdifString(readentity.GetSdifString());
entity.OpenWrite("testpr.sdif");


/*********************************************************************/


  
  SdifFSetCurrFrameHeader (entity.GetFile(), SdifSignatureConst('1','T','R','C'), _SdifUnknownSize, 1, NumID, Time);

  SizeFrameW = SdifFWriteFrameHeader (entity.GetFile());

  /****FIRST MATRIX***/

  SdifFSetCurrMatrixHeader (entity.GetFile(), SdifSignatureConst('1','T','R','C'), eFloat4, 1, 7);


  SizeMatrixW = SdifFWriteMatrixHeader (entity.GetFile());


  for (i=0;i<7;++i){
    SdifFSetCurrOneRowCol (entity.GetFile(), i+1, TabValue2[i]);
    fprintf(stderr,"Value=%f\n",TabValue2[i]);
  }


  SizeMatrixW += SdifFWriteOneRow (entity.GetFile());

  SizeMatrixW += SdifFWritePadding(entity.GetFile(),
                       SdifFPaddingCalculate(entity.GetFile()->Stream, SizeMatrixW))  ;

  SizeFrameW += SizeMatrixW;

  SizeFrameW -=  8;
  SdifUpdateChunkSize(entity.GetFile(), SizeFrameW);




/*********************************************************************/

    SdifGenKill();
    return 0;
}
   

