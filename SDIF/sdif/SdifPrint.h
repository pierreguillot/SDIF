/* SdifPrint.h
 *
 * for message at sdif interpretation
 * print of structures with some reference
 * FILE* is usually stdout or stderr
 *
 * author: Dominique Virolle 1997
 *
 */

#ifndef _SdifPrint_
#define _SdifPrint_

#include "SdifGlobals.h"
#include <stdio.h>
#include "SdifNameValue.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifMatrix.h"
#include "SdifFrame.h"
#include "SdifFileStruct.h"



/*************** Matrix Type ***************/

extern void SdifPrintMatrixType(FILE *fw, SdifMatrixTypeT *MatrixType);
extern void SdifPrintAllMatrixType(FILE *fw, SdifFileT *SdifF);

/*************** Frame Type ***************/

extern void SdifPrintFrameType(FILE *fw, SdifFrameTypeT *FrameType);
extern void SdifPrintAllFrameType(FILE *fw, SdifFileT *SdifF);

/********** Matrix **********/

extern void SdifPrintMatrixHeader(FILE *f, SdifMatrixHeaderT *MatrixHeader);
extern void SdifPrintOneRow(FILE *f, SdifOneRowT *OneRow);
extern void SdifPrintMatrixRows(FILE* f, SdifMatrixDataT *MatrixData);

/********** Frame ***********/

extern void SdifPrintFrameHeader(FILE *f, SdifFrameHeaderT* FrameHeader);

/************ High ***********/

extern void SdifPrintAllType(FILE *fw, SdifFileT *SdifF);

#endif /* _SdifPrint_ */
