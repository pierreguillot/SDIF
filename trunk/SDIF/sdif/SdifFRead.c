/* SdifFRead.c
 *
 * F : SdifFileT* SdifF, Read : binary read (SdifF->Stream)
 *
 *
 * author: Dominique Virolle 1997
 *
 *
 */


#include "SdifFRead.h"
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





size_t
SdifFReadChunkSize(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  SdifInt4 ChunkSizeInt4 = 0;

  SizeR += sizeof(SdifInt4) * SdiffReadInt4(&(ChunkSizeInt4), 1, SdifF->Stream);
  SdifF->ChunkSize = (size_t) ChunkSizeInt4;
  return SizeR;
}





size_t 
SdifFReadGeneralHeader(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  
  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

  SdifFGetSignature(SdifF, &SizeR);
  SizeR += SdifFReadChunkSize(SdifF);
  SizeR += sizeof(SdifUInt4) * SdiffReadUInt4 ( &(SdifF->FormatVersion), 1, SdifF->Stream);
  SizeR += SdifFReadPadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeR-8));
  
  if (SdifF->CurrSignature != eSDIF)
    {
      sprintf(gSdifErrorMess, "%s not correctly read", SdifSignatureToString(eSDIF));
      _SdifFError(SdifF, eBadHeader, gSdifErrorMess);
    }

  if (SdifF->FormatVersion > _SdifFormatVersion)
    {
      sprintf(gSdifErrorMess, "%d is hupper than %d", SdifF->FormatVersion, _SdifFormatVersion);
      _SdifFError(SdifF, eBadFormatVersion, gSdifErrorMess);
    }
    
  return SizeR;
}



/* Signature of chunck already read */
size_t
SdifFReadNameValueCurrHT(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  
  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));
  SdifF->StartChunkPos -= sizeof(SdifSignature);

  SizeR += SdifFReadChunkSize(SdifF);
  SizeR += SdifFGetNameValueCurrHT(SdifF, 's');
  SizeR += SdifFReadPadding(SdifF,
			    SdifFPaddingCalculate(SdifF->Stream, SizeR + sizeof(SdifSignature)));
  
  if (    (SizeR != SdifF->ChunkSize + sizeof(SdifInt4))
       && ((unsigned) SdifF->ChunkSize != (unsigned) _SdifUnknownSize))
    {
      sprintf(gSdifErrorMess,
	      "erreur size 1NVT: lu: %u  Attendu: %u\n",
	      SizeR - sizeof(SdifInt4),
	      SdifF->ChunkSize);
      _SdifRemark(gSdifErrorMess);
    }

  return SizeR;
}










size_t
SdifFReadOneMatrixType(SdifFileT *SdifF)
{
  return SdifFGetOneMatrixType(SdifF, 's');
}







size_t SdifFReadOneFrameType(SdifFileT *SdifF)
{
  return SdifFGetOneFrameType(SdifF, 's');
}







/* SdifFReadAllType ne lit pas "1TYP" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "1TYP"
 */
/* Signature of chunck already read */
size_t
SdifFReadAllType(SdifFileT *SdifF)
{
  size_t  SizeR = 0;
  
  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));
  SdifF->StartChunkPos -= sizeof(SdifSignature);
  
  SizeR += SdifFReadChunkSize(SdifF);
  SizeR += SdifFGetAllType(SdifF, 's');
  SizeR += SdifFReadPadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeR + sizeof(SdifSignature)));
  
  if (    (SizeR != SdifF->ChunkSize + sizeof(SdifInt4))
       && ((unsigned) SdifF->ChunkSize != (unsigned) _SdifUnknownSize))
    {
      sprintf(gSdifErrorMess,
	      "erreur size 1TYP: lu: %u  Attendu: %u\n",
	      SizeR - sizeof(SdifInt4),
	      SdifF->ChunkSize);
      _SdifRemark(gSdifErrorMess);
    }
  
  return SizeR;
}



  





/* SdifFReadAllStreamID ne lit pas "SSIC" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "SSIC"
 */
size_t
SdifFReadAllStreamID(SdifFileT *SdifF)
{
  size_t
    SizeR = 0;
  
  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));
  SdifF->StartChunkPos -= sizeof(SdifSignature);
  
  SizeR += SdifFReadChunkSize(SdifF);
  SizeR += SdifFGetAllStreamID(SdifF, 's');
  SizeR += SdifFReadPadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeR + sizeof(SdifSignature)));
  
  if (    (SizeR != SdifF->ChunkSize + sizeof(SdifInt4))
       && ((unsigned) SdifF->ChunkSize != (unsigned) _SdifUnknownSize))
    {
      sprintf(gSdifErrorMess,
	      "erreur size 1IDS: lu: %u  Attendu: %u\n",
	      SizeR - sizeof(SdifInt4),
	      SdifF->ChunkSize);
      _SdifRemark(gSdifErrorMess);
    }

  return SizeR;
}







/* Read from the end of File Header until TO HAVE READ THE FIRST FRAME SIGNATURE
 * Then, the first frame signature is stocked into SdifF->CurrSignature 
 */
size_t
SdifFReadAllASCIIChunks(SdifFileT *SdifF)
{
  size_t    SizeR = 0;
  
  while (  (SdifFGetSignature(SdifF, &SizeR)!=eEof)  )
    {
      switch (SdifF->CurrSignature)
	{
	case e1NVT :
	  SdifNameValuesLNewHT(SdifF->NameValues);
	  SizeR += SdifFReadNameValueCurrHT(SdifF);
	  break;
	  
	case e1TYP :
	  SizeR += SdifFReadAllType(SdifF);
	  break;
	  
	case e1IDS :
	  SizeR += SdifFReadAllStreamID(SdifF);
	  break;

	default:
	  return SizeR;
	}
    }

  return SizeR;
}









size_t
SdifFReadMatrixHeader(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  SdifUInt4 UIntTab[3];
  
  SdifFCreateCurrMtrxH(SdifF); /* create only if it's necessary */
  
  SdiffGetSignature(SdifF->Stream, &(SdifF->CurrMtrxH->Signature), &SizeR);
  SizeR += sizeof(SdifUInt4) * SdiffReadUInt4( UIntTab, 3, SdifF->Stream);
 
  /* when DataType was 32 for Float4 at Ircam 
  if ((SdifDataTypeET) UIntTab[0] == eFloat4Old)
    UIntTab[0] = eFloat4;
  */

  SdifF->CurrMtrxH->DataType  = (SdifDataTypeET) UIntTab[0];
  SdifF->CurrMtrxH->NbRow     = UIntTab[1];
  SdifF->CurrMtrxH->NbCol     = UIntTab[2];
  
  SdifF->CurrOneRow->DataType = SdifF->CurrMtrxH->DataType;

  if ( SdifFTestMatrixHeader(SdifF) )
    {
      SdifReInitOneRow(SdifF->CurrOneRow,
                       SdifF->CurrMtrxH->DataType,
                       SdifF->CurrMtrxH->NbCol);
    }

  return SizeR;
}






size_t
SdifFReadOneRow(SdifFileT *SdifF)
{
  switch (SdifF->CurrOneRow->DataType)
    {
    case eFloat4 :
      return sizeof(SdifFloat4) * SdiffReadFloat4(SdifF->CurrOneRow->Data.F4,
						  SdifF->CurrOneRow->NbData,
						  SdifF->Stream);
    case eFloat8 :
      return sizeof(SdifFloat8) * SdiffReadFloat8(SdifF->CurrOneRow->Data.F8,
						  SdifF->CurrOneRow->NbData,
						  SdifF->Stream);
    default :
      sprintf(gSdifErrorMess, "OneRow 0x%04x, then Float4 used", SdifF->CurrOneRow->DataType);
      _SdifFError(SdifF, eTypeDataNotSupported, gSdifErrorMess);
      return sizeof(SdifFloat4) * SdiffReadFloat4(SdifF->CurrOneRow->Data.F4,
						  SdifF->CurrOneRow->NbData,
						  SdifF->Stream);
    }
}





/* Frame Signature read yet, then update CurrFramH->FrameSignature.
 * If it'is the first ReadFrameHeader then Create CurrFramH
 */
size_t
SdifFReadFrameHeader(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  SdifUInt4 UInt4Tab[2];

  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));
  SdifF->StartChunkPos -= sizeof(SdifSignature);

  /* Create only if it's necessary else update signature */
  SdifFCreateCurrFramH(SdifF, SdifF->CurrSignature);

  SizeR += sizeof(SdifUInt4)  * SdiffReadUInt4 ( &(SdifF->CurrFramH->Size), 1, SdifF->Stream);
  SizeR += sizeof(SdifFloat8) * SdiffReadFloat8( &(SdifF->CurrFramH->Time), 1, SdifF->Stream);
  SizeR += sizeof(SdifUInt4)  * SdiffReadUInt4(  UInt4Tab, 2, SdifF->Stream);

  SdifF->CurrFramH->NumID    = UInt4Tab[0];
  SdifF->CurrFramH->NbMatrix = UInt4Tab[1];

  /*SdifFSetCurrFrameHeader (SdifF, Signature, Size, NbMatrix, NumID, Time);*/

  SdifF->CurrFramT = SdifTestFrameType(SdifF, SdifF->CurrFramH->Signature);
  if (SdifF->CurrFramT)
    SdifFReInitMtrxUsed(SdifF);

  return SizeR;
}





size_t
SdifFReadPadding (SdifFileT *SdifF, size_t Padding)
{
  return sizeof(char) * Sdiffread(gSdifString, sizeof(char), Padding, SdifF->Stream);
}





size_t
SdifFReadUndeterminatedPadding(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  int c;

  while ((c = fgetc(SdifF->Stream) == (int)' ') && (!feof(SdifF->Stream)))
    SizeR++;
  
  if (feof(SdifF->Stream))
    return SizeR;
  else
    {
      ungetc(c, SdifF->Stream);
      return SizeR;
    }
}






/* SdifSkipMatrix read entire matrix header. */
size_t
SdifSkipMatrix(SdifFileT *SdifF)
{
  size_t
    SizeR = 0;
  
  SizeR =  SdifFReadMatrixHeader(SdifF);
  SizeR += SdifSkipMatrixData(SdifF);
  return SizeR;
}






/* SdifSkipMatrixData don't read matrix header. */
size_t
SdifSkipMatrixData(SdifFileT *SdifF)
{
  size_t
    SizeR = 0,
    NbBytesToSkip;
  SdiffPosT Pos = 0;

  NbBytesToSkip =
    SdifF->CurrMtrxH->NbCol 
    * SdifF->CurrMtrxH->NbRow 
    * SdifSizeofDataType(SdifF->CurrMtrxH->DataType);

  NbBytesToSkip += SdifPaddingCalculate(NbBytesToSkip);

  SdiffGetPos(SdifF->Stream, &Pos);
  Pos += NbBytesToSkip;
  if (SdiffSetPos(SdifF->Stream, &Pos) != 0)
    return (size_t) -1;
  else
    {
      SizeR += NbBytesToSkip;
      return SizeR;
    }
}






size_t
SdifSkipFrameData(SdifFileT *SdifF)
{
  size_t
    SizeR = 0,
    Boo,
    NbBytesToSkip;
  SdifUInt4
    iMtrx;
  SdiffPosT Pos;
  
  if (SdifF->CurrFramH->Size != _SdifUnknownSize)
    {
      NbBytesToSkip = SdifF->CurrFramH->Size - _SdifFrameHeaderSize;
      SdiffGetPos(SdifF->Stream, &Pos);
      Pos += NbBytesToSkip;
      if (SdiffSetPos(SdifF->Stream, &Pos) != 0)
        {
	        sprintf(gSdifErrorMess,
		    "Skip FrameData %s ID:%u T:%g\n",
		    SdifSignatureToString(SdifF->CurrFramH->Signature),
		    SdifF->CurrFramH->NumID,
		    SdifF->CurrFramH->Time);
	        _SdifError(eEof, gSdifErrorMess);
	        return (size_t) -1;
	    }
      else
	    return (NbBytesToSkip);    
    }
  else
    {
      for (iMtrx = 0; iMtrx<SdifF->CurrFramH->NbMatrix; iMtrx++)
	{
	  Boo = SdifSkipMatrix(SdifF);
	  if (Boo == -1)
	    {
	      sprintf(gSdifErrorMess,
		      "Skip Matrix %d in FrameData %s ID:%u T:%g\n",
		      iMtrx,
		      SdifSignatureToString(SdifF->CurrFramH->Signature),
		      SdifF->CurrFramH->NumID,
		      SdifF->CurrFramH->Time);
	      _SdifError(eEof, gSdifErrorMess);
	      return (size_t) -1;
	    }
	  else
	    SizeR += Boo;
	}
      return SizeR;
    }
}
