/* $Id: SdifFPrint.c,v 3.7 2001-05-02 09:34:41 tisseran Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
 *
 *
 * F : SdifFileT* SdifF, Print : write only into the pseudo-sdif text file, SdifF->TextStream.
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.6  2000/11/15 14:53:26  lefevre
 * no message
 *
 * Revision 3.5  2000/10/27  20:03:27  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.4.2.1  2000/08/21  21:35:05  tisseran
 * *** empty log message ***
 *
 * Revision 3.4  2000/04/11  13:24:38  schwarz
 * Fixed ancient bug in ...AllAsciiChunks: SdifExistUserFrameType worked
 * on SdifF->MatrixTypesTable, not SdifF->FrameTypesTable.
 *
 * Revision 3.3  1999/10/13  16:05:40  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
 * Revision 3.2  1999/09/28  13:08:52  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:56:39  virolle
 * SdifStdErr add
 *
 *
 */


#include <preincluded.h>

#include "SdifFPrint.h"
#include "SdifTest.h"
#include "SdifFile.h"
#include "SdifRWLowLevel.h"
#include "SdifNameValue.h"
#include "SdifHash.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifFPut.h"
#include "SdifErrMess.h"
#include "SdifTimePosition.h"






size_t
SdifFPrintGeneralHeader(SdifFileT *SdifF)
{
  size_t SizeW = 0;
  
  SizeW += fprintf(SdifF->TextStream, "%s\n\n", SdifSignatureToString(eSDIF));
  return SizeW;
}




size_t
SdifFPrintNameValueLCurrNVT(SdifFileT *SdifF)
{
  size_t SizeW = 0;

  SizeW += fprintf(SdifF->TextStream, "%s\n", SdifSignatureToString(e1NVT));
  SizeW += SdifFPutNameValueLCurrNVT (SdifF, 't');
  SizeW += fprintf(SdifF->TextStream, "\n");
  return SizeW;
}




size_t
SdifFPrintAllNameValueNVT(SdifFileT *SdifF)
{
    size_t SizeW = 0;

    SdifListInitLoop(SdifF->NameValues->NVTList);
    while (SdifListIsNext(SdifF->NameValues->NVTList))
    {
        SdifF->NameValues->CurrNVT = SdifListGetNext(SdifF->NameValues->NVTList);
        SizeW += SdifFPrintNameValueLCurrNVT (SdifF);
    }
    return SizeW;
}



size_t
SdifFPrintMatrixType(SdifFileT *SdifF, SdifMatrixTypeT *MatrixType)
{
  return SdifFPutOneMatrixType(SdifF, 't', MatrixType);
}




size_t
SdifFPrintFrameType(SdifFileT *SdifF, SdifFrameTypeT *FrameType)
{
  return SdifFPutOneFrameType(SdifF, 't', FrameType);
}




size_t
SdifFPrintAllType(SdifFileT *SdifF)
{
  size_t SizeW = 0;
  
  if ((SdifF->TypeDefPass == eNotPass) || (SdifF->TypeDefPass == eReadPass))
    {      
      SizeW += fprintf(SdifF->TextStream, "%s\n", SdifSignatureToString(e1TYP));
      SizeW += SdifFPutAllType(SdifF, 't');
      SizeW += fprintf(SdifF->TextStream, "\n");

      SdifF->TypeDefPass = eWritePass;
    }
  else
    _SdifFError(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1TYP));

  return SizeW;
}





size_t
SdifFPrintAllStreamID(SdifFileT *SdifF)
{
  size_t SizeW = 0;

  if ((SdifF->StreamIDPass == eNotPass) || (SdifF->StreamIDPass == eReadPass))
    {      
      SizeW += fprintf(SdifF->TextStream, "%s\n", SdifSignatureToString(e1IDS));
      SizeW += SdifFPutAllStreamID(SdifF, 't');
      SizeW += fprintf(SdifF->TextStream, "\n");

      SdifF->StreamIDPass = eWritePass;
    }
  else
    _SdifFError(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1IDS));
      
  return SizeW;
}





size_t
SdifFPrintAllASCIIChunks(SdifFileT *SdifF)
{
  size_t SizeW = 0;

  if (SdifNameValuesLIsNotEmpty(SdifF->NameValues))
    {
      SizeW += SdifFPrintAllNameValueNVT(SdifF);
      SizeW += fprintf(SdifF->TextStream, "\n");
    }

  if (   (SdifExistUserMatrixType(SdifF->MatrixTypesTable))
      || (SdifExistUserFrameType(SdifF->FrameTypesTable)) )
    {
      SizeW += SdifFPrintAllType (SdifF);
      SizeW += fprintf(SdifF->TextStream, "\n");
    }

  if (SdifStreamIDTableGetNbData  (SdifF->StreamIDsTable) > 0)
    {
      SizeW += SdifFPrintAllStreamID (SdifF);
      SizeW += fprintf(SdifF->TextStream, "\n");
    }

  return SizeW;
}





size_t
SdifFPrintMatrixHeader(SdifFileT *SdifF)
{
  size_t
    SizeW = 0;

  SizeW += fprintf(SdifF->TextStream, "  %s\t0x%04x\t%u\t%u\n",
		   SdifSignatureToString(SdifF->CurrMtrxH->Signature),
		   SdifF->CurrMtrxH->DataType,
		   SdifF->CurrMtrxH->NbRow,
		   SdifF->CurrMtrxH->NbCol);

  return SizeW;
}






size_t
SdifFPrintOneRow(SdifFileT *SdifF)
{
  unsigned int
    iCol;
  size_t
    SizeW = 0;
  
  switch (SdifF->CurrOneRow->DataType)
    {
    case eFloat8 :
      for(iCol=0; iCol<SdifF->CurrOneRow->NbData; iCol++)
	SizeW += fprintf(SdifF->TextStream, "\t%g", SdifF->CurrOneRow->Data.Float8[iCol]);
      break;
    case eFloat4 :
      for(iCol=0; iCol<SdifF->CurrOneRow->NbData; iCol++)
	SizeW += fprintf(SdifF->TextStream, "\t%g", SdifF->CurrOneRow->Data.Float4[iCol]);
      break;
    default :
      sprintf(gSdifErrorMess, "OneRow 0x%04x, then Float4 used", SdifF->CurrOneRow->DataType);
      _SdifFError(SdifF, eTypeDataNotSupported, gSdifErrorMess);
      /* then values are considered as Float4 */
      for(iCol=0; iCol<SdifF->CurrOneRow->NbData; iCol++)
	SizeW += fprintf(SdifF->TextStream, "\t%g", SdifF->CurrOneRow->Data.Float4[iCol]);
      break;
    }
  SizeW += fprintf(SdifF->TextStream, "\n");

  return SizeW;
}






size_t
SdifFPrintFrameHeader(SdifFileT *SdifF)
{
  size_t
    SizeW = 0;
  
  SizeW += fprintf(SdifF->TextStream,
		   "%s\t%u\t%u\t%g\n",
		   SdifSignatureToString(SdifF->CurrFramH->Signature),  
		   SdifF->CurrFramH->NbMatrix,
		   SdifF->CurrFramH->NumID,
		   SdifF->CurrFramH->Time);

  return SizeW;
}



/*
 * obsolete
 */


size_t
SdifFPrintNameValueCurrHT(SdifFileT *SdifF)
{
    /* obsolete */
    _Debug("SdifFPrintNameValueCurrHT is obsolete, use SdifFPrintNameValueLCurrNVT");
    return SdifFPrintNameValueLCurrNVT(SdifF);
}



size_t
SdifFPrintAllNameValueHT(SdifFileT *SdifF)
{
    /* obsolete */
    _Debug("SdifFPrintAllNameValueHT is obsolete, use SdifFPrintAllNameValueNVT");
    return SdifFPrintAllNameValueNVT(SdifF);
}


