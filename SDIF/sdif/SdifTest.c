/* $Id: SdifTest.c,v 3.7 2000-11-15 14:53:36 lefevre Exp $
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
 * SdifTest.c
 *
 * Tests with message on errors apply on a SdifFileT*
 * exit(1) is exceptional
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.6  2000/10/27  20:03:45  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.5  2000/08/22  13:38:41  schwarz
 * SdifFTestMatrixWithFrameHeader warns about additional undeclared
 * matrix in frame but do not signal an error, because we allow any
 * additional matrices in frames.
 * Replaced short by int, because that's faster and less error-prone.
 *
 * Revision 3.4.2.1  2000/08/21  21:35:53  tisseran
 * *** empty log message ***
 *
 * Revision 3.4  2000/04/11  14:32:09  schwarz
 * Don't return error, if more columns in matrix than known,
 * since then the wrong number of columns is read.
 *
 * Revision 3.3  1999/10/13  16:05:59  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
 * Revision 3.2  1999/09/28  13:09:14  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:57:23  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:56:01  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:48  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 *
 */

#include <preincluded.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "SdifTest.h"
#include "SdifPrint.h"
#include "SdifRWLowLevel.h"
#include "SdifFile.h"
#include "SdifErrMess.h"

#define _NbOfWarningMax 19





SdifMatrixTypeT*
SdifTestMatrixType(SdifFileT *SdifF, SdifSignature Signature)
{
  SdifMatrixTypeT* MtrxT;
  SdifMatrixTypeT* PredefinedMtrxT;

  MtrxT = SdifGetMatrixType(SdifF->MatrixTypesTable, Signature);
  if (MtrxT)
    return MtrxT;
  else
    {
      PredefinedMtrxT = SdifGetMatrixType(gSdifPredefinedTypes->MatrixTypesTable, Signature);
      if (PredefinedMtrxT)
	    {
	      MtrxT = SdifCreateMatrixType(Signature, PredefinedMtrxT);
	      SdifPutMatrixType(SdifF->MatrixTypesTable, MtrxT);
	      return MtrxT;
	    }
      else
	    {
          sprintf(gSdifErrorMess, "Matrix Type : '%s'", SdifSignatureToString(Signature));
          _SdifFError(SdifF, eUnDefined, gSdifErrorMess);
          return NULL;
        }
    }
}






int
SdifFTestDataType(SdifFileT* SdifF)
{
#if (_SdifFormatVersion >= 3)
    if (SdifDataTypeKnown (SdifF->CurrMtrxH->DataType))
        return (eTrue);
    else
    {
        sprintf (gSdifErrorMess, " 0x%04x, then Float4 used", 
		 SdifF->CurrMtrxH->DataType);
	_SdifFError (SdifF, eTypeDataNotSupported, gSdifErrorMess);
	SdifF->CurrMtrxH->DataType = eFloat4;
	return eFalse;
    }
#else

  switch (SdifF->CurrMtrxH->DataType)
    {
    case eFloat4:
    case eFloat8:
      return eTrue;
    default:
      sprintf(gSdifErrorMess, " 0x%04x, then Float4 used", SdifF->CurrMtrxH->DataType);
      SdifF->CurrMtrxH->DataType = eFloat4;
      _SdifFError(SdifF, eTypeDataNotSupported, gSdifErrorMess);
      return eFalse;
    }

#endif
}






int SdifFTestNbColumns(SdifFileT* SdifF)
{
  if (SdifF->CurrMtrxT->NbColumnDef < SdifF->CurrMtrxH->NbCol)
    {
      sprintf(gSdifErrorMess, "%d maximum, %d in the header",
        SdifF->CurrMtrxT->NbColumnDef,
        SdifF->CurrMtrxH->NbCol);
      _SdifFError(SdifF, eBadNbData, gSdifErrorMess);
      return eTrue; /* don't return eFalse, since then the wrong number
                      of columns is read */
    }
  else
    return eTrue;
}







int SdifFTestMatrixWithFrameHeader(SdifFileT* SdifF)
{
    SdifComponentT *CD;

    CD = SdifFrameTypeGetComponent_MtrxS (SdifF->CurrFramT,
					  SdifF->CurrMtrxH->Signature);

    if (CD)
	if (! SdifFIsInMtrxUsed (SdifF, SdifF->CurrMtrxH->Signature))
	    return eTrue;
	else
	    _SdifFError(SdifF, eMtrxUsedYet,
			SdifSignatureToString(SdifF->CurrMtrxH->Signature));
    else
    {   /* warn about additional undeclared matrix in frame */
	_SdifFError(SdifF, eMtrxNotInFrame,
		    SdifSignatureToString(SdifF->CurrMtrxH->Signature));
	return eTrue; /* ...but do not signal an error, because we
                         allow any additional matrices in frames */
    }
    
    return eFalse;
}





int
SdifFTestNotEmptyMatrix(SdifFileT* SdifF)
{
  if ((SdifF->CurrMtrxH->NbCol <= 0) && (SdifF->CurrMtrxH->NbRow <= 0)) 
    return eFalse;
  else
    return eTrue;
} 





int
SdifFTestMatrixHeader(SdifFileT* SdifF)
{
  SdifF->CurrMtrxT = SdifTestMatrixType(SdifF, SdifF->CurrMtrxH->Signature);

  if (! SdifF->CurrMtrxT )
    return eFalse;

  if (SdifF->CurrFramT)
    if(! SdifFTestMatrixWithFrameHeader(SdifF) )
      return eFalse;

  if (! SdifFTestDataType (SdifF) )
    return eFalse;

  if (! SdifFTestNbColumns (SdifF) )
    return eFalse;

  return eTrue;
}





SdifColumnDefT*
SdifTestColumnDef(SdifFileT *SdifF, SdifMatrixTypeT *MtrxT, char *NameCD)
{
  SdifColumnDefT* CD;

  CD = SdifMatrixTypeGetColumnDef(MtrxT, NameCD);
  if (CD)
    return CD;
  else
    {
      sprintf(gSdifErrorMess,
	      "In Matrix Type '%s', Column : %s",
	      SdifSignatureToString(MtrxT->Signature),
	      NameCD);
      _SdifFError(SdifF, eUnDefined, gSdifErrorMess);
      return NULL;
    }
}








SdifFrameTypeT*
SdifTestFrameType(SdifFileT *SdifF, SdifSignature Signature)
{
  SdifFrameTypeT
    *FrameType,
    *PredefinedFrameType;
  
  FrameType = SdifGetFrameType(SdifF->FrameTypesTable, Signature);
  if (FrameType)
    return FrameType;
  else
    {
      PredefinedFrameType = SdifGetFrameType(gSdifPredefinedTypes->FrameTypesTable, Signature);
      if (PredefinedFrameType)
	    {
	      FrameType = SdifCreateFrameType(Signature, PredefinedFrameType);
	      SdifPutFrameType(SdifF->FrameTypesTable, FrameType);
	      return FrameType;
	    }
      else
	    {
	      sprintf(gSdifErrorMess, "Frame Type : '%s'",
		          SdifSignatureToString(Signature));
	      _SdifFError(SdifF, eUnDefined, gSdifErrorMess);
	      return NULL;
	    }
    }
}






SdifComponentT*
SdifTestComponent(SdifFileT* SdifF, SdifFrameTypeT *FramT, char *NameCD)
{
  SdifComponentT* CD;

  CD = SdifFrameTypeGetComponent(FramT, NameCD);
  if (CD)
    return CD;
  else
    {
      sprintf(gSdifErrorMess,
	      "In Frame Type '%s', Component : %s",
	      SdifSignatureToString(FramT->Signature),
	      NameCD);
      _SdifFError(SdifF, eUnDefined, gSdifErrorMess);
      return NULL;
    }
}







int
SdifTestSignature(SdifFileT *SdifF, int CharEnd, SdifSignature Signature, char *Mess)
{
  if ( (SdifIsAReservedChar((char) CharEnd) != -1) || (isspace((char) CharEnd)) )
    {
      sprintf(gSdifErrorMess,
	      "%s Name not correctly read : '%s'. Last char read : '%d'",
	      Mess,
	      SdifSignatureToString(Signature),
	      (char) CharEnd);
      _SdifFError(SdifF, eNameLength, gSdifErrorMess);
      return eFalse;
    }
  else
    return eTrue;
}






int
SdifTestCharEnd(SdifFileT *SdifF, int CharEnd, char MustBe, char *StringRead,
		int ErrCondition, char *Mess)
{
  if (   ( (unsigned) CharEnd != (unsigned) MustBe)
      ||   (ErrCondition))
    {
      sprintf(gSdifErrorMess,
	      "In %s, Attempt to read (%d) '%c': (%d) '%s%c' ",
	      Mess,
	      MustBe,
	      MustBe,
	      CharEnd,
	      StringRead,
	      CharEnd);
      _SdifFError (SdifF, eSyntax, gSdifErrorMess);
      return  eFalse;
    }
  else
    return eTrue;
}








int
SdifTestMatrixTypeModifMode(SdifFileT *SdifF, SdifMatrixTypeT *MatrixType)
{  
  if (eCanModif == MatrixType->ModifMode)
    return eTrue;
  else
    {
      sprintf(gSdifErrorMess, "Matrix Type '%s' ",
	      SdifSignatureToString(MatrixType->Signature));
      _SdifFError (SdifF, eUserDefInFileYet, gSdifErrorMess);
      return  eFalse;
    }
}





int
SdifTestFrameTypeModifMode(SdifFileT *SdifF, SdifFrameTypeT *FrameType)
{
  if (eCanModif == FrameType->ModifMode)
    return eTrue;
  else
    {
      sprintf(gSdifErrorMess, "Frame Type '%s' ",
	      SdifSignatureToString(FrameType->Signature));
      _SdifFError (SdifF, eUserDefInFileYet, gSdifErrorMess);
      return  eFalse;
    }
}

