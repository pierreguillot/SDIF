/* $Id: SdifFPut.c,v 3.1 1999-03-14 10:56:41 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * F : SdifFileT* SdifF, Put : write ascii frames into pseudo-sdif text file,
 * SdifF->TextStream or sdif file SdifF->Stream.
 * switch 'verbose'
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 *
 */


#include "SdifFPut.h"

#include "SdifTest.h"
#include "SdifFile.h"

#include "SdifRWLowLevel.h"

#include "SdifList.h"
#include "SdifHash.h"








size_t
SdifFPutOneNameValue(SdifFileT* SdifF, int Verbose, SdifNameValueT *NameValue)
{
  size_t  SizeW = 0;
  FILE   *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
  
  SizeW += fprintf(file, "%s\t", NameValue->Name);
  SizeW += fprintf(file, "%s;\n", NameValue->Value);

  return  SizeW;
}







size_t
SdifFPutNameValueLCurrNVT (SdifFileT *SdifF, int Verbose)
/* SdifFPutNameValueLCurrNVT doesn't write frame header and padding */
{
  size_t          SizeW = 0;
  SdifUInt4       iNV;
  SdifHashNT     *pNV;
  SdifHashTableT *HTable;
  FILE           *file;


  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  HTable = SdifF->NameValues->CurrNVT->NVHT;
  
  SizeW += fprintf(file, "{\n");
  for(iNV=0; iNV<HTable->HashSize; iNV++)
    for (pNV = HTable->Table[iNV]; pNV; pNV = pNV->Next)
      SizeW += SdifFPutOneNameValue(SdifF, Verbose, pNV->Data);
  
  SizeW += fprintf(file, "}");  

  return SizeW;
}



size_t
SdifFPutOneMatrixType(SdifFileT* SdifF, int Verbose, SdifMatrixTypeT *MatrixType)
{
  SdifColumnDefT    *ColumnDef;
  size_t            SizeW = 0;
  FILE              *file;
  
  
  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  if (! SdifListIsEmpty(MatrixType->ColumnUserList))
    {
      SizeW += fprintf(file, "  %s\t", SdifSignatureToString(e1MTD));
      SizeW += sizeof(SdifSignature) * SdiffWriteSignature(&(MatrixType->Signature), file);
      SizeW += fprintf(file, "\t{");

      ColumnDef = SdifListGetHead(MatrixType->ColumnUserList); /* Reinit GetNext */
	  SizeW += fprintf(file, "%s",ColumnDef->Name);

      while (SdifListIsNext(MatrixType->ColumnUserList))
      {
          ColumnDef = SdifListGetNext(MatrixType->ColumnUserList);
	      SizeW += fprintf(file, ", %s",ColumnDef->Name);
      }
      SizeW += fprintf(file, "}\n");  
    }

  return SizeW;
}






size_t
SdifFPutAllMatrixType(SdifFileT* SdifF, int Verbose)
{
  unsigned int   iName;
  SdifHashNT    *pName;
  size_t         SizeW = 0;

  for(iName=0; iName<SdifF->MatrixTypesTable->HashSize; iName++)
    for (pName = SdifF->MatrixTypesTable->Table[iName]; pName;  pName=pName->Next)
      SizeW += SdifFPutOneMatrixType(SdifF, Verbose, pName->Data);
  
  return SizeW;
}




size_t
SdifFPutOneComponent(SdifFileT *SdifF, int Verbose, SdifComponentT *Component)
{
  size_t   SizeW = 0;
  FILE    *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  SizeW += fprintf(file, "\t  ");
  SizeW += sizeof(SdifSignature) * SdiffWriteSignature(&(Component->MtrxS), file);
  SizeW += fprintf(file, "\t%s;\n", Component->Name);
  
  return SizeW;
}






size_t
SdifFPutOneFrameType(SdifFileT *SdifF, int Verbose, SdifFrameTypeT *FrameType)
{
  SdifUInt4 iC;
  size_t    SizeW = 0;
  FILE      *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
  
  if (FrameType->NbComponentUse > 0)
    {
      SizeW += fprintf(file, "  %s\t", SdifSignatureToString(e1FTD));
      SizeW += sizeof(SdifSignature) * SdiffWriteSignature( &(FrameType->Signature), file);
      SizeW += fprintf(file, "\n\t{\n");
      for(iC = FrameType->NbComponent - FrameType->NbComponentUse + 1;
          iC<= FrameType->NbComponent;
          iC++)
	    {
	      SizeW += SdifFPutOneComponent(SdifF, Verbose,
                   SdifFrameTypeGetNthComponent(FrameType, iC));
	    }
      SizeW += fprintf(file, "\t}\n");
    }

  return SizeW;
}




size_t
SdifFPutAllFrameType(SdifFileT *SdifF, int Verbose)
{
  unsigned int  iName;
  SdifHashNT   *pName;
  size_t        SizeW = 0;

  for(iName=0; iName<SdifF->FrameTypesTable->HashSize; iName++)
    for (pName = SdifF->FrameTypesTable->Table[iName]; pName;  pName=pName->Next)
      SizeW += SdifFPutOneFrameType(SdifF, Verbose, pName->Data);

  return SizeW;
}




/* SdifFPutAllType writes no '1TYP', no chunk size and no padding too */
size_t
SdifFPutAllType(SdifFileT *SdifF, int Verbose)
{
  size_t  SizeW = 0;
  FILE   *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
 
  SizeW += fprintf(file, "{\n");  
  SizeW += SdifFPutAllMatrixType(SdifF, Verbose);
  SizeW += SdifFPutAllFrameType(SdifF, Verbose);  
  SizeW += fprintf(file, "}");

  return SizeW;
}








size_t
SdifFPutOneStreamID(SdifFileT *SdifF, int Verbose, SdifStreamIDT *StreamID)
{
  size_t   SizeW = 0;
  FILE    *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  SizeW += fprintf(file, "  %u ", StreamID->NumID);
  SizeW += fprintf(file, "%s:", StreamID->Source);
  SizeW += fprintf(file, "%s;\n", StreamID->TreeWay);

  return  SizeW;
}









/* SdifFPutAllStreamID doesn't write '1IDS', chunk size and padding */
size_t
SdifFPutAllStreamID(SdifFileT *SdifF, int Verbose)
{
  unsigned int     iID;
  SdifHashNT *pID;
  size_t           SizeW = 0;
  FILE             *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  SizeW += fprintf(file, "{\n");
  
  for(iID=0; iID<SdifF->StreamIDsTable->SIDHT->HashSize; iID++)
    for (pID = SdifF->StreamIDsTable->SIDHT->Table[iID]; pID; pID = pID->Next)
      SizeW += SdifFPutOneStreamID(SdifF, Verbose, pID->Data);
  
  SizeW += fprintf(file, "}");

  return SizeW;
}


/*
 * obsolete
 */

size_t
SdifFPutNameValueCurrHT (SdifFileT *SdifF, int Verbose)
{
    /* obsolete */
    _Debug("SdifFPutNameValueCurrHT is obsolete, use SdifFPutNameValueLCurrNVT");
    return SdifFPutNameValueLCurrNVT(SdifF, Verbose);
}

