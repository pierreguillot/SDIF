/* $Id: SdifTextConv.c,v 2.4 1999-02-28 12:16:56 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * Convert a pseudo-sdif text file to a sdif file
 *
 * author: Dominique Virolle 1997
 *
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.3  1999/01/23  15:56:02  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:48  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 */


#include "SdifTextConv.h"
#include "SdifFile.h"
#include "SdifTest.h"

#include "SdifRWLowLevel.h"

#include "SdifFScan.h"
#include "SdifFWrite.h"
#include "SdifErrMess.h"

#include <stdlib.h>


size_t
SdifFTextConvMatrixData(SdifFileT *SdifF)
{
  size_t
    SizeW = 0;
  SdifUInt4
    iRow;

  if ( (SdifFCurrNbCol(SdifF) > 0) && (SdifFCurrNbRow(SdifF) > 0) )
    {      
      for(iRow=0; iRow<SdifFCurrNbRow(SdifF); iRow++)
        {
	      SdifFScanOneRow(SdifF);
	      SizeW += SdifFWriteOneRow(SdifF);
          if (SdifFLastErrorTag (SdifF)  != eNoError)
            return SizeW;
	    }
      
      return SizeW;
    }
  else
    return 0; /* empty matrix */
}








size_t
SdifFTextConvMatrix(SdifFileT *SdifF)
{
  size_t SizeW = 0;

  SdifFScanMatrixHeader(SdifF);
  if (SdifFLastErrorTag (SdifF)  != eNoError)
     return SizeW;

  SizeW += SdifFWriteMatrixHeader(SdifF);
  SizeW += SdifFTextConvMatrixData(SdifF);
  SizeW += SdifFWritePadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeW));
  
  return SizeW;
}









size_t
SdifFTextConvFrameData(SdifFileT *SdifF)
{
  size_t  SizeW = 0;
  SdifUInt4  iMtrx;

  for(iMtrx=1; iMtrx<=SdifFCurrNbMatrix(SdifF); iMtrx++)
    {
      SizeW += SdifFTextConvMatrix(SdifF);
      if (SdifFLastErrorTag (SdifF)  != eNoError)
        return SizeW;
    }
  
  return SizeW;
}





size_t
SdifFTextConvFrameHeader(SdifFileT *SdifF)
{
  size_t  SizeW = 0;

  SdifFScanFrameHeader(SdifF);
  SdifF->CurrFramH->Size  = _SdifUnknownSize;

  SizeW = SdifFWriteFrameHeader(SdifF);

  return SizeW;
}





size_t
SdifFTextConvFrame(SdifFileT *SdifF)
{
  size_t SizeW;
  
  SizeW = SdifFTextConvFrameHeader(SdifF);
  if (SdifFLastErrorTag (SdifF)  != eNoError)
    return SizeW;
  SizeW += SdifFTextConvFrameData(SdifF);
  if (SdifFLastErrorTag (SdifF)  != eNoError)
    return SizeW;
  SdifF->CurrFramH->Size = (SdifUInt4) SizeW;

  SdifUpdateChunkSize(SdifF, SizeW - sizeof(SdifSignature) - sizeof(SdifInt4));
  
  return SdifF->CurrFramH->Size;
}








size_t
SdifFTextConvAllFrame(SdifFileT *SdifF)
{
  size_t
    SizeR = 0,
    SizeW = 0;
  int
    CharEnd = 0;

  while ((CharEnd != eEof) && (SdifFCurrSignature(SdifF) != eENDC))
    {
      SizeW += SdifFTextConvFrame(SdifF);
      if (SdifFLastErrorTag (SdifF)  != eNoError)
         return SizeW;
      SdifFCleanCurrSignature(SdifF);
      CharEnd = SdiffGetSignature (SdifF->TextStream, &(SdifF->CurrSignature), &SizeR);
    }

  if (CharEnd == eEof)
    _SdifError(eEof, gSdifErrorMess);

  return SizeW;
}








size_t
SdifFTextConvFramesChunk(SdifFileT *SdifF)
{
  size_t
    SizeW = 0,
    SizeR = 0;  

  SdiffGetSignature(SdifF->TextStream, &(SdifF->CurrSignature), &SizeR);
  SdifF->ChunkSize = SdifFTextConvAllFrame(SdifF);
  SizeW += SdifF->ChunkSize;
  if (SdifFLastErrorTag (SdifF)  != eNoError)
     return SizeW;

  if (SdifFCurrSignature(SdifF) ==  eENDC)
    /*SdifUpdateChunkSize(SdifF, SdifF->ChunkSize)
     */
      ;
  else
    _SdifFError(SdifF, eSyntax, "Attempt to read 'ENDC' failed");
  
  return SizeW;
}








size_t
SdifFTextConv(SdifFileT *SdifF)
{
  size_t
    SizeW = 0,
    SizeR = 0; /* only here to assume the third argument of SdiffGetName */
    
  SdifFScanGeneralHeader(SdifF);  
  
  SdifF->FileSize = 0;
  SdifF->FileSize += SdifFWriteGeneralHeader(SdifF);

  SdifFScanAllASCIIChunks(SdifF);
  SdifF->FileSize += SdifFWriteAllASCIIChunks(SdifF);

  switch (SdifFCurrSignature(SdifF))
    {
    case eSDFC:
      SizeW = SdifFTextConvFramesChunk(SdifF);
      SdifF->FileSize += SizeW;
      break;

    case eENDF:
      return SdifF->FileSize;

    default:
      sprintf(gSdifErrorMess,
	      "It is not a chunk signature : '%s'",
	      SdifSignatureToString(SdifFCurrSignature(SdifF)));
      _SdifFError(SdifF, eSyntax, gSdifErrorMess);
      break;
    }
    
  if (SdifFLastErrorTag (SdifF)  != eNoError)
      return SizeW;
  if (SdifFCurrSignature(SdifF) != eENDC)
    _SdifFError(SdifF, eSyntax, "Attempt to read 'ENDF' failed");
  
  return SdifF->FileSize;
}








/* upper level : open the text in read mode */
size_t
SdifTextToSdif(SdifFileT *SdifF, char *TextStreamName)
{
  size_t
    FileSizeW = 0;
  

  if (SdifF->Mode != eWriteFile)
    _SdifFError(SdifF, eBadMode, "it must be eWriteFile");

  if (SdifF->TextStream)
    {
      SdiffBinClose(SdifF->TextStream);
      if (SdifF->TextStreamName)
	  SdifFree(SdifF->TextStreamName);
      _SdifRemark("TextStream Re-initialisation\n");
    }
  
  if (SdifStrCmp(TextStreamName, SdifF->Name) == 0)
    {
      sprintf(gSdifErrorMess, "Read=%s, Write=%s.", TextStreamName, SdifF->Name);
      _SdifFError(SdifF, eReadWriteOnSameFile, gSdifErrorMess);
      return FileSizeW;
    }
  else
    {
      SdifFOpenText(SdifF, TextStreamName, eReadFile);
      if (! SdifF->TextStream)
	    {
	      return  FileSizeW;
	    }
      else
        {
          FileSizeW = SdifFTextConv(SdifF);
          fflush(SdifF->Stream);
          return FileSizeW;
        }
    }
  
  /*  return FileSizeW; */
}

