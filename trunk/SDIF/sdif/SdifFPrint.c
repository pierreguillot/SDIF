/* SdifFPrint.c
 *
 * F : SdifFileT* SdifF, Print : write only into the pseudo-sdif text file, SdifF->TextStream.
 *
 *
 * author: Dominique Virolle 1997
 *
 *
 */


#include "SdifFPrint.h"
#include "SdifTest.h"
#include "SdifFile.h"

#include "SdifRWLowLevel.h"

#include "SdifNameValue.h"
#include "SdifHash.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifFPut.h"

#include "SdifTimePosition.h"






size_t
SdifFPrintGeneralHeader(SdifFileT *SdifF)
{
  size_t SizeW = 0;
  
  SizeW += fprintf(SdifF->TextStream, "%s\n\n", SdifSignatureToString(eSDIF));
  return SizeW;
}


size_t
SdifFPrintNameValueCurrHT(SdifFileT *SdifF)
{
  size_t SizeW = 0;

  SizeW += fprintf(SdifF->TextStream, "%s\n", SdifSignatureToString(e1NVT));
  SizeW += SdifFPutNameValueCurrHT (SdifF, 't');
  SizeW += fprintf(SdifF->TextStream, "\n");
  return SizeW;
}


size_t
SdifFPrintAllNameValueHT(SdifFileT *SdifF)
{
  SdifUInt4 iHT;
  size_t SizeW = 0;

  for (iHT = 1; iHT <= SdifF->NameValues->NbHTN; iHT++)
    {
      SdifNameValuesLSetCurrHT(SdifF->NameValues, iHT);
      SizeW += SdifFPrintNameValueCurrHT (SdifF);
    }

  return SizeW;
}




size_t
SdifFPrintMatrixType(SdifFileT *SdifF, SdifMatrixTypeT *MatrixType)
{
  return SdifFPutOneMatrixType(SdifF, 't', MatrixType);
}




size_t
SdifFPrintFrameType(SdifFileT *SdifF, SdifFrameTypeT *FrameType)
{
  return SdifFPutOneFrameType(SdifF, 't', FrameType);
}




size_t
SdifFPrintAllType(SdifFileT *SdifF)
{
  size_t SizeW = 0;
  
  if ((SdifF->TypeDefPass == eNotPass) || (SdifF->TypeDefPass == eReadPass))
    {      
      SizeW += fprintf(SdifF->TextStream, "%s\n", SdifSignatureToString(e1TYP));
      SizeW += SdifFPutAllType(SdifF, 't');
      SizeW += fprintf(SdifF->TextStream, "\n");

      SdifF->TypeDefPass = eWritePass;
    }
  else
    _SdifFileMess(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1TYP));

  return SizeW;
}





size_t
SdifFPrintAllStreamID(SdifFileT *SdifF)
{
  size_t SizeW = 0;

  if ((SdifF->StreamIDPass == eNotPass) || (SdifF->StreamIDPass == eReadPass))
    {      
      SizeW += fprintf(SdifF->TextStream, "%s\n", SdifSignatureToString(e1IDS));
      SizeW += SdifFPutAllStreamID(SdifF, 't');
      SizeW += fprintf(SdifF->TextStream, "\n");

      SdifF->StreamIDPass = eWritePass;
    }
  else
    _SdifFileMess(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1IDS));
      
  return SizeW;
}





size_t
SdifFPrintAllASCIIChunks(SdifFileT *SdifF)
{
  size_t SizeW = 0;

  if (SdifNameValuesLIsNotEmpty(SdifF->NameValues))
    {
      SizeW += SdifFPrintAllNameValueHT (SdifF);
      SizeW += fprintf(SdifF->TextStream, "\n");
    }

  if (   (SdifExistUserMatrixType(SdifF->MatrixTypesTable))
      || (SdifExistUserFrameType(SdifF->MatrixTypesTable)) )
    {
      SizeW += SdifFPrintAllType (SdifF);
      SizeW += fprintf(SdifF->TextStream, "\n");
    }

  if (SdifF->StreamIDsTable->NbOfData > 0)
    {
      SizeW += SdifFPrintAllStreamID (SdifF);
      SizeW += fprintf(SdifF->TextStream, "\n");
    }

  return SizeW;
}





size_t
SdifFPrintMatrixHeader(SdifFileT *SdifF)
{
  size_t
    SizeW = 0;

  SizeW += fprintf(SdifF->TextStream, "  %s\t%u\t%u\t%u\n",
		   SdifSignatureToString(SdifF->CurrMtrxH->Signature),
		   SdifF->CurrMtrxH->DataType,
		   SdifF->CurrMtrxH->NbRow,
		   SdifF->CurrMtrxH->NbCol);

  return SizeW;
}






size_t
SdifFPrintOneRow(SdifFileT *SdifF)
{
  unsigned int
    iCol;
  size_t
    SizeW = 0;
  
  switch (SdifF->CurrOneRow->DataType)
    {
    case eFloat8 :
      for(iCol=0; iCol<SdifF->CurrOneRow->NbData; iCol++)
	SizeW += fprintf(SdifF->TextStream, "\t%g", SdifF->CurrOneRow->Data.F8[iCol]);
      break;
    case eFloat4 :
      for(iCol=0; iCol<SdifF->CurrOneRow->NbData; iCol++)
	SizeW += fprintf(SdifF->TextStream, "\t%g", SdifF->CurrOneRow->Data.F4[iCol]);
      break;
    default :
      sprintf(gSdifErrorMess, "OneRow 0x%04x, then Float4 used", SdifF->CurrOneRow->DataType);
      _SdifFileMess(SdifF, eTypeDataNotSupported, gSdifErrorMess);
      /* then values are considered as Float4 */
      for(iCol=0; iCol<SdifF->CurrOneRow->NbData; iCol++)
	SizeW += fprintf(SdifF->TextStream, "\t%g", SdifF->CurrOneRow->Data.F4[iCol]);
      break;
    }
  SizeW += fprintf(SdifF->TextStream, "\n");

  return SizeW;
}






size_t
SdifFPrintFrameHeader(SdifFileT *SdifF)
{
  size_t
    SizeW = 0;
  
  SizeW += fprintf(SdifF->TextStream,
		   "%s\t%u\t%u\t%g\n",
		   SdifSignatureToString(SdifF->CurrFramH->Signature),  
		   SdifF->CurrFramH->NbMatrix,
		   SdifF->CurrFramH->NumID,
		   SdifF->CurrFramH->Time);

  return SizeW;
}
