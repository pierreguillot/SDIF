/* SdifFrame.h
 *
 *
 *
 */

#ifndef _SdifFrame_
#define _SdifFrame_

#include "SdifGlobals.h"
#include "SdifMatrix.h"


#define _SdifFrameHeaderSize 24  /* (name=4)+(version=2)+(count=2)+(ID=4)+(size=4)+(time=8) */

typedef struct SdifFrameHeaderT
{
  char FrameName[_SdifNameLen];
  SdifUInt4  Size;
  SdifUInt4  NbMatrix;
  SdifUInt4  NumID;
  SdifFloat8 Time;
} SdifFrameHeaderType;


extern SdifFrameHeaderType*
SdifCreateFrameHeader(char *FrameName,
		      SdifUInt4  Size,
		      SdifUInt4  NbMatrix,
		      SdifUInt4  NumID,
		      SdifFloat8 Time);


extern SdifFrameHeaderType*
SdifCreateFrameHeaderEmpty(char *FrameName);

extern void
SdifKillFrameHeader(SdifFrameHeaderType *FrameHeader);

extern void
SdifFScanFrameHeader(FILE *fr, char *FrameName, SdifFrameHeaderType* FrameHeader);

extern int
SdifFPrintFrameHeader(FILE *f, SdifFrameHeaderType* FrameHeader);

extern int
SdifFReadFrameHeader(char *FrameName, SdifFrameHeaderType* FrameHeader, FILE *fr);
	      
extern int
SdifFWriteFrameHeader(SdifFrameHeaderType* FrameHeader, FILE *fw);


typedef struct SdifFrameDataT
{
  SdifFrameHeaderType *Header;
  SdifMatrixDataType* *Matrix_s;
} SdifFrameDataType;

extern SdifFrameDataType*
SdifCreateFrameData(char *FrameName,
		    SdifUInt4 NumID,
		    SdifFloat8 Time);

extern void
SdifKillFrameData(SdifFrameDataType *FrameData);

extern SdifFrameDataType*
SdifFrameDataPutNthMatrixData(SdifFrameDataType *FrameData,
			      unsigned int NthMatrix,
			      SdifMatrixDataType *MatrixData);

extern SdifFrameDataType*
SdifFrameDataPutComponentMatrixData(SdifFrameDataType *FrameData,
				    char *CompoName,
				    SdifMatrixDataType *MatrixData);

extern SdifMatrixDataType*
SdifFrameDataGetNthMatrixData(SdifFrameDataType *FrameData,
			      SdifUInt4 NthMatrix);

extern SdifMatrixDataType*
SdifFrameDataGetComponentMatrixData(SdifFrameDataType *FrameData,
				    char *CompoName);

extern int
SdifFWriteMatrix_s(SdifFrameDataType *FrameData, FILE *fw);

extern int
SdifFWriteFrameData(SdifFrameDataType *FrameData, FILE *fw);

extern int
SdifFReadFrameMatrix_s(SdifFrameHeaderType *FramH,
		       SdifFrameDataType *FramD,
		       FILE *fr);

extern int
SdifSkipFrameData(SdifFrameHeaderType *FramH, FILE *fr);

#endif /* _SdifFrame_ */
