/* SdifMatrix.h
 *
 * Matrix Header, Data, Rows structure management.
 *
 * Memory allocation of SdifOneRowT* depend on size of one granule.
 * if nb rows or data type (float4 or float8),
 * then SdifReInitOneRow makes a realloc. Then think to use it before write.
 *
 * author: Dominique Virolle 1997
 *
 */

#ifndef _SdifMatrix_
#define _SdifMatrix_

#include "SdifGlobals.h"
#include "SdifMatrixType.h"

typedef struct SdifMatrixHeaderS
{
  SdifSignature  Signature;
  SdifDataTypeET DataType; /* Low level data type */
  SdifUInt4      NbRow;
  SdifUInt4      NbCol;
} SdifMatrixHeaderT;

typedef union DataTypeU
{
  SdifFloat4 *F4;
  SdifFloat8 *F8;
} DataTypeUT;


typedef struct SdifOneRowS
{
  SdifDataTypeET DataType;
  SdifUInt4      NbData;
  DataTypeUT     Data;
  SdifUInt4      NbGranuleAlloc;
} SdifOneRowT;

typedef struct SdifMatrixDataS
{
  SdifMatrixHeaderT *Header;
  SdifOneRowT       **Rows;
  SdifUInt4         Size;
} SdifMatrixDataT;



extern SdifMatrixHeaderT* SdifCreateMatrixHeader    (SdifSignature Signature, SdifUInt4 DataType,
						     SdifUInt4 NbRow, SdifUInt4 NbCol);

extern SdifMatrixHeaderT* SdifCreateMatrixHeaderEmpty (void);
extern void               SdifKillMatrixHeader        (SdifMatrixHeaderT *MatrixHeader);

extern SdifOneRowT*       SdifCreateOneRow          (SdifDataTypeET DataType, SdifUInt4  NbGranuleAlloc);
extern SdifOneRowT*       SdifReInitOneRow          (SdifOneRowT *OneRow, SdifDataTypeET DataType, SdifUInt4 NbData);
extern void               SdifKillOneRow            (SdifOneRowT *OneRow);
extern SdifOneRowT*       SdifOneRowPutValue        (SdifOneRowT *OneRow, SdifUInt4 numCol, SdifFloat8 Value);
extern SdifFloat8         SdifOneRowGetValue        (SdifOneRowT *OneRow, SdifUInt4 numCol);
extern SdifFloat8         SdifOneRowGetValueColName (SdifOneRowT *OneRow, SdifMatrixTypeT *MatrixType, char * NameCD);

extern SdifMatrixDataT*   SdifCreateMatrixData      (SdifSignature Signature, SdifDataTypeET DataType,
						     SdifUInt4 NbRow, SdifUInt4 NbCol);

extern void               SdifKillMatrixData        (SdifMatrixDataT *MatrixData);
extern SdifMatrixDataT*   SdifMatrixDataPutValue    (SdifMatrixDataT *MatrixData,
						     SdifUInt4  numRow, SdifUInt4  numCol, SdifFloat8 Value);

extern SdifFloat8         SdifMatrixDataGetValue    (SdifMatrixDataT *MatrixData,
						     SdifUInt4  numRow, SdifUInt4  numCol);

#endif /* _SdifMatrix_ */
