/* $Id: SdifFScan.c,v 3.9 2000-08-21 10:02:49 tisseran Exp $
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
 * Revision 3.8  2000/05/15  16:25:55  schwarz
 * cc on alpha doesn't like empty cases.
 *
 * Revision 3.7  2000/05/15  16:23:08  schwarz
 * Avoided avoidable warnings.
 *
 * Revision 3.6  2000/03/01  11:17:35  schwarz
 * Backwards compatibility of data types for reading text.
 *
 * Revision 3.5  1999/11/03  16:42:33  schwarz
 * Use _SdifNVTStreamID for stream ID of 1NVT frames because of CNMAT
 * restriction of only one frame type per stream.
 * (See SdifNameValuesLNewTable)
 *
 * Revision 3.4  1999/10/15  12:27:51  schwarz
 * No time parameter for name value tables and stream ID tables, since
 * this decision is better left to the library.  (It uses the _SdifNoTime
 * constant, which happens to be _Sdif_MIN_DOUBLE_.)
 *
 * Revision 3.3  1999/10/13  16:05:42  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
 * Revision 3.2  1999/09/28  13:08:55  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:56:45  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:45  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:28  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 */


#include <preincluded.h>
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
	  SdifNameValuesLNewTable(SdifF->NameValues, _SdifNVTStreamID);
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
  SdifInt4 DataType;
  
  SdifFCreateCurrMtrxH(SdifF); /* create only if it's necessary */
  
  SdiffGetSignature(SdifF->TextStream, &(SdifF->CurrMtrxH->Signature), &SizeR);
  fscanf(SdifF->TextStream, "%i", &DataType);

  /* when DataType was 32 for Float4 at Ircam and others */
  switch ((SdifDataTypeET) (int) DataType)
  {
      case eFloat4a:
      case eFloat4b:
          DataType = eFloat4;
      break;

      case eFloat8a:
      case eFloat8b:
	  DataType = eFloat8;
      break;
      default: /* all is fine! */;
  }

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
    /* case template for type from SdifDataTypeET */
#   define scanrowcase(type)						  \
    case e##type:  							  \
        SdiffScan##type (SdifF->TextStream, SdifF->CurrOneRow->Data.type, \
			 SdifF->CurrOneRow->NbData);			  \
    break;

    switch (SdifF->CurrOneRow->DataType)
    {
        /* generate cases for all types */
	sdif_foralltypes (scanrowcase)

	default :
	    sprintf (gSdifErrorMess, "in text file, OneRow 0x%04x, then Float4 used", SdifF->CurrOneRow->DataType);
	    _SdifFError(SdifF, eTypeDataNotSupported, gSdifErrorMess);
	    SdiffScanFloat4(SdifF->TextStream, SdifF->CurrOneRow->Data.Float4,
			    SdifF->CurrOneRow->NbData);
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


