/* SdifMatrix.h
 *
 *
 *
 */

#ifndef _SdifMatrix_
#define _SdifMatrix_

#include "SdifGlobals.h"
#include "SdifMatrixType.h"

#include <stdio.h>





typedef struct SdifMatrixHeaderT
{
  char MatrixName[_SdifNameLen];
  SdifDataTypeEnum DataType; /* Low level data type */
  SdifUInt4 NbRow;
  SdifUInt4 NbCol;
} SdifMatrixHeaderType;

extern SdifMatrixHeaderType*
SdifCreateMatrixHeader(char *MatrixName,
		       SdifUInt4 DataType,
		       SdifUInt4 NbRow,
		       SdifUInt4 NbCol);

extern SdifMatrixHeaderType*
SdifCreateMatrixHeaderEmpty(void);

extern void
SdifKillMatrixHeader(SdifMatrixHeaderType *MatrixHeader);

extern int
SdifFReadMatrixHeader(SdifMatrixHeaderType* MatrixHeader, FILE *fr);

extern int
SdifFPrintMatrixHeader(FILE *f, SdifMatrixHeaderType *MatrixHeader);

extern void
SdifFScanMatrixHeader(FILE *fr, char *MatrixName, SdifMatrixHeaderType* MatrixHeader);

extern int
SdifFWriteMatrixHeader(SdifMatrixHeaderType* MatrixHeader, FILE *fw);



typedef union DataTypeU
{
  SdifFloat4 *F4;
  SdifFloat8 *F8;
} DataTypeUnion;


typedef struct SdifOneRowT
{
  SdifDataTypeEnum DataType;
  SdifUInt4  NbData;
  DataTypeUnion Data;
} SdifOneRowType;

extern SdifOneRowType*
SdifCreateOneRow(SdifDataTypeEnum DataType, SdifUInt4  NbData);

extern void
SdifKillOneRow(SdifOneRowType *OneRow);

extern int
SdifFPrintOneRow(FILE *f, SdifOneRowType *OneRow);

extern SdifOneRowType*
SdifOneRowPutValue(SdifOneRowType *OneRow, SdifUInt4 numCol, SdifFloat8 Value);

extern SdifFloat8
SdifOneRowGetValue(SdifOneRowType *OneRow, SdifUInt4 numCol);

extern SdifFloat8
SdifOneRowGetValueColName(SdifOneRowType *OneRow,
			  SdifMatrixTypeType *MatrixType,
			  char * NameCD);

extern int
SdifFReadOneRow(SdifOneRowType *OneRow, FILE* fr);

extern int
SdifFWriteOneRow(SdifOneRowType *OneRow, FILE* fw);




typedef struct SdifMatrixDataT
{
  SdifMatrixHeaderType *Header;
  SdifOneRowType       **Rows;
  SdifUInt4            Size;
} SdifMatrixDataType;

SdifMatrixDataType*
SdifCreateMatrixData(char *MatrixName,
		     SdifDataTypeEnum DataType,
		     SdifUInt4 NbRow,
		     SdifUInt4 NbCol);

extern void
SdifKillMatrixData(SdifMatrixDataType *MatrixData);

extern int
SdifFPrintMatrixRows(FILE* f, SdifMatrixDataType *MatrixData);

extern int
SdifFWriteMatrixRows(SdifMatrixDataType *MatrixData, FILE* fw);

extern int
SdifFReadMatrixRows(SdifMatrixDataType *MatrixData, FILE* fw);

extern int
SdifFWriteMatrixData(SdifMatrixDataType *MatrixData, FILE* fw);

extern SdifMatrixDataType*
SdifMatrixDataPutValue(SdifMatrixDataType *MatrixData,
		       SdifUInt4  numRow,
		       SdifUInt4  numCol,
		       SdifFloat8 Value);

extern SdifFloat8
SdifMatrixDataGetValue(SdifMatrixDataType *MatrixData,
		       SdifUInt4  numRow,
		       SdifUInt4  numCol);

extern SdifMatrixDataType*
SdifMatrixDataColNamePutValue(SdifMatrixDataType *MatrixData,
			      SdifUInt4  numRow,
			      char *ColName,
			      SdifFloat8 Value);

extern SdifFloat8
SdifMatrixDataColNameGetValue(SdifMatrixDataType *MatrixData,
			      SdifUInt4  numRow,
			      char *ColName);

extern int
SdifSkipMatrixData(SdifMatrixHeaderType *MtrxH, FILE *fr);

extern int
SdifSkipMatrix(FILE *fr);

#endif /* _SdifMatrix_ */

