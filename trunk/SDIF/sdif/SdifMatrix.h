/* $Id: SdifMatrix.h,v 1.4 1998-11-10 15:31:51 schwarz Exp $
 *
 * SdifMatrix.h
 *
 * Matrix Header, Data, Rows structure management.
 *
 * Memory allocation of SdifOneRowT* depend on size of one granule.
 * if nb rows or data type (float4 or float8),
 * then SdifReInitOneRow makes a realloc. Then think to use it before write.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */

#ifndef _SdifMatrix_
#define _SdifMatrix_

#include "SdifGlobals.h"
#include "SdifMatrixType.h"



typedef struct SdifMatrixHeaderS SdifMatrixHeaderT;

struct SdifMatrixHeaderS
{
  SdifSignature  Signature;
  SdifDataTypeET DataType; /* Low level data type */
  SdifUInt4      NbRow;
  SdifUInt4      NbCol;
} ;




typedef union DataTypeU DataTypeUT;

union DataTypeU
{
  SdifFloat4 *F4;
  SdifFloat8 *F8;
} ;




typedef struct SdifOneRowS SdifOneRowT;

struct SdifOneRowS
{
  SdifDataTypeET DataType;
  SdifUInt4      NbData;
  DataTypeUT     Data;
  SdifUInt4      NbGranuleAlloc;
} ;



typedef struct SdifMatrixDataS SdifMatrixDataT;
struct SdifMatrixDataS
{
  SdifMatrixHeaderT *Header;
  SdifOneRowT       **Rows;
  SdifUInt4         Size;
} ;



SdifMatrixHeaderT* SdifCreateMatrixHeader    (SdifSignature Signature, 
					      SdifUInt4 DataType,
					      SdifUInt4 NbRow, 
					      SdifUInt4 NbCol);
SdifMatrixHeaderT* SdifCreateMatrixHeaderEmpty (void);
void               SdifKillMatrixHeader      (SdifMatrixHeaderT *MatrixHeader);

SdifOneRowT*       SdifCreateOneRow          (SdifDataTypeET DataType, 
					      SdifUInt4  NbGranuleAlloc);
SdifOneRowT*       SdifReInitOneRow          (SdifOneRowT *OneRow, 
					      SdifDataTypeET DataType, 
					      SdifUInt4 NbData);
void               SdifKillOneRow            (SdifOneRowT *OneRow);
SdifOneRowT*       SdifOneRowPutValue        (SdifOneRowT *OneRow, 
					      SdifUInt4 numCol, 
					      SdifFloat8 Value);
SdifFloat8         SdifOneRowGetValue        (SdifOneRowT *OneRow, 
					      SdifUInt4 numCol);
SdifFloat8         SdifOneRowGetValueColName (SdifOneRowT *OneRow, 
					      SdifMatrixTypeT *MatrixType, 
					      char * NameCD);

SdifMatrixDataT*   SdifCreateMatrixData      (SdifSignature Signature, 
					      SdifDataTypeET DataType,
					      SdifUInt4 NbRow, 
					      SdifUInt4 NbCol);

void               SdifKillMatrixData        (SdifMatrixDataT *MatrixData);
SdifMatrixDataT*   SdifMatrixDataPutValue    (SdifMatrixDataT *MatrixData,
					      SdifUInt4  numRow, 
					      SdifUInt4  numCol, 
					      SdifFloat8 Value);

SdifFloat8         SdifMatrixDataGetValue    (SdifMatrixDataT *MatrixData,
					      SdifUInt4  numRow, 
					      SdifUInt4  numCol);

#endif /* _SdifMatrix_ */
