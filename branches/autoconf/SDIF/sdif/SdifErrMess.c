/* $Id: SdifErrMess.c,v 3.8.2.2 2000-08-21 21:34:59 tisseran Exp $
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
 * SdifErrorMess.c
 *
 * high level (SdifF) error management
 *
 * author: Dominique Virolle 1998
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.8.2.1  2000/08/21  14:04:03  tisseran
 * *** empty log message ***
 *
 * Revision 3.7  2000/05/15  16:22:29  schwarz
 * Avoid warning about KillerFT function pointer type (ANSI prototype given).
 * Argument to kill func is now void *.
 *
 * Revision 3.6  2000/03/01  11:17:29  schwarz
 * More meaningful error messages.
 *
 * Revision 3.5  1999/11/03  16:42:30  schwarz
 * Use _SdifNVTStreamID for stream ID of 1NVT frames because of CNMAT
 * restriction of only one frame type per stream.
 * (See SdifNameValuesLNewTable)
 *
 * Revision 3.4  1999/10/13  16:05:39  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
 * Revision 3.3  1999/09/28  13:08:49  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.2  1999/09/28  10:34:47  schwarz
 * Added SdifEnableErrorOutput/SdifDisableErrorOutput to temporarily
 * switch off error messages when doing things that might fail.
 *
 * Revision 3.1  1999/03/14  10:56:33  virolle
 * SdifStdErr add
 *
 */


#include <preincluded.h>
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
{ eBadNbData,			eWarning,	"Number of rows or columns too big : %s\n"},
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
SdifKillError(void *Error)
{
  if (Error)
    {
	  if (((SdifErrorT *)Error)->UserMess)
		SdifKillStr(((SdifErrorT *) Error)->UserMess);

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



#if defined (DEBUG)  ||  defined (_DEBUG)
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
#endif /* ifdef DEBUG */


	if (SdifF->Stream)
		SdiffGetPos(SdifF->Stream, &(SdifF->Pos));
	if (SdifF->Pos !=0)
		sprintf(PosErrMess, " (byte:%6lu=0x%04lx=0%06lo)", 
			SdifF->Pos, SdifF->Pos, SdifF->Pos);

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
			sprintf(MtrxErrMess, "  MtrxH :   %s   DataWidth: %04x   Rows: %d   Columns: %d\n",
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
