/* $Id: SdifFWrite.c,v 3.8 2000-04-11 13:25:39 schwarz Exp $
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
 * Revision 3.7  2000/03/01  11:19:45  schwarz
 * Added functions for matrix-wise writing:  SdifUpdateFrameHeader,
 * SdifFWriteMatrixData, SdifFWriteMatrix, SdifFWriteFrameAndOneMatrix
 *
 * Revision 3.6  1999/11/03  16:42:34  schwarz
 * Use _SdifNVTStreamID for stream ID of 1NVT frames because of CNMAT
 * restriction of only one frame type per stream.
 * (See SdifNameValuesLNewTable)
 *
 * Revision 3.5  1999/10/15  12:28:44  schwarz
 * Updated writing of types and stream-id chunks to frames.
 * No time parameter for name value tables and stream ID tables, since
 * this decision is better left to the library.  (It uses the _SdifNoTime
 * constant, which happens to be _Sdif_MIN_DOUBLE_.)
 *
 * Revision 3.4  1999/10/13  16:05:43  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
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


/* Correct frame size and number of matrices */
int
SdifUpdateFrameHeader (SdifFileT *SdifF, size_t ChunkSize, SdifInt4 NumMatrix)
{
    int	      ret = -1;
    SdiffPosT WritePos;
    SdifInt4  ChunkSizeInt4;

    ChunkSizeInt4 = (SdifInt4) ChunkSize;
 
    /* proper solution:  SdifFileT.isSeekable flag is false for stdio and
       pipe i/o.  In this case, don't even try to update chunk size. */
    if (!SdifF->isSeekable)
      _SdifRemark("SdifUpdateFrameHeader: Can't update non-seekable stream\n");
    else
    {
	if (SdiffGetPos(SdifF->Stream, &(SdifF->Pos)) != 0)
	    _SdifRemark("SdifUpdateFrameHeader, SdifFGetPos error\n");
	else
	{
	    /* skip frame signature */
	    WritePos = SdifF->StartChunkPos + sizeof(SdifSignature);
	    SdiffSetPos (SdifF->Stream, &WritePos);
	    SdiffWriteInt4 (&ChunkSizeInt4, 1, SdifF->Stream);

	    /* skip frame signature, frame size, time, stream id */
	    WritePos = SdifF->StartChunkPos  + sizeof(SdifSignature) 
		     + 2 * sizeof (SdifInt4) + sizeof (SdifFloat8);
	    SdiffSetPos (SdifF->Stream, &WritePos);
	    SdiffWriteInt4 (&NumMatrix, 1, SdifF->Stream);

	    if (SdiffSetPos (SdifF->Stream, &(SdifF->Pos)) !=0)
		_SdifRemark ("SdifUpdateFrameHeader, SdifFSetPos error\n");
	    else
		ret = 0;
	}
    }
    return (ret);
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




/* write one 1NVT frame */
size_t
SdifFWriteNameValueLCurrNVT (SdifFileT *f)
{
  char nvtstring [_SdifStringLen];
  int  nvtlength;
 
  SdiffGetPos(f->Stream, &(f->StartChunkPos));

#if (_SdifFormatVersion >= 3)
  /* write NVT as frame with 1 text matrix */
  SdifFSetCurrFrameHeader  (f, e1NVT, _SdifUnknownSize, 1, 
			    f->NameValues->CurrNVT->StreamID, _SdifNoTime);
  f->ChunkSize  = SdifFWriteFrameHeader (f);
  nvtlength     = SdifFNameValueLCurrNVTtoString(f, nvtstring, _SdifStringLen);
  f->ChunkSize += SdifFWriteTextMatrix (f, e1NVT, nvtlength, nvtstring);
#else
  f->ChunkSize  = SdifFWriteChunkHeader (f, e1NVT, _SdifUnknownSize);
  f->ChunkSize += SdifFPutNameValueLCurrNVT (f, 's');
  f->ChunkSize += SdifFWritePadding (f, SdifFPaddingCalculate (f->Stream, 
							       f->ChunkSize));
#endif
  SdifUpdateChunkSize (f, f->ChunkSize - sizeof (SdifSignature) 
				       - sizeof (SdifInt4));

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
  char typstr [_SdifStringLen];
  int  typlen;

  if ((SdifF->TypeDefPass == eNotPass) || (SdifF->TypeDefPass == eReadPass))
    {      
      SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

#if (_SdifFormatVersion >= 3)
      /* write types as frame (for now with no matrices, but ascii data) */
      SdifFSetCurrFrameHeader (SdifF, e1TYP, _SdifUnknownSize, 0, 
			       _SdifNoStreamID, _SdifNoTime);
      SizeW  = SdifFWriteFrameHeader (SdifF);
 /*
      SdifFSetCurrFrameHeader (SdifF, e1TYP, _SdifUnknownSize, 1, 
			       _SdifNoStreamID, _SdifNoTime);
      SizeW  = SdifFWriteFrameHeader (SdifF);
      typlen = SdifFAllTypeToString(f, typstr, _SdifStringLen);
      SizeW += SdifFWriteTextMatrix (f, e1NVT, nvtlength, nvtstring);
 */
#else
      SizeW  = SdifFWriteChunkHeader(SdifF, e1TYP, _SdifUnknownSize);
      SizeW += SdifFPutAllType(SdifF, 's');
#endif
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
      
#if (_SdifFormatVersion >= 3)
      /* write types as frame (for now with no matrices, but ascii data) */
      SdifFSetCurrFrameHeader (SdifF, e1IDS, _SdifUnknownSize, 0, 
			       _SdifAllStreamID, _SdifNoTime);
      SizeW  = SdifFWriteFrameHeader (SdifF);
#else
      SizeW  = SdifFWriteChunkHeader(SdifF, e1IDS, _SdifUnknownSize);
#endif
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
      || (SdifExistUserFrameType(SdifF->FrameTypesTable)) )
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


/* Write whole matrix data (but no padding).
   Data points to NbRow * NbCol * SdifSizeofDataType (DataType) bytes in
   row-major order. */
size_t 
SdifFWriteMatrixData (SdifFileT *SdifF, void *data)
{
    /* case template for type from SdifDataTypeET */
#   define writemdatacase(type) \
    case e##type:  return (sizeof (Sdif##type) * SdiffWrite##type (data, \
        SdifF->CurrMtrxH->NbRow * SdifF->CurrMtrxH->NbCol, SdifF->Stream));

    switch (SdifF->CurrMtrxH->DataType)
    {
        /* generate cases for all types */
	sdif_foralltypes (writemdatacase)

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


/* Write whole matrix: header, data, and padding.
   Data points to NbRow * NbCol * SdifSizeofDataType (DataType) bytes in
   row-major order. */
size_t
SdifFWriteMatrix (SdifFileT     *f,
		  SdifSignature  Signature,
		  SdifDataTypeET DataType,
		  SdifUInt4      NbRow,
		  SdifUInt4      NbCol,
		  void		*Data)
{
    size_t	bytes = 0;

    SdifFSetCurrMatrixHeader(f, Signature, DataType, NbRow, NbCol);
    bytes += SdifFWriteMatrixHeader (f);
    bytes += SdifFWriteMatrixData (f, Data);
    bytes += SdifFWritePadding (f, SdifFPaddingCalculate (f->Stream, bytes));

    return (bytes);
}


/* Write a matrix with datatype text (header, data, and padding).
   Data points to Length bytes(!) of UTF-8 encoded text.  Length
   includes the terminating '\0' character!!!  That is, to write a
   C-String, use SdifFWriteTextMatrix (f, sig, strlen (str) + 1, str);
   to include it. */
size_t 
SdifFWriteTextMatrix (SdifFileT     *f,
		      SdifSignature  Signature,
		      SdifUInt4      Length,
		      char	    *Data)
{
    size_t	bytes = 0;

    SdifFSetCurrMatrixHeader (f, Signature, eText, Length, 1);
    bytes += SdifFWriteMatrixHeader (f);
    bytes += SdifFWriteMatrixData (f, Data);
    bytes += SdifFWritePadding (f, SdifFPaddingCalculate (f->Stream, bytes));

    return (bytes);
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

size_t  
SdifFWriteFrameAndOneMatrix (SdifFileT	    *f,
			     SdifSignature  FrameSignature,
			     SdifUInt4      NumID,
			     SdifFloat8     Time,
			     SdifSignature  MatrixSignature,
			     SdifDataTypeET DataType,
			     SdifUInt4      NbRow,
			     SdifUInt4      NbCol,
			     void	    *Data)
{
    /* calculate frame size (frame sig and frame size field is not counted) */
    SdifUInt4 fsz = 
           /* frame  header  */ sizeof(SdifFloat8)    + 2 * sizeof(SdifUInt4) +
           /* matrix header  */ sizeof(SdifSignature) + 3 * sizeof(SdifUInt4) +
           /* matrix data    */ NbRow * NbCol * SdifSizeofDataType (DataType);
    fsz += /* matrix padding */ SdifPaddingCalculate (fsz);

    SdifFSetCurrFrameHeader (f, FrameSignature, fsz, 1, NumID, Time);
    fsz  = SdifFWriteFrameHeader (f);
    fsz += SdifFWriteMatrix (f, MatrixSignature, DataType, NbRow, NbCol, Data);

    return (fsz);
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


