/* $Id: SdifErrMess.h,v 1.1 2008-09-10 16:16:44 diemo Exp $
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
 * Revision 3.5  2000/08/22  13:17:24  schwarz
 * Centralised error report function SdifFError, called from _SdifFError
 * macro.  Error level names (gSdifErrorLevel) are printed, so that users
 * can tell warnings from errors.  Errors are counted by level in
 * SdifFileT.ErrorCount[].  Error counts can be queried with SdifFNumErrors.
 *
 * Revision 3.4  2000/08/07  15:05:44  schwarz
 * Error checking for read general header.
 * Remove double definition of 1GAI matrix type.
 *
 * Revision 3.3  2000/05/15  16:22:30  schwarz
 * Avoid warning about KillerFT function pointer type (ANSI prototype given).
 * Argument to kill func is now void *.
 *
 * Revision 3.2  1999/09/28  10:35:43  schwarz
 * Added SdifEnableErrorOutput/SdifDisableErrorOutput to temporarily
 * switch off error messages when doing things that might fail.
 *
 * Revision 3.1  1999/03/14  10:56:34  virolle
 * SdifStdErr add
 *
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


/*
// DATA GROUP:	Error Handling
*/


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
	eNoLevel,
	eNumLevels	/* level count, must always be last */
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


/*
// FUNCTION GROUP:	Error Handling
*/

SdifErrorT*	SdifCreateError		(SdifErrorTagET Tag,
					 SdifErrorLevelET Level, 
					 const char* UserMess);
void		SdifKillError		(void *Error);
SdifErrorLT*	SdifCreateErrorL	(SdifFileT* SdifF);
void		SdifKillErrorL		(SdifErrorLT *ErrorL);
SdifUInt4       SdifInsertTailError     (SdifErrorLT* ErrorL, 
					 int ErrorCount [], 
					 SdifErrorTagET Tag, 
					 const char* UserMess);
SdifErrorT*	SdifLastError		(SdifErrorLT *ErrorL);
SdifErrorTagET	SdifLastErrorTag	(SdifErrorLT *ErrorL);

SdifUInt4	SdifFError		(SdifFileT* SdifF, 
					 SdifErrorTagET ErrorTag, 
					 const char *UserMess, 
					 const char *file, 
					 const int line);
SdifInt4	SdifFsPrintError	(char* oErrMess, SdifFileT* SdifF,
					 SdifErrorT* Error,
					 const char *LibFile, int LibLine);


/*DOC:
  Switch output of error messages on stderr by _SdifFError on. 
*/
void	SdifEnableErrorOutput  (void);

/*DOC:
  Switch output of error messages on stderr by _SdifFError off. 
*/
void	SdifDisableErrorOutput (void);


extern char	gSdifBufferError[4096];
extern int	gSdifErrorOutputEnabled;


#define _SdifFError(SdifF, ErrorTag, UserMess) \
	 SdifFError(SdifF, ErrorTag, UserMess, __FILE__, __LINE__)

#endif  /* _SdifErrMess_ */
