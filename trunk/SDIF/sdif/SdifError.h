/* $Id: SdifError.h,v 2.0 1998-11-29 11:41:26 virolle Exp $
 * 
 * SdifError.h
 *
 * Fatal or program error management
 *
 * author: Dominique Virolle 1997
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.4  1998/11/10  15:31:39  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 * Revision 1.3  1998/04/24  12:40:32  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 *
 */


#ifndef _SdifError_
#define _SdifError_

typedef enum SdifErrorE
{
  eFalse = 0,
  eTrue = 1,
  eFreeNull = 256,
  eAllocFail,
  eArrayPosition,
  eEof,
  eFileNotFound,
  eInvalidPreType,
  eAffectationOrder,
  eNoModifErr,
  eNotInDataTypeUnion,
  eNotFound,
  eExistYet,
  eWordCut,
  eTokenLength
} SdifErrorEnum;

extern char *SdifErrorFile;
extern int SdifErrorLine;

void SdifErrorWarning(SdifErrorEnum Error, const void *ErrorMess);

#define _SdifError(error, mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifErrorWarning((error), (mess)))


#define _Debug(mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, \
fprintf(stderr, "%s, %d:\t", SdifErrorFile, SdifErrorLine), \
fprintf(stderr, (mess)))

#define _SdifRemark(mess) \
fprintf(stderr, "*Sdif* %s\n", mess)

#endif /* _SdifError_ */






