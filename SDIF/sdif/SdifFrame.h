/* $Id: SdifFrame.h,v 2.0 1998-11-29 11:41:44 virolle Exp $
 *
 * SdifFrame.h
 *
 * Frame Header, Frame Data structures management
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.4  1998/11/10  15:31:46  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
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


SdifFrameHeaderT* SdifCreateFrameHeader(SdifSignature Signature,
					       SdifUInt4 Size,
					       SdifUInt4 NbMatrix,
					       SdifUInt4 NumID,
					       SdifFloat8 Time);

SdifFrameHeaderT* SdifCreateFrameHeaderEmpty(SdifSignature Signature);

void              SdifKillFrameHeader  (SdifFrameHeaderT *FrameHeader);

SdifFrameDataT* SdifCreateFrameData(SdifHashTableT *FrameTypesTable,
					   SdifSignature FrameSignature,
					   SdifUInt4 NumID,
					   SdifFloat8 Time);

void            SdifKillFrameData   (SdifHashTableT *FrameTypesTable, SdifFrameDataT *FrameData);

SdifFrameDataT* SdifFrameDataPutNthMatrixData(SdifFrameDataT *FrameData, unsigned int NthMatrix,
						     SdifMatrixDataT *MatrixData);

SdifFrameDataT* SdifFrameDataPutComponentMatrixData(SdifHashTableT *FrameTypesTable,
							   SdifFrameDataT *FrameData,
							   char *CompoName, SdifMatrixDataT *MatrixData);

SdifMatrixDataT* SdifFrameDataGetNthMatrixData(SdifFrameDataT *FrameData, unsigned int NthMatrix);

SdifMatrixDataT* SdifFrameDataGetComponentMatrixData(SdifHashTableT *FrameTypesTable,
							    SdifFrameDataT *FrameData,
							    char *CompoName);

#endif /* _SdifFrame_ */


