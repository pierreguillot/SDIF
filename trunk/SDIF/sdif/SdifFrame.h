/* SdifFrame.h
 *
 * Frame Header, Frame Data structures management
 *
 * author: Dominique Virolle 1997
 *
 */

#ifndef _SdifFrame_
#define _SdifFrame_

#include "SdifGlobals.h"
#include "SdifHash.h"
#include "SdifMatrix.h"

#define _SdifFrameHeaderSize 16  /* (ID=4)+(size=4)+(time=8) */



typedef struct SdifFrameHeaderS SdifFrameHeaderT;
struct SdifFrameHeaderS
{
  SdifSignature Signature;
  SdifUInt4  Size;
  SdifUInt4  NbMatrix;
  SdifUInt4  NumID;
  SdifFloat8 Time;
} ;


typedef struct SdifFrameDataS SdifFrameDataT;
struct SdifFrameDataS
{
  SdifFrameHeaderT *Header;
  SdifMatrixDataT* *Matrix_s;
} ;


extern SdifFrameHeaderT* SdifCreateFrameHeader(SdifSignature Signature,
					       SdifUInt4 Size,
					       SdifUInt4 NbMatrix,
					       SdifUInt4 NumID,
					       SdifFloat8 Time);

extern SdifFrameHeaderT* SdifCreateFrameHeaderEmpty(SdifSignature Signature);

extern void              SdifKillFrameHeader  (SdifFrameHeaderT *FrameHeader);

extern SdifFrameDataT* SdifCreateFrameData(SdifHashTableT *FrameTypesTable,
					   SdifSignature FrameSignature,
					   SdifUInt4 NumID,
					   SdifFloat8 Time);

extern void            SdifKillFrameData   (SdifHashTableT *FrameTypesTable, SdifFrameDataT *FrameData);

extern SdifFrameDataT* SdifFrameDataPutNthMatrixData(SdifFrameDataT *FrameData, unsigned int NthMatrix,
						     SdifMatrixDataT *MatrixData);

extern SdifFrameDataT* SdifFrameDataPutComponentMatrixData(SdifHashTableT *FrameTypesTable,
							   SdifFrameDataT *FrameData,
							   char *CompoName, SdifMatrixDataT *MatrixData);

extern SdifMatrixDataT* SdifFrameDataGetNthMatrixData(SdifFrameDataT *FrameData, unsigned int NthMatrix);

extern SdifMatrixDataT* SdifFrameDataGetComponentMatrixData(SdifHashTableT *FrameTypesTable,
							    SdifFrameDataT *FrameData,
							    char *CompoName);

#endif /* _SdifFrame_ */


