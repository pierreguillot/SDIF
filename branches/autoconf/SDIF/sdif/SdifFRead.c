/* $Id: SdifFRead.c,v 3.10.2.2 2000-08-21 18:34:09 tisseran Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * F : SdifFileT* SdifF, Read : binary read (SdifF->Stream)
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.10.2.1  2000/08/21  14:04:11  tisseran
 * *** empty log message ***
 *
 * Revision 3.9  2000/07/18  15:08:32  tisseran
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
 * Revision 3.8  2000/05/12  14:41:46  schwarz
 * On behalf of Adrien, synchronisation with Mac sources, with some slight
 * changes because of cross-platform issues:
 * - Mac only stuff: XpSetFileAttribute XpFileSize
 * - Cross platform wrapper: XpGetenv XpExit
 * - Dangerous: strings.h (and thus bzero, bcopy) is not ANSI and thus doesn't
 *   exist on Mac.  Use string.h and memset, memcpy.
 *
 * Revision 3.7  2000/04/11  14:31:41  schwarz
 * Read/write NVT as frame with 1 text matrix, conforming to SDIF spec.
 * Read rest of general header chunk (might contain additional data).
 *
 * Revision 3.6  2000/03/01  11:19:57  schwarz
 * Assert Padding, added SdifFReadAndIgnore.
 * SdiffSetPos checks for pipe and then uses SdifFReadAndIgnore to seek forward.
 *
 * Revision 3.5  1999/11/03  16:42:32  schwarz
 * Use _SdifNVTStreamID for stream ID of 1NVT frames because of CNMAT
 * restriction of only one frame type per stream.
 * (See SdifNameValuesLNewTable)
 *
 * Revision 3.4  1999/10/15  12:28:43  schwarz
 * Updated writing of types and stream-id chunks to frames.
 * No time parameter for name value tables and stream ID tables, since
 * this decision is better left to the library.  (It uses the _SdifNoTime
 * constant, which happens to be _Sdif_MIN_DOUBLE_.)
 *
 * Revision 3.3  1999/10/13  16:05:41  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
 * Revision 3.2  1999/09/28  13:08:54  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:56:43  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:43  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:27  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 */


#include <preincluded.h>
#include "SdifFRead.h"
#include "SdifTest.h"
#include "SdifFile.h"
#include "SdifGlobals.h"
#include "SdifRWLowLevel.h"

#include "SdifNameValue.h"
#include "SdifHash.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifErrMess.h"

#include "SdifTimePosition.h"

#include "SdifFGet.h"

#include <assert.h>
#include <errno.h>




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
  size_t SizeS = 0;

  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));

  SdifFGetSignature(SdifF, &SizeR);
  SizeS = SizeR += SdifFReadChunkSize(SdifF);
  SizeR += SdiffReadUInt4 (&(SdifF->FormatVersion), 1, SdifF->Stream) * sizeof(SdifUInt4);
  SizeR += SdiffReadUInt4 (&(SdifF->TypesVersion),  1, SdifF->Stream) * sizeof(SdifUInt4);
  
  if (SdifF->CurrSignature != eSDIF)
    {
      sprintf(gSdifErrorMess, "%s not correctly read", 
	      SdifSignatureToString(eSDIF));
      _SdifFError(SdifF, eBadHeader, gSdifErrorMess);
    }

  /* read rest of header chunk (might contain additional data) */
  SdifFReadPadding (SdifF, SdifF->ChunkSize - (SizeR - SizeS));

  if (SdifF->FormatVersion != _SdifFormatVersion)
  {
      char *mfmt = SdifF->FormatVersion > _SdifFormatVersion
	? "file is in a newer SDIF format version (%d) than the library (%d)"
	: "File is in an old SDIF format version (%d).  "
	  "The library (version %d) is not backwards compatible.";

      sprintf (gSdifErrorMess, mfmt, SdifF->FormatVersion, _SdifFormatVersion);
      _SdifFError(SdifF, eBadFormatVersion, gSdifErrorMess);
    }
    
  return SizeR;
}



size_t
SdifFReadNameValueLCurrNVT(SdifFileT *SdifF)
{
  /* Signature of chunck already read and checked for 1NVT */
  size_t SizeR = 0;
  
  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));
  SdifF->StartChunkPos -= sizeof(SdifSignature);

#if (_SdifFormatVersion >= 3)	
  /* read frame header */
  SizeR += SdifFReadFrameHeader (SdifF);
  SdifF->ChunkSize = SdifF->CurrFramH->Size;
#else
  SizeR += SdifFReadChunkSize(SdifF);
#endif
  SizeR += SdifFGetNameValueLCurrNVT(SdifF, 's');
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

/*************************************************************************************/
/* THE FOLLOWING FUNCTION DOESN'T TAKE CARE ANYMORE ABOUT THE OLD SDIF SPECIFICATION */
/*************************************************************************************/

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
  
  SizeR += SdifFReadFrameHeader (SdifF);
  SdifF->ChunkSize = SdifF->CurrFramH->Size;

  if (SdifF->CurrFramH && SdifFCurrNbMatrix(SdifF))
    /* For the new version of SDIF specification (June 1999) */
    {
      SdifStringT *SdifString;

      SdifString = SdifStringNew();
      
      SizeR += SdifFReadTextMatrix(SdifF, SdifString);
      SizeR += SdifFGetAllTypefromSdifString(SdifF, SdifString);

      SdifStringFree(SdifString);
    }
  else
    {
      SizeR += SdifFGetAllType(SdifF, 's');
    }
  
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




/*************************************************************************************/
/* THE FOLLOWING FUNCTION DOESN'T TAKE CARE ANYMORE ABOUT THE OLD SDIF SPECIFICATION */
/*************************************************************************************/
  
/* SdifFReadAllStreamID ne lit pas "SSIC" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "SSIC"
 */
size_t
SdifFReadAllStreamID(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  
  SdiffGetPos(SdifF->Stream, &(SdifF->StartChunkPos));
  SdifF->StartChunkPos -= sizeof(SdifSignature);
  
  SizeR += SdifFReadFrameHeader (SdifF);
  SdifF->ChunkSize = SdifF->CurrFramH->Size;

  if (SdifF->CurrFramH && SdifFCurrNbMatrix(SdifF))
    {
      /* For the new version of SDIF specification (June 1999) */
      SdifStringT *SdifString;

      SdifString = SdifStringNew();

      SizeR += SdifFReadTextMatrix(SdifF, SdifString);
      SizeR += SdifFGetAllStreamIDfromSdifString(SdifF, SdifString);

      SdifStringFree(SdifString);
    }
  else
    {
      SizeR += SdifFGetAllStreamID(SdifF, 's');
    }
  
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
	  SdifNameValuesLNewTable(SdifF->NameValues, _SdifNVTStreamID);
	  SizeR += SdifFReadNameValueLCurrNVT(SdifF);
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
    /* case template for type from SdifDataTypeET */
#   define readrowcase(type) \
    case e##type:  return (sizeof (Sdif##type) *			  \
			   SdiffRead##type (SdifF->CurrOneRow->Data.type, \
					    SdifF->CurrOneRow->NbData,    \
					    SdifF->Stream));

    switch (SdifF->CurrOneRow->DataType)
    {
        /* generate cases for all types */
	sdif_foralltypes (readrowcase)

	default :
	    sprintf(gSdifErrorMess, "OneRow 0x%04x, then Float4 used", 
		    SdifF->CurrOneRow->DataType);
	    _SdifFError(SdifF, eTypeDataNotSupported, gSdifErrorMess);
	    return (sizeof(SdifFloat4) * 
		    SdiffReadFloat4(SdifF->CurrOneRow->Data.Float4,
				    SdifF->CurrOneRow->NbData,
				    SdifF->Stream));
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
    assert (Padding <= _SdifStringLen);
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



/* read and throw away bytes data. */
size_t
SdifFReadAndIgnore (SdifFileT *SdifF, size_t bytes)
{
    size_t numread = 0, portion = _SdifStringLen;

    while (bytes > 0  &&  portion)
    {
	portion  = SdifFReadPadding (SdifF, MIN (bytes, _SdifStringLen));
	numread += portion;
	bytes   -= portion;
    }
    return (numread);
}



/* SdifFSkipMatrix read entire matrix header. */
size_t
SdifFSkipMatrix(SdifFileT *SdifF)
{
  size_t
    SizeR = 0;
  
  SizeR =  SdifFReadMatrixHeader(SdifF);
  SizeR += SdifFSkipMatrixData(SdifF);
  return SizeR;
}

/* obsolete */
size_t SdifSkipMatrix(SdifFileT *SdifF)
{
    _Debug("SdifSkipMatrix is obsolete, use SdifFSkipMatrix");
    return SdifFSkipMatrix(SdifF);
}


/* SdifFSkipMatrixData don't read matrix header. */
size_t
SdifFSkipMatrixData(SdifFileT *SdifF)
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
  {
#if HAVE_ERRNO_H
      if (errno == ESPIPE)
      {	  /* second chance: SdiffSetPos didn't work because we're
	  reading from a pipe.  Instead of making the whole thing
	  explode, we do the little extra work to read and throw away
	  the data. */
	  return (SdifFReadAndIgnore (SdifF, NbBytesToSkip));
      }
      else
#endif
	  return (size_t) -1;
  }
  else
  {
      SizeR += NbBytesToSkip;
      return SizeR;
  }
}

/* obsolete */
size_t SdifSkipMatrixData(SdifFileT *SdifF)
{
    _Debug("SdifSkipMatrixData is obsolete, use SdifFSkipMatrixData");
    return SdifFSkipMatrixData(SdifF);
}


/* Cette fonction à le même sens que SdifSkipMatrixData mais pour les
   frames. Il faut donc pour l'utiliser avoir au préalable lu la
   signature et l'entête.  */
size_t SdifFSkipFrameData(SdifFileT *SdifF)
{
    size_t    SizeR = 0, Boo, NbBytesToSkip;
    SdifUInt4 iMtrx;
    SdiffPosT Pos;
    
    if (SdifF->CurrFramH->Size != _SdifUnknownSize)
    {
	NbBytesToSkip = SdifF->CurrFramH->Size - _SdifFrameHeaderSize;
	SdiffGetPos(SdifF->Stream, &Pos);
	Pos += NbBytesToSkip;
	if (SdiffSetPos(SdifF->Stream, &Pos) != 0)
	{
	    sprintf(gSdifErrorMess, "Skip FrameData %s ID:%u T:%g\n",
		    SdifSignatureToString(SdifF->CurrFramH->Signature),
		    SdifF->CurrFramH->NumID, SdifF->CurrFramH->Time);
	    _SdifError(eEof, gSdifErrorMess);
	    return ((size_t) -1);
	}
	else
	    return (NbBytesToSkip);    
    }
    else
    {
	for (iMtrx = 0; iMtrx<SdifF->CurrFramH->NbMatrix; iMtrx++)
	{
	    Boo = SdifFSkipMatrix(SdifF);
	    if (Boo == -1)
	    {
		sprintf(gSdifErrorMess,
			"Skip Matrix %d in FrameData %s ID:%u T:%g\n", iMtrx, 
			SdifSignatureToString(SdifF->CurrFramH->Signature),
			SdifF->CurrFramH->NumID, SdifF->CurrFramH->Time);
		_SdifError(eEof, gSdifErrorMess);
		return ((size_t) -1);
	    }
	    else
		SizeR += Boo;
	}
	return SizeR;
    }
}

/* obsolete */
size_t SdifSkipFrameData(SdifFileT *SdifF)
{
    _Debug("SdifSkipFrameData is obsolete, use SdifFSkipFrameData");
    return SdifFSkipFrameData(SdifF);
}


/*DOC:
  Function to read text matrix.
  Read header.
  Call SdifFReadTextMatrixData.
  Read padding.
*/
size_t SdifFReadTextMatrix(SdifFileT *SdifF, SdifStringT *SdifString)
{
  size_t SizeR = 0;
  FILE *file;

  file = SdifF->Stream;
  
  SizeR += SdifFReadMatrixHeader (SdifF);
  SizeR += SdifFReadTextMatrixData(SdifF, SdifString);

  /* Number of bytes written */
  SizeR += SdifFReadPadding(SdifF, SdifFPaddingCalculate (file,
					 SizeR + sizeof(SdifSignature)));
  return SizeR;
}



/*DOC:
  Function to read text matrix data.
  Make reallocation.
  Read data.
*/
size_t SdifFReadTextMatrixData(SdifFileT *SdifF, SdifStringT *SdifString)
{
  int nrow;
  char *str;
  size_t SizeR = 0;
  int success = 1;
  FILE *file;

  file = SdifF->Stream;
  
  nrow = SdifFCurrNbRow(SdifF);
  str = SdifCalloc(char, nrow * SdifFCurrNbCol(SdifF));
  SizeR += SdiffReadChar(str, nrow, file);

  /* Append string in SdifString */
  success = SdifStringAppend(SdifString, str);
  
  SdifFree(str);
  return SizeR;
}



/*
 * obsolete
 */

size_t
SdifFReadNameValueCurrHT(SdifFileT *SdifF)
{
    /* obsolete */
    _Debug("SdifFReadNameValueCurrHT is obsolete, use SdifFReadNameValueLCurrNVT");
    return SdifFReadNameValueLCurrNVT(SdifF);
}


