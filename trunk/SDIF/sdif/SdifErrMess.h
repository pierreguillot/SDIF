/* $Id: SdifErrMess.h,v 3.1 1999-03-14 10:56:34 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 * 
 * SdifErrMess.h
 *
 * high level (SdifF) error management
 *
 * author: Dominique Virolle 1998
 * 
 * $Log: not supported by cvs2svn $
 *
 */


#ifndef _SdifErrMess_
#define _SdifErrMess_

#include "SdifGlobals.h"
#include "SdifList.h"
#include <stdio.h>
#include "SdifError.h"


/*#include "SdifFile.h"
 */
#ifndef SdifFileT_
#define SdifFileT_
typedef struct SdifFileS SdifFileT;
#endif

typedef enum SdifErrorTagE
{
	eUnknow,
	eNoError,
	eTypeDataNotSupported,
	eNameLength,
	eReDefined,
	eUnDefined,
	eSyntax,
	eBadTypesFile,
	eBadType,
	eBadHeader,
	eRecursiveDetect,
	eUnInterpreted,
	eOnlyOneChunkOf,
	eUserDefInFileYet,
	eBadMode,
	eBadStdFile,
	eBadNbData,
	eReadWriteOnSameFile,
    eBadFormatVersion,
    eMtrxUsedYet,
    eMtrxNotInFrame
} SdifErrorTagET;


typedef enum SdifErrorLevelE
{
	eFatal,
	eError,
	eWarning,
	eRemark,
	eNoLevel
} SdifErrorLevelET;




typedef struct SdifErrorS SdifErrorT;
struct SdifErrorS
{
	SdifErrorTagET		Tag;
	SdifErrorLevelET	Level;
	char*				UserMess;
};

/* Table of error format for the fprintf */
extern const SdifErrorT gSdifErrMessFormat[];


typedef struct SdifErrorLS SdifErrorLT;
struct SdifErrorLS
{
  SdifListT*    ErrorList;
  SdifFileT*	SdifF; /* only a link */
};


SdifErrorT*		SdifCreateError		(SdifErrorTagET Tag,
											SdifErrorLevelET Level, const char* UserMess);
void			SdifKillError		(SdifErrorT *Error);
SdifErrorLT*	SdifCreateErrorL	(SdifFileT* SdifF);
void			SdifKillErrorL		(SdifErrorLT *ErrorL);
SdifErrorLT*	SdifInsertTailError	(SdifErrorLT* ErrorL,
									SdifErrorTagET Tag, const char* UserMess);
SdifErrorT*		SdifLastError		(SdifErrorLT *ErrorL);
SdifErrorTagET	SdifLastErrorTag	(SdifErrorLT *ErrorL);
SdifInt4		SdifFsPrintError	(char* oErrMess, SdifFileT* SdifF,
									 SdifErrorT* Error,
									const char *LibFile, int LibLine);

extern char	gSdifBufferError[4096];

#define _SdifFError(SdifF, ErrorTag, UserMess) \
(SdifInsertTailError(SdifF->Errors, ErrorTag, UserMess), \
 SdifFsPrintError(gSdifBufferError, SdifF, SdifLastError(SdifF->Errors), __FILE__, __LINE__), \
 fprintf(SdifStdErr,"%s", gSdifBufferError))

#endif  /* _SdifErrMess_ */
