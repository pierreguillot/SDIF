/* $Id: SdifFRead.c,v 3.4 1999-10-15 12:28:43 schwarz Exp $
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
  SizeR += SdiffReadUInt4 (&(SdifF->FormatVersion), 1, SdifF->Stream) * sizeof(SdifUInt4);
  SizeR += SdiffReadUInt4 (&(SdifF->TypesVersion),  1, SdifF->Stream) * sizeof(SdifUInt4);
  
  if (SdifF->CurrSignature != eSDIF)
    {
      sprintf(gSdifErrorMess, "%s not correctly read", 
	      SdifSignatureToString(eSDIF));
      _SdifFError(SdifF, eBadHeader, gSdifErrorMess);
    }

  if (SdifF->FormatVersion != _SdifFormatVersion)
  {
      char *mfmt = SdifF->FormatVersion > _SdifFormatVersion
	? "file is in a newer format version (%d) than the library (%d)"
	: "File is in an old format version (%d).  "
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

#if (_SdifFormatVersion >= 3)	/* read frame header (with no matrices) */
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
  
#if (_SdifFormatVersion >= 3)	/* read frame header (with no matrices) */
  SizeR += SdifFReadFrameHeader (SdifF);
  SdifF->ChunkSize = SdifF->CurrFramH->Size;
#else
  SizeR += SdifFReadChunkSize(SdifF);
#endif
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
  
#if (_SdifFormatVersion >= 3)	/* read frame header (with no matrices) */
  SizeR += SdifFReadFrameHeader (SdifF);
  SdifF->ChunkSize = SdifF->CurrFramH->Size;
#else
  SizeR += SdifFReadChunkSize(SdifF);
#endif
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
	  SdifNameValuesLNewTable(SdifF->NameValues, _SdifNoStreamID);
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


