/* SdifError.h
 *
 * Fatal or program error management
 *
 * author: Dominique Virolle 1997
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

extern void SdifErrorWarning(SdifErrorEnum Error, void *ErrorMess);

#define _SdifError(error, mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifErrorWarning((error), (mess)))


#define _Debug(mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, \
fprintf(stderr, "%s, %d:\t", SdifErrorFile, SdifErrorLine), \
fprintf(stderr, (mess)))

#define _SdifRemark(mess) \
fprintf(stderr, "*Sdif* %s\n", mess)

#endif /* _SdifError_ */






