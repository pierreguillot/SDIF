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
    //  size_t  bytesread = 0;
    //  int     eof       = 0;  /* End-of-file flag */
    std::string stri, strin;
    SdifFileT* file;
    SdifFileT* filew;
    SdifStringT* stringt;
    SdifSignature sig;

  /* Essai pour la definition d'un type de Frame */
  SdifFrameTypeT *myFrameType;
  SdifMatrixTypeT *myMatrixType;
  SdifMatrixTypeT *oldMatrixType;

  SdifFileT* MySdifFileToWrite;

  size_t SizeFrameW;

    SdifGenInit("");


    sig = SdifSignatureConst('E','L','O','R');


    //   SDIFEntity readentity;
    //  SDIFEntity entity;

    // stringt->str = "test";
    //  std::cout << stringt << std::endl;
    //   entity.SetString("1MTD");
    //  entity.ViewString();

//MySdifFileToWrite = SdifFOpen ("testyp.sdif", eWriteFile);


  /* Creation du nouveau type de matrice */
  myMatrixType = SdifCreateMatrixType(sig,NULL);

  /* Ajout des colonnes a la definition de la matrice */
  SdifMatrixTypeInsertTailColumnDef(myMatrixType,"Index");  
  SdifMatrixTypeInsertTailColumnDef(myMatrixType,"Frequency");  
  SdifMatrixTypeInsertTailColumnDef(myMatrixType,"Amplitude");  

  /* Ajout du nouveau type a la liste des matrices */
  SdifPutMatrixType(MySdifFileToWrite->MatrixTypesTable, myMatrixType);

  /* Creation du nouveau type de frame */
  myFrameType = SdifCreateFrameType(sig,NULL);

  SdifFrameTypePutComponent(myFrameType, sig,"LorisTracks");
  
  SdifPutFrameType(MySdifFileToWrite->FrameTypesTable, myFrameType);


  SizeFrameW= SdifFWriteGeneralHeader(MySdifFileToWrite);  
  SizeFrameW += SdifFWriteAllASCIIChunks(MySdifFileToWrite);


//  SdifFClose(MySdifFileToWrite);
  //   readentity.OpenRead(argv[1]);
    //   entity.OpenWrite("testyp.sdif");


  //   file = readentity.GetFile();







    SdifGenKill();
    return 0;
}
   
