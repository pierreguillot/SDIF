/* $Id: SdifTextConv.c,v 3.3.2.1 2000-08-21 21:35:55 tisseran Exp $
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
 * Convert a pseudo-sdif text file to a sdif file
 *
 * author: Dominique Virolle 1997
 *
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.3  2000/05/12  14:41:49  schwarz
 * On behalf of Adrien, synchronisation with Mac sources, with some slight
 * changes because of cross-platform issues:
 * - Mac only stuff: XpSetFileAttribute XpFileSize
 * - Cross platform wrapper: XpGetenv XpExit
 * - Dangerous: strings.h (and thus bzero, bcopy) is not ANSI and thus doesn't
 *   exist on Mac.  Use string.h and memset, memcpy.
 *
 * Revision 3.2  1999/09/28  13:09:16  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:57:25  virolle
 * SdifStdErr add
 *
 * Revision 2.4  1999/02/28  12:16:56  virolle
 * memory report
 *
 * Revision 2.3  1999/01/23  15:56:02  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:48  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 */


#include <preincluded.h>
#include "XpGuiCalls.h"
#include "UniversalEnvVar.h"

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
  int CharEnd = 0;
  long fileSize;

  while ((CharEnd != eEof) && (SdifFCurrSignature(SdifF) != eENDC))
    {
      SizeW += SdifFTextConvFrame(SdifF);
      if (SdifFLastErrorTag (SdifF)  != eNoError)
         return SizeW;
      SdifFCleanCurrSignature(SdifF);
      CharEnd = SdiffGetSignature (SdifF->TextStream, &(SdifF->CurrSignature), &SizeR);

      fileSize = ftell(SdifF->TextStream);
      XpProBarSet((float)fileSize);
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
  size_t FileSizeW = 0;
  long   fileSize = 0;

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
          fileSize = XpFileSize(TextStreamName);
          XpProBarString("Convert Text To Sdif");
          XpProBarInit((float)fileSize);

          FileSizeW = SdifFTextConv(SdifF);
          fflush(SdifF->Stream);
 
          XpSetFileAttribute(SdifF->Name, FileType_Sdif, 0);
          return FileSizeW;
        }
    }
  
  /*  return FileSizeW; */
}

