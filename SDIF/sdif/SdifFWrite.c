/* $Id: SdifFWrite.c,v 3.4 1999-10-13 16:05:43 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * F : SdifFileT* SdifF, Write : sdif file write (SdifF->Stream)
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.3  1999/10/07  15:12:21  schwarz
 * Added isSeekable flag in SdifFileT struct.  This allows to simplify the
 * many tests for stdio on opening the stream.
 * Added SdifStrEq utility function.
 *
 * Revision 3.2  1999/09/28  13:08:56  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:56:47  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:46  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:30  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 */

#include <preincluded.h>
#include "SdifFWrite.h"
#include "SdifFile.h"
#include "SdifTest.h"
#include "SdifHash.h"
#include "SdifRWLowLevel.h"
#include "SdifFPut.h"
#include "SdifErrMess.h"

#include <string.h>



void
SdifUpdateChunkSize(SdifFileT *SdifF, size_t ChunkSize)
{
  SdiffPosT WritePos;
  SdifInt4 ChunkSizeInt4;

  ChunkSizeInt4 = (SdifInt4) ChunkSize;
 
  /* proper solution:  SdifFileT.isSeekable flag is false for stdio and
     pipe i/o.  In this case, don't even try to update chunk size. */
  if (SdifF->isSeekable)
    {
      if (SdiffGetPos(SdifF->Stream, &(SdifF->Pos)) != 0)
	_SdifRemark("SdifUpdateChunkSize, SdifFGetPos erreur\n");
      else
	{
	  WritePos = sizeof(SdifSignature) + SdifF->StartChunkPos;
	  SdiffSetPos(SdifF->Stream, &WritePos);

    /*  ChunkSizeInt4 = (SdifF->Pos - SdifF->StartChunkPos) -sizeof(SdifSignature) -sizeof(SdifUInt4);
	*/
	  SdiffWriteInt4(&ChunkSizeInt4, 1, SdifF->Stream);
	  if (SdiffSetPos(SdifF->Stream, &(SdifF->Pos)) !=0)
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
SdifFWriteGeneralHeader(SdifFileT *SdifF)
{
  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

  SdifF->ChunkSize  = SdifFWriteChunkHeader (SdifF, eSDIF, 2 * sizeof (SdifUInt4));
  SdifF->ChunkSize += sizeof(SdifUInt4) * 
		   SdiffWriteUInt4 (&(SdifF->FormatVersion), 1, SdifF->Stream);
  SdifF->ChunkSize += sizeof(SdifUInt4) * 
		   SdiffWriteUInt4 (&(SdifF->TypesVersion), 1, SdifF->Stream);

  /* We know how big the header is.  No need to update it.
     SdifUpdateChunkSize(SdifF, SdifF->ChunkSize -sizeof(SdifSignature) -sizeof(SdifInt4)); */  

  return SdifF->ChunkSize;  
}







size_t
SdifFWriteOneNameValue(SdifFileT *SdifF, SdifNameValueT *NameValue)
{
  return SdifFPutOneNameValue(SdifF, 's', NameValue);
}





size_t
SdifFWriteNameValueLCurrNVT (SdifFileT *f)
{
#if (_SdifFormatVersion >= 3)
  /* write NVT as frame (for now with no matrices, but ascii data) */
  SdifFSetCurrFrameHeader (f, e1NVT, _SdifUnknownSize, 0, 
			   f->NameValues->CurrNVT->NumIDLink,
			   f->NameValues->CurrNVT->Time);
  f->ChunkSize  = SdifFWriteFrameHeader (f);
  f->ChunkSize += SdifFPutNameValueLCurrNVT (f, 's');
  f->ChunkSize += SdifFWritePadding (f, SdifFPaddingCalculate (f->Stream, 
							       f->ChunkSize));
  SdifUpdateChunkSize (f, f->ChunkSize - sizeof(SdifSignature) - sizeof(SdifInt4));
#else
  SdiffGetPos(f->Stream, &(f->StartChunkPos));

  f->ChunkSize  = SdifFWriteChunkHeader(f, e1NVT, _SdifUnknownSize);
  f->ChunkSize += SdifFPutNameValueLCurrNVT(f, 's');
  f->ChunkSize += SdifFWritePadding(f, SdifFPaddingCalculate(f->Stream, f->ChunkSize));

  SdifUpdateChunkSize(f, f->ChunkSize -sizeof(SdifSignature) -sizeof(SdifInt4));
#endif

  return f->ChunkSize;  
}






size_t
SdifFWriteAllNameValueNVT(SdifFileT *SdifF)
{
    size_t SizeW = 0;

    SdifListInitLoop(SdifF->NameValues->NVTList);
    while (SdifListIsNext(SdifF->NameValues->NVTList))
    {
        SdifF->NameValues->CurrNVT = SdifListGetNext(SdifF->NameValues->NVTList);
        SizeW += SdifFWriteNameValueLCurrNVT (SdifF);
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
      SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

      SizeW  = SdifFWriteChunkHeader(SdifF, e1TYP, _SdifUnknownSize);
      SizeW += SdifFPutAllType(SdifF, 's');
      SizeW += SdifFWritePadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeW));

      SdifUpdateChunkSize(SdifF, SizeW -sizeof(SdifSignature) -sizeof(SdifInt4));

      SdifF->ChunkSize = SizeW;
      SdifF->TypeDefPass = eWritePass;
      return SdifF->ChunkSize;
    }
  else
    _SdifFError(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1TYP));
  
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
      SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));
      
      SizeW  = SdifFWriteChunkHeader(SdifF, e1IDS, _SdifUnknownSize);
      SizeW += SdifFPutAllStreamID(SdifF, 's');
      SizeW += SdifFWritePadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeW));
      
      SdifUpdateChunkSize(SdifF, SizeW -sizeof(SdifSignature) -sizeof(SdifInt4));

      SdifF->ChunkSize = SizeW;
      SdifF->StreamIDPass = eWritePass;
      return SdifF->ChunkSize;
    }
  else
    _SdifFError(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1IDS));

  return 0;
}




size_t
SdifFWriteAllASCIIChunks(SdifFileT *SdifF)
{
  size_t SizeW = 0;

  if (SdifNameValuesLIsNotEmpty(SdifF->NameValues))
    SizeW += SdifFWriteAllNameValueNVT(SdifF);

  if (   (SdifExistUserMatrixType(SdifF->MatrixTypesTable))
      || (SdifExistUserFrameType(SdifF->MatrixTypesTable)) )
    SizeW += SdifFWriteAllType(SdifF);
  
  if (SdifStreamIDTableGetNbData  (SdifF->StreamIDsTable) > 0)
    SizeW += SdifFWriteAllStreamID(SdifF);
  
  return SizeW;
}






size_t
SdifFWriteMatrixHeader (SdifFileT *SdifF)
{
  size_t SizeW = 0;
  SdifUInt4 UIntTab[3];

  UIntTab[0] = SdifF->CurrMtrxH->DataType;
  UIntTab[1] = SdifF->CurrMtrxH->NbRow;
  UIntTab[2] = SdifF->CurrMtrxH->NbCol;
  
  SizeW += sizeof(SdifSignature) * SdiffWriteSignature( &(SdifF->CurrMtrxH->Signature), SdifF->Stream);
  SizeW += sizeof(SdifUInt4)     * SdiffWriteUInt4(UIntTab, 3, SdifF->Stream);
  return SizeW;
}






size_t 
SdifFWriteOneRow (SdifFileT *SdifF)
{
    /* case template for type from SdifDataTypeET */
#   define writerowcase(type) \
    case e##type:  return (sizeof (Sdif##type) *			   \
			   SdiffWrite##type (SdifF->CurrOneRow->Data.type, \
					     SdifF->CurrOneRow->NbData,    \
					     SdifF->Stream));

    switch (SdifF->CurrOneRow->DataType)
    {
        /* generate cases for all types */
	sdif_foralltypes (writerowcase)

	default :
	    sprintf(gSdifErrorMess, "OneRow 0x%04x, then Float4 used", 
		    SdifF->CurrOneRow->DataType);
	    _SdifFError(SdifF, eTypeDataNotSupported, gSdifErrorMess);
	    return (sizeof (SdifFloat4) * 
		    SdiffWriteFloat4(SdifF->CurrOneRow->Data.Float4,
				     SdifF->CurrOneRow->NbData,
				     SdifF->Stream));
    }
}





size_t
SdifFWriteFrameHeader (SdifFileT *SdifF)
{
  size_t SizeW = 0;

  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

  SizeW += sizeof(SdifSignature) * SdiffWriteSignature( &(SdifF->CurrFramH->Signature), SdifF->Stream);
  SizeW += sizeof(SdifUInt4)     * SdiffWriteUInt4(     &(SdifF->CurrFramH->Size), 1, SdifF->Stream);
  SizeW += sizeof(SdifFloat8)    * SdiffWriteFloat8(    &(SdifF->CurrFramH->Time), 1, SdifF->Stream);
  SizeW += sizeof(SdifUInt4)     * SdiffWriteUInt4(     &(SdifF->CurrFramH->NumID), 1, SdifF->Stream);
  SizeW += sizeof(SdifUInt4)     * SdiffWriteUInt4(     &(SdifF->CurrFramH->NbMatrix), 1, SdifF->Stream);

  return SizeW;
}


/*
 * obsolete
 */

size_t
SdifFWriteNameValueCurrHT (SdifFileT *SdifF)
{
    /* obsolete */
    _Debug("SdifFWriteNameValueCurrHT is obsolete, use SdifFWriteValueLCurrNVT");
    return SdifFWriteNameValueLCurrNVT(SdifF);
}


size_t
SdifFWriteAllNameValueHT(SdifFileT *SdifF)
{
    /* obsolete */
    _Debug("SdifFWriteAllNameValueHT is obsolete, use SdifFWriteAllNameValueNVT");
    return SdifFWriteAllNameValueNVT(SdifF);
}


