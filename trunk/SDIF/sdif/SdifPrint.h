/* SdifPrint.h
 *
 * for debug
 * print of structures with some reference
 * FILE* is usually stdout or stderr
 *
 */

#ifndef _SdifPrint_
#define _SdifPrint_

#include <stdio.h>
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifMatrix.h"
#include "SdifFrame.h"



/*************** Matrix Type ***************/

extern void
SdifPrintMatrixType(FILE *fw, SdifMatrixTypeType *MatrixType);

extern void
SdifPrintAllMatrixType(FILE *fw);





/*************** Frame Type ***************/

extern void
SdifPrintFrameType(FILE *fw, SdifFrameTypeType *FrameType);

extern void
SdifPrintAllFrameType(FILE *fw);




/********** Matrix **********/

extern void
SdifPrintMatrixHeader(FILE *f, SdifMatrixHeaderType *MatrixHeader);

extern void
SdifPrintOneRow(FILE *f, SdifOneRowType *OneRow);

extern void
SdifPrintMatrixRows(FILE* f, SdifMatrixDataType *MatrixData);

extern void
SdifPrintMatrixData(FILE* f, SdifMatrixDataType *MtrxD);





/********** Frame ***********/

extern void
SdifPrintFrameHeader(FILE *f, SdifFrameHeaderType* FrameHeader);

extern void
SdifPrintFrameData(FILE *f, SdifFrameDataType *FrameData);





/************ High ***********/

extern void
SdifPrintAllType(FILE *fw);

#endif /* _SdifPrint_ */
