/* $Id: SdifTest.c,v 2.1 1998-12-21 18:27:42 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifTest.c
 *
 * Tests with message on errors apply on a SdifFileT*
 * exit(1) is exceptional
 *
 * author: Dominique Virolle 1997
 *
 * $log$
 */

#include <stdio.h>
#include <stdlib.h>


#include "SdifTest.h"
#include "SdifPrint.h"
#include "SdifRWLowLevel.h"
#include <ctype.h>
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






short
SdifFTestDataType(SdifFileT* SdifF)
{
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
}






short
SdifFTestNbColumns(SdifFileT* SdifF)
{
  if (SdifF->CurrMtrxT->NbColumnDef < SdifF->CurrMtrxH->NbCol)
    {
      sprintf(gSdifErrorMess, "%d maximum, %d in the header",
        SdifF->CurrMtrxT->NbColumnDef,
        SdifF->CurrMtrxH->NbCol);
      _SdifFError(SdifF, eBadNbData, gSdifErrorMess);
      return eFalse;
    }
  else
    return eTrue;
}







short
SdifFTestMatrixWithFrameHeader(SdifFileT* SdifF)
{
  SdifComponentT *CD;

  CD = SdifFrameTypeGetComponent_MtrxS(SdifF->CurrFramT,
                                       SdifF->CurrMtrxH->Signature);

  if (CD)
    if (! SdifFIsInMtrxUsed (SdifF, SdifF->CurrMtrxH->Signature))
      return eTrue;
    else
	  _SdifFError(SdifF,
        eMtrxUsedYet,
        SdifSignatureToString(SdifF->CurrMtrxH->Signature));
  else
	_SdifFError(SdifF,
      eMtrxNotInFrame,
      SdifSignatureToString(SdifF->CurrMtrxH->Signature));

  return eFalse;
}





short
SdifFTestNotEmptyMatrix(SdifFileT* SdifF)
{
  if ((SdifF->CurrMtrxH->NbCol <= 0) && (SdifF->CurrMtrxH->NbRow <= 0)) 
    return eFalse;
  else
    return eTrue;
} 





short
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







short
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






short
SdifTestCharEnd(SdifFileT *SdifF, int CharEnd, char MustBe, char *StringRead,
		short ErrCondition, char *Mess)
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








short
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





short
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

