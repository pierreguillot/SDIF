/* $Id: SdifFScan.c,v 2.2 1999-01-23 13:57:28 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * F : SdifFileT* SdifF, Scan : pseudo-sdif text file read (SdifF->TextStream)
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 *
 *
 */


#include "SdifFScan.h"
#include "SdifTest.h"
#include "SdifFile.h"

#include "SdifRWLowLevel.h"

#include "SdifNameValue.h"
#include "SdifHash.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifErrMess.h"

#include "SdifTimePosition.h"

#include "SdifFGet.h"

#include <stdlib.h>


size_t 
SdifFScanGeneralHeader(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  
  SdiffGetSignature(SdifF->TextStream, &(SdifF->CurrSignature), &SizeR);
  if (SdifF->CurrSignature != eSDIF)
    {
      sprintf(gSdifErrorMess,
	      "'%s' not correctly read\t: '%s'.",
	       SdifSignatureToString(eSDIF),
	       SdifSignatureToString(SdifF->CurrSignature));
      /*_SdifFError(SdifF, eBadHeader, "SDIF not correctly read");
	  */
      _SdifFError(SdifF, eBadHeader, gSdifErrorMess);
      return 0;
    }

  return SizeR;
}






size_t
SdifFScanNameValueLCurrNVT(SdifFileT *SdifF)
{
    return SdifFGetNameValueLCurrNVT(SdifF, 't');
}





size_t
SdifFScanMatrixType(SdifFileT *SdifF)
{
  return SdifFGetOneMatrixType(SdifF, 't');
}







size_t
SdifFScanFrameType(SdifFileT *SdifF)
{
  return SdifFGetOneFrameType(SdifF, 't');
}







/* SdifFScanAllType ne lit pas "1TYP" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "1TYP"
 */
size_t
SdifFScanAllType(SdifFileT *SdifF)
{
  return SdifFGetAllType(SdifF, 't');
}









/* SdifFScanAllStreamID ne lit pas "1IDS" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "1IDS"
 */
size_t
SdifFScanAllStreamID(SdifFileT *SdifF)
{
  return SdifFGetAllStreamID(SdifF, 't');
}







/* Read from the end of File Header until TO HAVE READ 'SDFC' */
size_t
SdifFScanAllASCIIChunks(SdifFileT *SdifF)
{
  size_t    SizeR = 0;
  
  while (   (SdiffGetSignature(SdifF->TextStream, &(SdifF->CurrSignature), &SizeR)) != eEof  )
    {
      switch (SdifF->CurrSignature)
	{

	case e1NVT :
	  SdifNameValuesLNewTable(SdifF->NameValues, _SdifNoStreamID, _Sdif_MIN_DOUBLE_);
	  SizeR += SdifFScanNameValueLCurrNVT(SdifF);
	  break;

	case e1TYP :
	  SizeR += SdifFScanAllType(SdifF);
	  break;
	  
	case e1IDS :
	      SizeR += SdifFScanAllStreamID(SdifF);
	  break;
	  
	case eENDC :
	  _SdifFError(SdifF, eSyntax, "Attempt to read 'SDFC' failed");
	case eSDFC :
	case eENDF :
	  return SizeR;
	  
	default :
	  sprintf(gSdifErrorMess,
		  "It is not a chunk name : '%s'",
		  SdifSignatureToString(SdifF->CurrSignature));
	  _SdifFError(SdifF, eSyntax, gSdifErrorMess);
	}
    }
  return SizeR;
}










void
SdifFScanMatrixHeader(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  SdifUInt4 DataType;
  
  SdifFCreateCurrMtrxH(SdifF); /* create only if it's necessary */
  
  SdiffGetSignature(SdifF->TextStream, &(SdifF->CurrMtrxH->Signature), &SizeR);


  fscanf(SdifF->TextStream, "%u", &(DataType));
  if ((SdifDataTypeET) (int) DataType == eFloat4Old) /* when DataType was 32 for Float4 at Ircam */
    DataType = eFloat4;
  SdifF->CurrMtrxH->DataType = (SdifDataTypeET) (int) DataType;

  fscanf(SdifF->TextStream, "%u", &(SdifF->CurrMtrxH->NbRow));
  fscanf(SdifF->TextStream, "%u", &(SdifF->CurrMtrxH->NbCol));

  if ( SdifFTestMatrixHeader(SdifF) )
    {
      SdifReInitOneRow(SdifF->CurrOneRow,
                       SdifF->CurrMtrxH->DataType,
                       SdifF->CurrMtrxH->NbCol);
    }

}









void
SdifFScanOneRow(SdifFileT *SdifF)
{
  switch (SdifF->CurrOneRow->DataType)
    {
    case eFloat4 :
      SdiffScanFloat4(SdifF->TextStream, SdifF->CurrOneRow->Data.F4, SdifF->CurrOneRow->NbData);
      break;
    case eFloat8 :
      SdiffScanFloat8(SdifF->TextStream, SdifF->CurrOneRow->Data.F8, SdifF->CurrOneRow->NbData);
      break;
    default :
      sprintf(gSdifErrorMess, "OneRow 0x%04x, then Float4 used", SdifF->CurrOneRow->DataType);
      _SdifFError(SdifF, eTypeDataNotSupported, gSdifErrorMess);
      SdiffScanFloat4(SdifF->TextStream, SdifF->CurrOneRow->Data.F4, SdifF->CurrOneRow->NbData);
    }
}







/* Frame signature read before and update */
void
SdifFScanFrameHeader(SdifFileT *SdifF)
{

  SdifFCreateCurrFramH(SdifF, SdifF->CurrSignature);
    
  SdifF->CurrFramH->Size = _SdifUnknownSize ;
  fscanf(SdifF->TextStream, "%u", &(SdifF->CurrFramH->NbMatrix));
  fscanf(SdifF->TextStream, "%u", &(SdifF->CurrFramH->NumID));
  fscanf(SdifF->TextStream, "%lg", &(SdifF->CurrFramH->Time));


  SdifF->CurrFramT = SdifTestFrameType(SdifF, SdifF->CurrFramH->Signature);
  if (SdifF->CurrFramT)
    SdifFReInitMtrxUsed(SdifF);
}

/*
 * obsolete
 */


size_t
SdifFScanNameValueCurrHT(SdifFileT *SdifF)
{
    /* obsolete */
    _Debug("SdifFScanNameValueCurrHT is obsolete, use SdifFScantNameValueLCurrNVT");
    return SdifFScanNameValueLCurrNVT(SdifF);
}


