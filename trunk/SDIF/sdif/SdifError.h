/* SdifError.h
 *
 *
 * 
 */


#ifndef _SdifError_
#define _SdifError_

typedef enum eOutReport
{
  eFalse = 256,
  eTrue,
  eFreeNull,
  eAllocFail,
  eUserdefBefore,
  eTypeDataNotSupported,
  eArrayPosition,
  eWordCut,
  eEof,
  eTokenLength,
  eNameLength,
  eReDefined,
  eUnDefined,
  eSyntax,
  eAffectationOrder,
  eRecursiveDetect,
  eBadType,
  eBadHeader,
  eBadTypesFile,
  eInfoTableMissing,
  eOnlyOneChunkOf,
  eFileNotFound,
  eUnInterpreted
} SdifOutReportType;

extern char *SdifErrorFile;
extern int SdifErrorLine;

extern void
SdifErrorWarning(SdifOutReportType Error, void *ErrorMess);

#define _SdifError(error, mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifErrorWarning((error), (mess)))

#endif /* _SdifError_ */






