/* SdifTextConv.c
 *
 * Convert a pseudo-sdif text file to a sdif file
 *
 * author: Dominique Virolle 1997
 *
 *
 */


#include "SdifTextConv.h"
#include "SdifFile.h"
#include "SdifTest.h"

#include "SdifRWLowLevel.h"

#include "SdifFScan.h"
#include "SdifFWrite.h"

#include <stdlib.h>


size_t
SdifFTextConvMatrixData(SdifFileT *SdifF)
{
  size_t
    SizeW = 0;
  SdifUInt4
    iRow;

  if ( (SdifCurrNbCol(SdifF) > 0) && (SdifCurrNbRow(SdifF) > 0) )
    {      
      for(iRow=0; iRow<SdifCurrNbRow(SdifF); iRow++)
	{
	  SdifFScanOneRow(SdifF);
	  SizeW += SdifFWriteOneRow(SdifF);
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
  SdifTestMatrixHeader(SdifF);

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

  for(iMtrx=1; iMtrx<=SdifCurrNbMatrix(SdifF); iMtrx++)
    {
      SizeW += SdifFTextConvMatrix(SdifF);
      SdifTestFrameWithMatrixHeader(SdifF, iMtrx);
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
  SizeW += SdifFTextConvFrameData(SdifF);
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

  while ((CharEnd != eEof) && (SdifCurrSignature(SdifF) != eENDC))
    {
      SizeW += SdifFTextConvFrame(SdifF);
      SdifCleanCurrSignature(SdifF);
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

  if (SdifCurrSignature(SdifF) ==  eENDC)
    /*SdifUpdateChunkSize(SdifF, SdifF->ChunkSize)
     */
      ;
  else
    _SdifFileMess(SdifF, eSyntax, "Attempt to read 'ENDC' failed");
  
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

  switch (SdifCurrSignature(SdifF))
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
	      SdifSignatureToString(SdifCurrSignature(SdifF)));
      _SdifFileMess(SdifF, eSyntax, gSdifErrorMess);
      break;
    }
    
  if (SdifCurrSignature(SdifF) != eENDC)
    _SdifFileMess(SdifF, eSyntax, "Attempt to read 'ENDF' failed");
  
  return SdifF->FileSize;
}








/* upper level : open the text in read mode */
size_t
SdifTextToSdif(SdifFileT *SdifF, char *TextStreamName)
{
  size_t
    FileSizeW = 0;
  

  if (SdifF->Mode != eWriteFile)
    _SdifFileMess(SdifF, eBadMode, "it must be eWriteFile");

  if (SdifF->TextStream)
    {
      fclose(SdifF->TextStream);
      if (SdifF->TextStreamName)
	free(SdifF->TextStreamName);
      _SdifRemark("TextStream Re-initialisation\n");
    }
  
  SdifF->TextStreamName = SdifCreateStrNCpy(TextStreamName, SdifStrLen(TextStreamName)+1);

  if (SdifStrCmp(SdifF->TextStreamName, SdifF->Name) == 0)
    {
      sprintf(gSdifErrorMess, "Read=%s, Write=%s.", SdifF->TextStreamName, SdifF->Name);
      _SdifFileMess(SdifF, eReadWriteOnSameFile, gSdifErrorMess);
      return FileSizeW;
    }
  else
    if (    (SdifStrCmp(TextStreamName, "stderr")==0)
	 || (SdifStrCmp(TextStreamName, "stdout")==0) )
      _SdifFileMess(SdifF, eBadStdFile, "read on stderr or stdout forbidden");
    else
      if (SdifStrCmp(TextStreamName, "stdin")==0)
	SdifF->TextStream = stdin;
      else
	if (! (SdifF->TextStream = fopen(SdifF->TextStreamName, "rb")) )
	  {
	    _SdifError(eFileNotFound, TextStreamName);
	    free(SdifF->TextStreamName);
	    return  FileSizeW;
	  }
    
  FileSizeW = SdifFTextConv(SdifF);
  fflush(SdifF->Stream);
  
  return FileSizeW;
}

