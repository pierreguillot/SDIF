/* $Id: SdifErrMess.c,v 3.2 1999-09-28 10:34:47 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifErrorMess.c
 *
 * high level (SdifF) error management
 *
 * author: Dominique Virolle 1998
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.1  1999/03/14  10:56:33  virolle
 * SdifStdErr add
 *
 */


#include "SdifErrMess.h"
#include "SdifFile.h"
#include <string.h>
#include <stdlib.h>



char gSdifBufferError[4096];
int  gSdifErrorOutputEnabled = 1;


const SdifErrorT gSdifErrMessFormat[] = {

{ eUnknow,				eNoLevel,	"Warning unknown"},
{ eNoError,				eNoLevel,   "No Error"},
{ eTypeDataNotSupported,eWarning,	"Type of data not actualy supported : %s\n"},
{ eNameLength,			eWarning,	"Bad UserMess : '%s'\n"},
{ eReDefined,			eError,		"%s redefined\n"},
{ eUnDefined,			eError,		"%s undefined\n"},
{ eSyntax,				eWarning,	"Syntax error: %s\n"},
{ eBadTypesFile,		eWarning,	"%s is not a Types Definitions file\n"},
{ eBadType,				eWarning,	"Bad Type used : %s\n"},
{ eBadHeader,			eError,		"Bad Sdif Header : %s\n"},
{ eRecursiveDetect,		eWarning,	"Recursive declaration detected : %s\n"},
{ eUnInterpreted,		eWarning,	"Attempt to interpret %s failed\n"},
{ eOnlyOneChunkOf,		eWarning,	"There can be only 0 or 1 '%s' chunk.\n"},
{ eUserDefInFileYet,	eWarning,	"%s has been completed in this file yet\n"},
{ eBadMode,				eError,		"Bad mode at sdif file opening (%d), %s\n"},
{ eBadStdFile,			eError,		"Bad standart file or bad mode (%d), %s\n"},
{ eBadNbData,			eWarning,	"Matrix number of columns too big : %s\n"},
{ eReadWriteOnSameFile,	eError,		"Read file and Write file are the same file : %s"},
{ eBadFormatVersion,	eError,		"Bad Format Version : %s"},
{ eMtrxUsedYet,			eWarning,	"Matrix has been used in this frame yet : %s\n"},
{ eMtrxNotInFrame,		eWarning,	"Matrix is not in frames components : %s\n"},

};



SdifErrorT*
SdifCreateError(SdifErrorTagET Tag, SdifErrorLevelET Level, const char* UserMess)
{
  SdifErrorT *NewError = NULL;

  NewError = SdifMalloc(SdifErrorT);
  if (NewError)
    {
      NewError->Tag		= Tag;
      NewError->Level	= Level;
      NewError->UserMess= SdifCreateStrNCpy(UserMess, SdifStrLen(UserMess)+1);
      
      return NewError;
    }
  else
    {
      _SdifError(eAllocFail, "Error allocation");
      return NULL;
    }
}







void
SdifKillError(SdifErrorT *Error)
{
  if (Error)
    {
	  if (Error->UserMess)
		SdifKillStr(Error->UserMess);

      SdifFree(Error);
    }
  else
    _SdifError(eFreeNull, "Error free");
}



SdifErrorLT*
SdifCreateErrorL(SdifFileT* SdifF)
{
  SdifErrorLT *NewErrorL = NULL;
  
  NewErrorL = SdifMalloc(SdifErrorLT);
  if (NewErrorL)
  {
	  NewErrorL->SdifF = SdifF;
      NewErrorL->ErrorList = SdifCreateList(SdifKillError);
      return NewErrorL;
	}
  else
    {
      _SdifError(eAllocFail, "ErrorL allocation");
      return NULL;
    }
}








void
SdifKillErrorL(SdifErrorLT *ErrorL)
{
/* Error->SdifF is only a link to know the Sdif file of the error
 * then it isn't killed
 */
  if (ErrorL)
    {
        SdifKillList(ErrorL->ErrorList);
	SdifFree(ErrorL);
    }
  else
    _SdifError(eFreeNull, "ErrorL free");
}




SdifErrorLT*
SdifInsertTailError(SdifErrorLT* ErrorL, SdifErrorTagET Tag, const char* UserMess)
{
	SdifErrorT* NewError = NULL;

	NewError = SdifCreateError(Tag, gSdifErrMessFormat[Tag].Level, UserMess);

    SdifListPutTail(ErrorL->ErrorList, NewError);

	return ErrorL;
}



SdifErrorT*
SdifLastError(SdifErrorLT *ErrorL)
{
	if (SdifListIsEmpty(ErrorL->ErrorList))
		return NULL;
	else
        return (SdifErrorT*) SdifListGetTail(ErrorL->ErrorList);
}



SdifErrorTagET
SdifLastErrorTag(SdifErrorLT *ErrorL)
{
	if (SdifListIsEmpty(ErrorL->ErrorList))
		return eNoError;
    else
        return ((SdifErrorT*) SdifListGetTail(ErrorL->ErrorList))->Tag;
}




SdifInt4
SdifFsPrintError(char* oErrMess,
				 SdifFileT* SdifF,
				 SdifErrorT* Error,
				 const char *LibFile, int LibLine)
{
	char HeadErrMess [512];
	char PosErrMess  [64];
	char TextErrMess [512];
	char FramErrMess [512];
	char MtrxErrMess [512];
	char ErrErrMess  [1024];


	strcpy(HeadErrMess,"");
	strcpy(PosErrMess, "");
	strcpy(TextErrMess,"");
	strcpy(FramErrMess,"");
	strcpy(MtrxErrMess,"");
	strcpy(ErrErrMess, "");



#ifdef _DEBUG
	sprintf(HeadErrMess,
			"*Sdif* Error %d of level %d (%s, %d)\n  SdifFile: %s",
			Error->Tag,
			Error->Level,
  			LibFile,
  			LibLine,
  			SdifF->Name);
#else
	sprintf(HeadErrMess,
			"*Sdif* Error %d of level %d\n  SdifFile: %s",
			Error->Tag,
			Error->Level,
			SdifF->Name);
#endif /* ifdef _DEBUG */


	if (SdifF->Stream)
		SdiffGetPos(SdifF->Stream, &(SdifF->Pos));
	if (SdifF->Pos !=0)
		sprintf(PosErrMess, " (byte:%lu)", SdifF->Pos);

	if (SdifF->TextStream)
		sprintf(TextErrMess, ", TextFile: %s\n", SdifF->TextStreamName);
	else
		sprintf(TextErrMess, "\n");
  

	if (SdifF->CurrFramH)
	{
		sprintf(FramErrMess, "  FramH : %s   Size: 0x%04x   NbMatrix: %u   NumID: %u   Time: %g\n",
			SdifSignatureToString(SdifF->CurrFramH->Signature),
			SdifF->CurrFramH->Size,
			SdifF->CurrFramH->NbMatrix,
			SdifF->CurrFramH->NumID,
			SdifF->CurrFramH->Time);
	}

	if (SdifF->CurrMtrxH)
	{
		if (SdifF->CurrMtrxH->Signature != eEmptySignature)
		{
			sprintf(MtrxErrMess, "  MtrxH :   %s   DataWidth: %d   Rows: %d   Columns: %d\n",
				SdifSignatureToString(SdifF->CurrMtrxH->Signature),
				SdifF->CurrMtrxH->DataType,
				SdifF->CurrMtrxH->NbRow,
				SdifF->CurrMtrxH->NbCol);
		}
	}


	switch(Error->Tag)
	{
	case eNoError :
	case eUnknow :
		sprintf(ErrErrMess, gSdifErrMessFormat[Error->Tag].UserMess);
		break;
    case eBadMode :
    case eBadStdFile :
		sprintf(ErrErrMess, gSdifErrMessFormat[Error->Tag].UserMess,
			SdifF->Mode, Error->UserMess);
		break;
    default :
		sprintf(ErrErrMess, gSdifErrMessFormat[Error->Tag].UserMess,
			Error->UserMess);
		break;
    }

	return sprintf(oErrMess,"%s%s%s%s%s--> %s\n",
				HeadErrMess,
				PosErrMess,
				TextErrMess,
				FramErrMess,
				MtrxErrMess,
				ErrErrMess);
}


void	
SdifEnableErrorOutput  (void)
{
    gSdifErrorOutputEnabled = 1;
}

void
SdifDisableErrorOutput (void)
{
    gSdifErrorOutputEnabled = 0;
}
