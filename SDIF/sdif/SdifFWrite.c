/* SdifFWrite.c
 *
 * F : SdifFileT* SdifF, Write : sdif file write (SdifF->Stream)
 *
 *
 * author: Dominique Virolle 1997
 *
 *
 */

#include "SdifFWrite.h"
#include "SdifFile.h"
#include "SdifTest.h"
#include "SdifHash.h"
#include "SdifRWLowLevel.h"
#include "SdifFPut.h"

#include <string.h>



void
SdifUpdateChunkSize(SdifFileT *SdifF, size_t ChunkSize)
{
  SdifFPosT WritePos;
  SdifInt4 ChunkSizeInt4;

  ChunkSizeInt4 = (SdifInt4) ChunkSize;
 
  if  ( (SdifF->Stream != stdout) && (SdifF->Stream != stderr) )
    {
      if (SdifFGetPos(SdifF->Stream, &(SdifF->Pos)) != 0)
	_SdifRemark("SdifUpdateChunkSize, SdifFGetPos erreur\n");
      else
	{
	  WritePos = sizeof(SdifSignature) + SdifF->StartChunkPos;
	  SdifFSetPos(SdifF->Stream, &WritePos);

	  /*  ChunkSizeInt4 = (SdifF->Pos - SdifF->StartChunkPos) -sizeof(SdifSignature) -sizeof(SdifUInt4);
	   */
	  SdiffWriteInt4(&ChunkSizeInt4, 1, SdifF->Stream);
	  if (SdifFSetPos(SdifF->Stream, &(SdifF->Pos)) !=0)
	    _SdifRemark("SdifUpdateChunkSize, SdifFSetPos erreur\n");
	}
    }
}




size_t
SdifFWritePadding(SdifFileT *SdifF, size_t Padding)
{
  size_t SizeW = 0;
  
  if (Padding > 0)
    {
      memset(gSdifString, _SdifPaddingChar, Padding);
      SizeW += sizeof(char) * Sdiffwrite(gSdifString, sizeof(char), Padding, SdifF->Stream);
    }

  return  SizeW;
}







size_t SdifFWriteGeneralHeader(SdifFileT *SdifF)
{
  size_t SizeW = 0;
  SdifUInt4 NONE = 0;

  SdifF->CurrSignature = eSDIF;
  SizeW += sizeof(SdifSignature) * SdiffWriteSignature (&SdifF->CurrSignature, SdifF->Stream);
  SizeW += sizeof(SdifUInt4)     * SdiffWriteUInt4     (&NONE, 1, SdifF->Stream);

  return SizeW;
}







size_t SdifFWriteChunkHeader (SdifFileT *SdifF, SdifSignature ChunkSignature, size_t ChunkSize)
{
  size_t SizeW = 0;
  SdifInt4 ChunkSizeInt4;

  ChunkSizeInt4 = (SdifInt4) ChunkSize;

  SizeW += sizeof(SdifSignature) * SdiffWriteSignature (&ChunkSignature, SdifF->Stream);
  SizeW += sizeof(SdifInt4)      * SdiffWriteInt4(&ChunkSizeInt4, 1, SdifF->Stream);

  return SizeW;
}







size_t
SdifFWriteOneNameValue(SdifFileT *SdifF, SdifNameValueT *NameValue)
{
  return SdifFPutOneNameValue(SdifF, 's', NameValue);
}





size_t
SdifFWriteNameValueCurrHT (SdifFileT *SdifF)
{
  SdifFGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

  SdifF->ChunkSize  = SdifFWriteChunkHeader(SdifF, e1NVT, _SdifUnknownSize);
  SdifF->ChunkSize += SdifFPutNameValueCurrHT(SdifF, 's');
  SdifF->ChunkSize += SdifFWritePadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SdifF->ChunkSize));

  SdifUpdateChunkSize(SdifF, SdifF->ChunkSize -sizeof(SdifSignature) -sizeof(SdifInt4));

  
  return SdifF->ChunkSize;  
}






size_t
SdifFWriteAllNameValueHT(SdifFileT *SdifF)
{
  size_t SizeW = 0;
  int    iHT;

  for (iHT=1; iHT<=SdifF->NameValues->NbHTN; iHT++)
    {
      SdifNameValuesLSetCurrHT(SdifF->NameValues, iHT);
      SizeW += SdifFWriteNameValueCurrHT (SdifF);
    }

  return SizeW;
}




size_t
SdifFWriteOneMatrixType (SdifFileT *SdifF, SdifMatrixTypeT *MatrixType)
{
  return SdifFPutOneMatrixType(SdifF, 's', MatrixType);
}




size_t
SdifFWriteAllMatrixType (SdifFileT* SdifF)
{
  return SdifFPutAllMatrixType(SdifF, 's');
}




size_t
SdifFWriteOneComponent (SdifFileT *SdifF, SdifComponentT *Component)
{
  return SdifFPutOneComponent(SdifF, 's', Component);
}





size_t
SdifFWriteOneFrameType (SdifFileT *SdifF, SdifFrameTypeT *FrameType)
{
  return SdifFPutOneFrameType(SdifF, 's', FrameType);
}





size_t
SdifFWriteAllFrameType (SdifFileT *SdifF)
{
  return SdifFPutAllFrameType(SdifF, 's');
}




size_t
SdifFWriteAllType (SdifFileT *SdifF)
{
  size_t SizeW = 0;

  if ((SdifF->TypeDefPass == eNotPass) || (SdifF->TypeDefPass == eReadPass))
    {      
      SdifFGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

      SizeW  = SdifFWriteChunkHeader(SdifF, e1TYP, _SdifUnknownSize);
      SizeW += SdifFPutAllType(SdifF, 's');
      SizeW += SdifFWritePadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeW));

      SdifUpdateChunkSize(SdifF, SizeW -sizeof(SdifSignature) -sizeof(SdifInt4));

      SdifF->ChunkSize = SizeW;
      SdifF->TypeDefPass = eWritePass;
      return SdifF->ChunkSize;
    }
  else
    _SdifFileMess(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1TYP));
  
  return 0;
}





size_t
SdifFWriteOneStreamID(SdifFileT *SdifF, SdifStreamIDT *StreamID)
{
  return SdifFPutOneStreamID(SdifF, 's', StreamID);
}





size_t
SdifFWriteAllStreamID (SdifFileT *SdifF)
{
  size_t SizeW;

  if ((SdifF->StreamIDPass == eNotPass) || (SdifF->StreamIDPass == eReadPass))
    {      
      SdifFGetPos(SdifF->Stream, &(SdifF->StartChunkPos));
      
      SizeW  = SdifFWriteChunkHeader(SdifF, e1IDS, _SdifUnknownSize);
      SizeW += SdifFPutAllStreamID(SdifF, 's');
      SizeW += SdifFWritePadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeW));
      
      SdifUpdateChunkSize(SdifF, SizeW -sizeof(SdifSignature) -sizeof(SdifInt4));

      SdifF->ChunkSize = SizeW;
      SdifF->StreamIDPass = eWritePass;
      return SdifF->ChunkSize;
    }
  else
    _SdifFileMess(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1IDS));

  return 0;
}




size_t
SdifFWriteAllASCIIChunks(SdifFileT *SdifF)
{
  size_t SizeW = 0;

  if (SdifNameValuesLIsNotEmpty(SdifF->NameValues))
    SizeW += SdifFWriteAllNameValueHT(SdifF);

  if (   (SdifExistUserMatrixType(SdifF->MatrixTypesTable))
      || (SdifExistUserFrameType(SdifF->MatrixTypesTable)) )
    SizeW += SdifFWriteAllType(SdifF);
  
  if (SdifF->StreamIDsTable->NbOfData > 0)
    SizeW += SdifFWriteAllStreamID(SdifF);
  
  return SizeW;
}






size_t
SdifFWriteMatrixHeader (SdifFileT *SdifF)
{
  size_t SizeW = 0;
  SdifFloat4 FloatTab[3];

  FloatTab[0] = (SdifFloat4) SdifF->CurrMtrxH->DataType;
  FloatTab[1] = (SdifFloat4) SdifF->CurrMtrxH->NbRow;
  FloatTab[2] = (SdifFloat4) SdifF->CurrMtrxH->NbCol;
  
  SizeW += sizeof(SdifSignature) * SdiffWriteSignature( &(SdifF->CurrMtrxH->Signature), SdifF->Stream);
  SizeW += sizeof(SdifFloat4)    * SdiffWriteFloat4(FloatTab, 3, SdifF->Stream);
  return SizeW;
}






size_t SdifFWriteOneRow (SdifFileT *SdifF)
{
  switch (SdifF->CurrOneRow->DataType)
    {
    case eFloat4 :
      return sizeof(SdifFloat4) * SdiffWriteFloat4(SdifF->CurrOneRow->Data.F4,
						   SdifF->CurrOneRow->NbData,
						   SdifF->Stream);
    case eFloat8 :
      return sizeof(SdifFloat8) * SdiffWriteFloat8(SdifF->CurrOneRow->Data.F8,
						   SdifF->CurrOneRow->NbData,
						   SdifF->Stream);
    default :
      sprintf(gSdifErrorMess, "OneRow 0x%04x, then Float4 used", SdifF->CurrOneRow->DataType);
      _SdifFileMess(SdifF, eTypeDataNotSupported, gSdifErrorMess);
      return sizeof(SdifFloat4) * SdiffWriteFloat4(SdifF->CurrOneRow->Data.F4,
						   SdifF->CurrOneRow->NbData,
						   SdifF->Stream);
    }
}





size_t
SdifFWriteFrameHeader (SdifFileT *SdifF)
{
  size_t SizeW = 0;
  SdifUInt4 UInt4Tab[3];

  UInt4Tab[0] =  SdifF->CurrFramH->Size;
  UInt4Tab[1] =  SdifF->CurrFramH->NbMatrix;
  UInt4Tab[2] =  SdifF->CurrFramH->NumID;

  SdifFGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

  SizeW += sizeof(SdifSignature) * SdiffWriteSignature( &(SdifF->CurrFramH->Signature), SdifF->Stream);
  SizeW += sizeof(SdifUInt4)     * SdiffWriteUInt4( UInt4Tab , 3, SdifF->Stream);
  SizeW += sizeof(SdifFloat8)    * SdiffWriteFloat8( &(SdifF->CurrFramH->Time), 1, SdifF->Stream);

  return SizeW;
}
