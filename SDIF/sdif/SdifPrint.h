/* $Id: SdifPrint.h,v 2.0 1998-11-29 11:42:02 virolle Exp $
 *
 * SdifPrint.h
 *
 * for message at sdif interpretation
 * print of structures with some reference
 * FILE* is usually stdout or stderr
 *
 * author: Dominique Virolle 1997
 * $Log: not supported by cvs2svn $
 * Revision 1.3  1998/11/10  15:31:54  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
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

void SdifPrintMatrixType(FILE *fw, SdifMatrixTypeT *MatrixType);
void SdifPrintAllMatrixType(FILE *fw, SdifFileT *SdifF);

/*************** Frame Type ***************/

void SdifPrintFrameType(FILE *fw, SdifFrameTypeT *FrameType);
void SdifPrintAllFrameType(FILE *fw, SdifFileT *SdifF);

/********** Matrix **********/

void SdifPrintMatrixHeader(FILE *f, SdifMatrixHeaderT *MatrixHeader);
void SdifPrintOneRow(FILE *f, SdifOneRowT *OneRow);
void SdifPrintMatrixRows(FILE* f, SdifMatrixDataT *MatrixData);

/********** Frame ***********/

void SdifPrintFrameHeader(FILE *f, SdifFrameHeaderT* FrameHeader);

/************ High ***********/

void SdifPrintAllType(FILE *fw, SdifFileT *SdifF);

#endif /* _SdifPrint_ */
