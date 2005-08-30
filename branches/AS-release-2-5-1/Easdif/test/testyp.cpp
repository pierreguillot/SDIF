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
    int z;
    SdifGenInit("");

    SDIFEntity readentity;
    SDIFEntity entity;
    //   SdifMatrixTypeT* matrixtype;
  SdifFrameTypeT* myFrameType;
  SdifMatrixTypeT* myMatrixType;


    //  SdifHashTableT* MatrixTypesTable;
    //SdifSignature Signature;

    readentity.OpenRead(argv[1]);
    entity.OpenWrite("testyp.sdif");


    file = readentity.GetFile();
    //  filew = entity.GetFile();
/*
  std::cout << "av"<< std::endl;
  std::cout << file->TextStreamName << std::endl;
  std::cout << "apres" << std::endl;
  std::cout << file->MatrixTypesTable<< std::endl;   

  std::cout <<file->FrameTypesTable << std::endl;

  std::cout <<file->TextStream<< std::endl;

  std::cout <<file->TextStreamName<< std::endl;
  entity.OpenWrite("testyp.sdif");
*/
    std::cout <<SdifSignatureToString(file->CurrFramH->Signature)<< std::endl;
    //  std::cout <<SdifSignatureToString(file->CurrMtrxT->Signature) << std::endl;

    // MatrixTypesTable = file->MatrixTypesTable;
    //  Signature = file->CurrMtrxT->Signature;
//matrixtype =  SdifGetMatrixType (MatrixTypesTable,Signature);



 // std::cout <<file->CurrMtrxT->ColumnUserList << std::endl;
  std::cout <<file->CurrMtrxT->NbColumnDef << std::endl;
  std::cout <<file->CurrMtrxT->ModifMode  << std::endl;

  myMatrixType = SdifCreateMatrixType(SdifSignatureConst('E','H','O','P'),NULL);
  SdifMatrixTypeInsertTailColumnDef(myMatrixType,"Index");  

  SdifPutMatrixType(entity.GetFile()->MatrixTypesTable, myMatrixType);


  /* Creation du nouveau type de frame */
  myFrameType = SdifCreateFrameType(SdifSignatureConst('E','H','O','P'),NULL);

  SdifFrameTypePutComponent(myFrameType, SdifSignatureConst('E','H','O','P'),"LorisTracks");
  
  SdifPutFrameType(entity.GetFile()->FrameTypesTable, myFrameType);




    SdifGenKill();
    return 0;
}
   
