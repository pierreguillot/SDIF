/* $Id: SdifConvToText.c,v 3.3.2.1 2000-08-21 21:34:56 tisseran Exp $
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
 * functions to convert an sdif file into a pseudo-sdif text file.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.3  2000/05/12  14:41:42  schwarz
 * On behalf of Adrien, synchronisation with Mac sources, with some slight
 * changes because of cross-platform issues:
 * - Mac only stuff: XpSetFileAttribute XpFileSize
 * - Cross platform wrapper: XpGetenv XpExit
 * - Dangerous: strings.h (and thus bzero, bcopy) is not ANSI and thus doesn't
 *   exist on Mac.  Use string.h and memset, memcpy.
 *
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
