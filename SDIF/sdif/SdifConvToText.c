/* SdifConvToText.c
 *
 * functions to convert an sdif file into a pseudo-sdif text file.
 *
 * author: Dominique Virolle 1997
 *
 *
 */

#include "SdifConvToText.h"
#include "SdifFile.h"
#include "SdifTest.h"

#include "SdifRWLowLevel.h"

#include "SdifFGet.h"
#include "SdifFRead.h"
#include "SdifFPrint.h"

#include <stdlib.h>





size_t
SdifFConvToTextMatrixData(SdifFileT *SdifF)
{
  size_t
    SizeR = 0;
  SdifUInt4
    iRow;

  if ( (SdifCurrNbCol(SdifF) > 0) && (SdifCurrNbRow(SdifF) > 0) )
    {
      for(iRow=0; iRow<SdifCurrNbRow(SdifF); iRow++)
	{
	  SizeR += SdifFReadOneRow(SdifF);
	  SdifFPrintOneRow(SdifF);
	}
      
      return SizeR;
    }
  else
    return 0; /* empty matrix */
}







size_t
SdifFConvToTextMatrixHeader(SdifFileT *SdifF)
{
  size_t SizeR = 0;

  SizeR += SdifFReadMatrixHeader(SdifF);
  SdifTestMatrixHeader(SdifF);
  SdifFPrintMatrixHeader(SdifF);

  return SizeR;
}









size_t
SdifFConvToTextMatrix(SdifFileT *SdifF)
{
  size_t SizeR = 0;

  SizeR += SdifFConvToTextMatrixHeader(SdifF);
  SizeR += SdifFConvToTextMatrixData(SdifF);
  SizeR += SdifFReadPadding(SdifF, SdifFPaddingCalculate(SdifF->Stream, SizeR));

  return SizeR;
}









size_t
SdifFConvToTextFrameData(SdifFileT *SdifF)
{
  size_t SizeR = 0;
  SdifUInt4  iMtrx;

  for(iMtrx=1; iMtrx<=SdifCurrNbMatrix(SdifF); iMtrx++)
    {
      SizeR += SdifFConvToTextMatrix(SdifF);
      SdifTestFrameWithMatrixHeader(SdifF, iMtrx);
    }
  
  return SizeR;
}




size_t
SdifFConvToTextFrameHeader(SdifFileT *SdifF)
{
  size_t SizeR = 0;

  SizeR += SdifFReadFrameHeader(SdifF);

  fprintf(SdifF->TextStream, "\n"); /* only for presentation */
  SdifFPrintFrameHeader(SdifF);

  return SizeR;
}








size_t
SdifFConvToTextFrame(SdifFileT *SdifF)
{
  size_t SizeR = 0;

  SizeR += SdifFConvToTextFrameHeader(SdifF);
  SizeR += SdifFConvToTextFrameData(SdifF);

  return SizeR;
}








size_t
SdifFConvToTextAllFrame(SdifFileT *SdifF)
{
  size_t
    SizeRSign = 0,
    SizeR = 0;
  int CharEnd = 0;

  while (CharEnd != eEof)
    {
      SizeR += SdifFConvToTextFrame(SdifF);
      SdifCleanCurrSignature(SdifF);
      CharEnd = SdifFGetSignature(SdifF, &SizeRSign);
      if (CharEnd != eEof)
	{
	  SizeR += SizeRSign;
	  SizeRSign = 0;
	}
    }

  return SizeR;
}







size_t
SdifFConvToText(SdifFileT *SdifF)
{
  size_t  SizeR = 0;
  

  SizeR += SdifFReadGeneralHeader(SdifF);
  SdifFPrintGeneralHeader(SdifF);

  SizeR += SdifFReadAllASCIIChunks(SdifF);
  SdifFPrintAllASCIIChunks(SdifF);

  if (SdifCurrSignature(SdifF) != eEmptySignature)
    {
      fprintf(SdifF->TextStream, "\n%s\n", SdifSignatureToString(eSDFC));
      SizeR += SdifFConvToTextAllFrame(SdifF);
      fprintf(SdifF->TextStream, "\n%s\n", SdifSignatureToString(eENDC));
    }
      
  fprintf(SdifF->TextStream, "%s\n", SdifSignatureToString(eENDF));

  return SizeR;
}








/* upper level : open the text in write mode */
size_t
SdifToText(SdifFileT *SdifF, char *TextStreamName)
{
  size_t  SizeR = 0;
  
  if (SdifF->Mode != eReadFile)
    _SdifFileMess(SdifF, eBadMode, "it must be eReadFile");

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
      sprintf(gSdifErrorMess, "Write=%s, Read=%s.", SdifF->TextStreamName, SdifF->Name);
      _SdifFileMess(SdifF, eReadWriteOnSameFile, gSdifErrorMess);
      return SizeR;
    }
  else
    if (SdifStrCmp(TextStreamName, "stdin")==0)
      _SdifFileMess(SdifF, eBadStdFile, "write on stdin forbidden");
    else
      if (SdifStrCmp(TextStreamName, "stdout")==0)
	SdifF->TextStream = stdout;
      else
	if (! (SdifF->TextStream = fopen(SdifF->TextStreamName, "w")) )
	  {
	    _SdifError(eFileNotFound, TextStreamName);
	    free(SdifF->TextStreamName);
	    return  SizeR;
	  }

  if ( (SdifF->TextStream) && (SdifF->Stream) )
    {
      SizeR = SdifFConvToText(SdifF);
      fflush(SdifF->TextStream);
    }
  
  return SizeR;
}
