/* $Id: SdifFWrite.c,v 3.12.2.1 2000-08-21 21:35:13 tisseran Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
 *
 *
 * F : SdifFileT* SdifF, Write : sdif file write (SdifF->Stream)
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.12  2000/07/18  15:08:34  tisseran
 * This release implements the New SDIF Specification (june 1999):
 * - Name Values Table are written in a 1NVT frame which contains a 1NVT matrix
 * - Frame and matrix type declaration are written in a 1TYP frame which contains a 1TYP matrix.
 * - Stream ID are written in a 1IDS frame which contains a 1IDS matrix.
 *
 * Read function accept the previous version of the specification (read a text frame without matrix) to be compatible with older SDIF files.
 *
 * SdifString.h and SdifString.c implements some string mangement (creation, destruction, append, test of end of string, getc, ungetc).
 *
 * WATCH OUT:
 *      We don't care about the old SDIF Specification (_SdifFormatVersion < 3)
 * To use _SdifFormatVersion < 3, get the previous release.
 *
 * Revision 3.11  2000/07/06  19:01:47  tisseran
 * Add function for frame and matrix type declaration
 * Remove string size limitation for NameValueTable
 * TODO: 1TYP and 1IDS frame must contain an 1NVT (text) matrix
 *       Actually, data are written with fprintf.
 *
 * Revision 3.10  2000/05/15  16:23:09  schwarz
 * Avoided avoidable warnings.
 *
 * Revision 3.9  2000/05/10  15:32:12  schwarz
 * Added functions to calculate the Size argument for SdifFSetCurrFrameHeader:
 * SdifSizeOfFrameHeader and SdifSizeOfMatrix
 *
 * Revision 3.8  2000/04/11  13:25:39  schwarz
 * Write NVT as frame with 1 text matrix, conforming to SDIF spec, using
 * new function SdifFWriteTextMatrix.
 * Fixed ancient bug in ...AllAsciiChunks: SdifExistUserFrameType worked
 * on SdifF->MatrixTypesTable, not SdifF->FrameTypesTable.
 *
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
  size_t SizeW = 0;
  SdifStringT *SdifString;
  int success;
  
  SdifString = SdifStringNew();
  
  SdiffGetPos(f->Stream, &(f->StartChunkPos));

  success = SdifFNameValueLCurrNVTtoSdifString(f, SdifString);
  SizeW += SdifFWriteTextFrameSdifString(f, e1NVT, f->NameValues->CurrNVT->StreamID,
			       _SdifNoTime, e1NVT, SdifString);

  /* Free memory allocated */
  SdifStringFree(SdifString);
  
  return SizeW;
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



/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
   Removed test for _SdifFormatVersion
   Now we write type in 1TYP frame which contains a 1TYP matrix
*/
size_t
SdifFWriteAllType (SdifFileT *SdifF)
{
  size_t SizeW = 0;
  SdifStringT *SdifString;
  int success = 1;
  
  SdifString = SdifStringNew();
  
  if ((SdifF->TypeDefPass == eNotPass) || (SdifF->TypeDefPass == eReadPass))
    {      
      SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));
            
      /* append all matrix type in SdifString */
      success *= SdifFAllMatrixTypeToSdifString(SdifF, SdifString);
      
      /* append all frame type in SdifString */
      success *= SdifFAllFrameTypeToSdifString(SdifF, SdifString);
      
      /* Now write the frame text */
      SizeW += SdifFWriteTextFrameSdifString (SdifF, e1TYP, _SdifNoStreamID, _SdifNoTime,
					      e1TYP, SdifString);

      SdifStringFree(SdifString);
      return SizeW;
    }
  else
    _SdifFError(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1TYP));
  SdifStringFree(SdifString);
  return 0;
}





size_t
SdifFWriteOneStreamID(SdifFileT *SdifF, SdifStreamIDT *StreamID)
{
  return SdifFPutOneStreamID(SdifF, 's', StreamID);
}




/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
  Removed test for _SdifFormatVersion
  Now we write type in 1IDS frame which contains a 1IDS matrix
*/
size_t
SdifFWriteAllStreamID (SdifFileT *SdifF)
{
  size_t SizeW = 0;
  SdifStringT *SdifString;
  int success = 1;

  SdifString = SdifStringNew(); /* Memory allocation for SdifString */
  
  if ((SdifF->StreamIDPass == eNotPass) || (SdifF->StreamIDPass == eReadPass))
    {      
      SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

      success *= SdifFAllStreamIDToSdifString(SdifF, SdifString);
      
      SizeW += SdifFWriteTextFrameSdifString(SdifF, e1IDS, _SdifIDSStreamID, _SdifNoTime,
					     e1IDS, SdifString);
      
      SdifStringFree(SdifString);
      return SizeW;
    }
    _SdifFError(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1TYP));
  SdifStringFree(SdifString);
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
    SdifUInt4 fsz = SdifSizeOfFrameHeader () 
                  + SdifSizeOfMatrix (DataType, NbRow, NbCol);

    SdifFSetCurrFrameHeader (f, FrameSignature, fsz, 1, NumID, Time);
    fsz  = SdifFWriteFrameHeader (f);
    fsz += SdifFWriteMatrix (f, MatrixSignature, DataType, NbRow, NbCol, Data);

    return (fsz);
}


size_t
SdifSizeOfFrameHeader (void)
{
    return (sizeof(SdifFloat8) + 2 * sizeof(SdifUInt4));
}


size_t
SdifSizeOfMatrix (SdifDataTypeET DataType,
		  SdifUInt4      NbRow,
		  SdifUInt4      NbCol)
{
    SdifUInt4 msz = 
           /* matrix header  */ sizeof(SdifSignature) + 3 * sizeof(SdifUInt4) +
           /* matrix data    */ NbRow * NbCol * SdifSizeofDataType (DataType);
    msz += /* matrix padding */ SdifPaddingCalculate (msz);
    return (msz);
}






/*DOC:
  Write a text matrix using a string.
  Return number of bytes written.
*/
size_t
SdifFWriteTextFrame(SdifFileT     *SdifF,
		    SdifSignature FrameSignature,
		    SdifUInt4     NumID,
		    SdifFloat8    Time,
		    SdifSignature MatrixSignature,
		    char          *str,
		    size_t        length)
     
{
  size_t SizeW = 0;
  size_t FrameSize = 0;
  
  FrameSize = SdifSizeOfFrameHeader();
  FrameSize += SdifSizeOfMatrix(eText,length,1);

  SdifFSetCurrFrameHeader (SdifF, FrameSignature, FrameSize, 1,
			   NumID, Time);

  SizeW = SdifFWriteFrameHeader(SdifF);
  SizeW += SdifFWriteTextMatrix(SdifF, MatrixSignature, length, str);
  
  return SizeW;
}


/*DOC:
  Write a text matrix using a SdifString.
  Return number of bytes written.
*/
size_t SdifFWriteTextFrameSdifString(SdifFileT     *SdifF,
				     SdifSignature FrameSignature,
				     SdifUInt4     NumID,
				     SdifFloat8    Time,
				     SdifSignature MatrixSignature,
				     SdifStringT   *SdifString)
{
  size_t SizeW = 0;
  size_t FrameSize = 0;
  
  FrameSize = SdifSizeOfFrameHeader();
  FrameSize += SdifSizeOfMatrix(eText,(SdifString->SizeW + 1),1);

  SdifFSetCurrFrameHeader (SdifF, FrameSignature, FrameSize, 1,
			   NumID, Time);

  SizeW = SdifFWriteFrameHeader(SdifF);
  /* Add 1 for SdifString->SizeW for future use of null terminaison */
  SizeW += SdifFWriteTextMatrix(SdifF, MatrixSignature
				, (SdifString->SizeW + 1), SdifString->str);
  
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



