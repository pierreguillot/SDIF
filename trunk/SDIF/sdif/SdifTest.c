/* $id$
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

#define _NbOfWarningMax 19

void
SdifInterpretationError(SdifInterpretationErrorET Error, SdifFileT* SdifF, const void *ErrorMess)
{
  if (SdifF->NbOfWarning >= _NbOfWarningMax)
    {
      fprintf(stderr, "*Sdif* STOP     --- Too Many Warnings ---\n");
      exit(1);
    }
  

  if (SdifF->Stream)
    SdiffGetPos(SdifF->Stream, &(SdifF->Pos));

  /*fprintf(stderr,
   *	  "*Sdif* Interpretation Warning (%s, %d)\n  SdifFile: %s",
   *	  SdifErrorFile,
   *	  SdifErrorLine,
   *	  SdifF->Name);
   */

  fprintf(stderr,
	  "*Sdif* Interpretation Warning\n  SdifFile: %s",
	  SdifF->Name);

  if (SdifF->Pos !=0)
    fprintf(stderr, " (byte:%lu)", SdifF->Pos);


  if (SdifF->TextStream)
    fprintf(stderr, ", TextFile: %s\n", SdifF->TextStreamName);
  else
    fprintf(stderr, "\n");
  
  if (SdifF->CurrFramH)
    {
      fprintf(stderr, "  FramH : ");
      fflush(stderr);
      SdifPrintFrameHeader(stderr, SdifF->CurrFramH);
    }
  
  if (SdifF->CurrMtrxH)
    {
      if (SdifF->CurrMtrxH->Signature != eEmptySignature)
	{
	  fprintf(stderr, "  MtrxH :   ");
	  fflush(stderr);
	  SdifPrintMatrixHeader(stderr, SdifF->CurrMtrxH);
	}
    }


  fprintf(stderr, "--> ");

  switch(Error)
    {
    case eTypeDataNotSupported :
      fprintf(stderr, "Type of data not actualy supported : %s\n", ErrorMess);
      /*SdifF->NbOfWarning++;*/
      break;
    case  eNameLength :
      fprintf(stderr, "Bad Name : '%s'\n", ErrorMess);
      SdifF->NbOfWarning++;
      break;
    case  eReDefined :
      fprintf(stderr, "%s redefined\n", ErrorMess);
      /*SdifF->NbOfWarning++;*/
      break;
    case  eUnDefined :
      fprintf(stderr, "%s undefined\n", ErrorMess);
      /*SdifF->NbOfWarning++;*/
      break;
    case  eSyntax :
      fprintf(stderr, "Syntax error: %s\n", ErrorMess);
      SdifF->NbOfWarning++;
      break;
    case  eBadTypesFile:
      fprintf(stderr, "%s is not a Types Definitions file\n", ErrorMess);
      /*SdifF->NbOfWarning++;*/
      break;
    case  eBadType :
      fprintf(stderr, "Bad Type used : %s\n", ErrorMess);
      /*SdifF->NbOfWarning++;*/
      break;
    case eBadHeader:
      fprintf(stderr, "EXIT: Bad Sdif Header : %s\n", ErrorMess);
      /*SdifF->NbOfWarning++;*/
      exit(1);
      break;
    case  eRecursiveDetect :
      fprintf(stderr, "Recursive declaration detected : %s\n", ErrorMess);
      SdifF->NbOfWarning++;
      break;
    case  eUnInterpreted :
      fprintf(stderr, "Attempt to interpret %s failed\n", ErrorMess);
      SdifF->NbOfWarning++;
      break;
    case  eOnlyOneChunkOf:
      fprintf(stderr, "There can be only 0 or 1 '%s' chunk.\n",  ErrorMess);
      /*SdifF->NbOfWarning++;*/
      break;            
    case eUserDefInFileYet:
      fprintf(stderr, "%s has been completed in this file yet\n",  ErrorMess);
      /*SdifF->NbOfWarning++;*/
      break;
    case eBadMode :
      fprintf(stderr, "Bad mode at sdif file opening (%d), %s\n", SdifF->Mode, ErrorMess);
      SdifF->NbOfWarning++;
      break;
    case eBadStdFile :
      fprintf(stderr, "Bad standart file or bad mode (%d), %s\n", SdifF->Mode, ErrorMess);
      SdifF->NbOfWarning++;
      break;
    case eBadNbData :
      fprintf(stderr, "Matrix number of columns too big : %s\n", ErrorMess);
      /*SdifF->NbOfWarning++;*/
      break;
    case eReadWriteOnSameFile :
      fprintf(stderr, "Read file and Write file are the same file : %s", ErrorMess);
      SdifF->NbOfWarning++;
    default :
      fprintf(stderr, "Warning unknown", ErrorMess);
      SdifF->NbOfWarning++;
      break;
    }
  fprintf(stderr, "\n");
  fflush(stderr);
}










SdifMatrixTypeT*
SdifTestMatrixType(SdifFileT *SdifF, SdifSignature Signature)
{
  SdifMatrixTypeT* MtrxT;
  SdifMatrixTypeT* PredefinedMtrxT;

  if (MtrxT = SdifGetMatrixType(SdifF->MatrixTypesTable, Signature))
    return MtrxT;
  else
    {
      if (PredefinedMtrxT = SdifGetMatrixType(gSdifPredefinedTypes->MatrixTypesTable, Signature))
	{
	  MtrxT = SdifCreateMatrixType(Signature, PredefinedMtrxT);
	  SdifPutMatrixType(SdifF->MatrixTypesTable, MtrxT);
	  return MtrxT;
	}
      else
	{
	  sprintf(gSdifErrorMess, "Matrix Type : '%s'",
		  SdifSignatureToString(Signature));
	  _SdifFileMess(SdifF, eUnDefined, gSdifErrorMess);
	  return NULL;
	}
    }
}




short
SdifTestDataType(SdifFileT* SdifF)
{
  switch (SdifF->CurrMtrxH->DataType)
    {
    case eFloat4:
    case eFloat8:
      return eTrue;
    default:
      sprintf(gSdifErrorMess, " 0x%04x, then Float4 used", SdifF->CurrMtrxH->DataType);
      SdifF->CurrMtrxH->DataType = eFloat4;
      _SdifFileMess(SdifF, eTypeDataNotSupported, gSdifErrorMess);
      return eFalse;
    }
}






short
SdifTestNbColumns(SdifFileT* SdifF, SdifUInt4 NbCol, SdifMatrixTypeT *MtrxT)
{
  if (MtrxT->NbColumnDef < NbCol)
    {
      sprintf(gSdifErrorMess, "%d maximum, %d in the header", MtrxT->NbColumnDef, NbCol);
      _SdifFileMess(SdifF, eBadNbData, gSdifErrorMess);
      return eFalse;
    }
  else
    return eTrue;
}





short
SdifTestMatrixHeader(SdifFileT* SdifF)
{
  SdifMatrixTypeT *MtrxT;
  int res;

  MtrxT = SdifTestMatrixType(SdifF, SdifF->CurrMtrxH->Signature);

  if (MtrxT)
    {
      res = SdifTestDataType  (SdifF);
      res = (res) ? SdifTestNbColumns (SdifF, SdifF->CurrMtrxH->NbCol, MtrxT) : 0;
      return (short) res;
    }
  else
    return (short) eFalse;
}





SdifColumnDefT*
SdifTestColumnDef(SdifFileT *SdifF, SdifMatrixTypeT *MtrxT, char *NameCD)
{
  SdifColumnDefT* CD;

  if (CD = SdifMatrixTypeGetColumnDef(MtrxT, NameCD))
    return CD;
  else
    {
      sprintf(gSdifErrorMess,
	      "In Matrix Type '%s', Column : %s",
	      SdifSignatureToString(MtrxT->Signature),
	      NameCD);
      _SdifFileMess(SdifF, eUnDefined, gSdifErrorMess);
      return NULL;
    }
}








SdifFrameTypeT*
SdifTestFrameType(SdifFileT *SdifF, SdifSignature Signature)
{
  SdifFrameTypeT
    *FrameType,
    *PredefinedFrameType;
  
  if (FrameType = SdifGetFrameType(SdifF->FrameTypesTable, Signature))
    return FrameType;
  else
    {
      if (PredefinedFrameType = SdifGetFrameType(gSdifPredefinedTypes->FrameTypesTable, Signature))
	{
	  FrameType = SdifCreateFrameType(Signature, PredefinedFrameType);
	  SdifPutFrameType(SdifF->FrameTypesTable, FrameType);
	  return FrameType;
	}
      else
	{
	  sprintf(gSdifErrorMess, "Frame Type : '%s'",
		  SdifSignatureToString(Signature));
	  _SdifFileMess(SdifF, eUnDefined, gSdifErrorMess);
	  return NULL;
	}
    }
}






SdifComponentT*
SdifTestComponent(SdifFileT* SdifF, SdifFrameTypeT *FramT, char *NameCD)
{
  SdifComponentT* CD;

  if (CD = SdifFrameTypeGetComponent(FramT, NameCD))
    return CD;
  else
    {
      sprintf(gSdifErrorMess,
	      "In Frame Type '%s', Component : %s",
	      SdifSignatureToString(FramT->Signature),
	      NameCD);
      _SdifFileMess(SdifF, eUnDefined, gSdifErrorMess);
      return NULL;
    }
}







short
SdifTestFrameWithMatrixHeader(SdifFileT* SdifF, int NumComponent)
{
  SdifComponentT *CD;
  SdifFrameTypeT *FramT;

  FramT = SdifGetFrameType(SdifF->FrameTypesTable, SdifF->CurrFramH->Signature);
  if (FramT)
    {
      CD = SdifFrameTypeGetNthComponent(FramT, NumComponent);
      if (CD)
	{
	  if (CD->MatrixSignature == SdifF->CurrMtrxH->Signature)
	    return eTrue;
	  else
	    {
	      sprintf(gSdifErrorMess, "the %dth Component is: '%s' %s",
		      NumComponent,
		      SdifSignatureToString(CD->MatrixSignature),
		      CD->Name);
	      _SdifFileMess(SdifF, eBadType, gSdifErrorMess);
	      return eFalse;
	    }
	}
    }
  return eFalse;
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
      _SdifFileMess(SdifF, eNameLength, gSdifErrorMess);
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
      _SdifFileMess (SdifF, eSyntax, gSdifErrorMess);
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
      _SdifFileMess (SdifF, eUserDefInFileYet, gSdifErrorMess);
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
      _SdifFileMess (SdifF, eUserDefInFileYet, gSdifErrorMess);
      return  eFalse;
    }
}

