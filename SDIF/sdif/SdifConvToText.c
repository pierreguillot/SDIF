/* $Id: SdifConvToText.c,v 3.3 2000-05-12 14:41:42 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * functions to convert an sdif file into a pseudo-sdif text file.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.2  1999/09/28  13:08:48  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:56:31  virolle
 * SdifStdErr add
 *
 *
 */

#include <preincluded.h>
#include "XpGuiCalls.h"
#include "UniversalEnvVar.h"

#include "SdifConvToText.h"
#include "SdifFile.h"
#include "SdifTest.h"

#include "SdifRWLowLevel.h"

#include "SdifFGet.h"
#include "SdifFRead.h"
#include "SdifFPrint.h"

#include "SdifErrMess.h"

#include <stdlib.h>





size_t
SdifFConvToTextMatrixData(SdifFileT *SdifF)
{
  size_t
    SizeR = 0;
  SdifUInt4
    iRow;

  if ( (SdifFCurrNbCol(SdifF) > 0) && (SdifFCurrNbRow(SdifF) > 0) )
    {
      for(iRow=0; iRow<SdifFCurrNbRow(SdifF); iRow++)
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

  for(iMtrx=1; iMtrx<=SdifFCurrNbMatrix(SdifF); iMtrx++)
    {
      SizeR += SdifFConvToTextMatrix(SdifF);
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
  long fileSize;

  while (CharEnd != eEof)
    {
      SizeR += SdifFConvToTextFrame(SdifF);
      SdifFCleanCurrSignature(SdifF);
      CharEnd = SdifFGetSignature(SdifF, &SizeRSign);
      if (CharEnd != eEof)
      {  
	SizeR += SizeRSign;
	SizeRSign = 0;
      }
      fileSize = ftell(SdifF->Stream);
      XpProBarSet((float)fileSize);
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

  if (SdifFCurrSignature(SdifF) != eEmptySignature)
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
  size_t SizeR = 0;
  long   fileSize = 0;
  
  if (SdifF->Mode != eReadFile)
      _SdifFError(SdifF, eBadMode, "it must be eReadFile");

  if (SdifF->TextStream)
    {
      SdiffBinClose(SdifF->TextStream);
      if (SdifF->TextStreamName)
	free(SdifF->TextStreamName);
      _SdifRemark("TextStream Re-initialisation\n");
    }
  
  SdifF->TextStreamName = SdifCreateStrNCpy(TextStreamName, SdifStrLen(TextStreamName)+1);
  
  if (SdifStrCmp(SdifF->TextStreamName, SdifF->Name) == 0)
    {
      sprintf(gSdifErrorMess, "Write=%s, Read=%s.", SdifF->TextStreamName, SdifF->Name);
      _SdifFError(SdifF, eReadWriteOnSameFile, gSdifErrorMess);
      return SizeR;
    }
  else
    {
      SdifFOpenText(SdifF, TextStreamName, eWriteFile);
      if (! SdifF->TextStream)
	    {
	      return  SizeR;
	    }
      else
        {
          fileSize = XpFileSize(SdifF->Name);
          XpProBarString("Convert Sdif To Text");
          XpProBarInit((float)fileSize);

          SizeR = SdifFConvToText(SdifF);
          fflush(SdifF->TextStream);

          XpSetFileAttribute(TextStreamName, FileType_Text, 0);
	      return  SizeR;
        }
    }
  
  return SizeR;
}

