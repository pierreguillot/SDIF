/* $Id: SdifMatrixType.h,v 1.4 1998-11-10 15:31:52 schwarz Exp $
 *
 * SdifMatrixType.h
 *
 * Matrix Types management (interpreted sdif frame types)
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */


#ifndef _SdifMatrixType_
#define _SdifMatrixType_

#include "SdifGlobals.h"
#include "SdifHash.h"





typedef struct SdifColumnDefS SdifColumnDefT;

struct SdifColumnDefS
{
  char *Name;
  SdifUInt4 Num;
} ;




typedef struct SdifColumnDefNS SdifColumnDefNT;

struct SdifColumnDefNS
{
  SdifColumnDefNT *Next;
  SdifColumnDefT *ColumnDef;
};




typedef struct SdifMatrixTypeS SdifMatrixTypeT;

struct SdifMatrixTypeS
{
  SdifSignature      Signature;

  SdifMatrixTypeT    *MatrixTypePre;

  SdifColumnDefNT *HeadUse;
  SdifColumnDefNT *TailUse;
  SdifUInt4       NbColumnDefUse;

  SdifUInt4       NbColumnDef;
  SdifModifModeET ModifMode;
};

SdifColumnDefT*  SdifCreateColumnDef  (char *Name,  unsigned int Num);
void             SdifKillColumnDef    (SdifColumnDefT *ColumnDef);
SdifColumnDefNT* SdifCreateColumnDefN (SdifColumnDefNT *Next, 
				       SdifColumnDefT  *ColumnDef);
SdifColumnDefNT* SdifKillColumnDefN   (SdifColumnDefNT *ColDefNode);

SdifMatrixTypeT* SdifCreateMatrixType (SdifSignature   Signature,
				       SdifMatrixTypeT *PredefinedMatrixType);
void             SdifKillMatrixType   (SdifMatrixTypeT *MatrixType);

SdifMatrixTypeT* SdifMatrixTypeInsertTailColumnDef(SdifMatrixTypeT *MatrixType,
						   char *NameCD);
SdifUInt4        SdifMatrixTypeGetNumColumnDef    (SdifMatrixTypeT *MatrixType,
						   char *NameCD);
SdifColumnDefT*  SdifMatrixTypeGetColumnDef       (SdifMatrixTypeT *MatrixType,
						   char *NameCD);
SdifColumnDefT*  SdifMatrixTypeGetNthColumnDef    (SdifMatrixTypeT *MatrixType,
						   SdifUInt4 NumCD);

SdifMatrixTypeT* SdifGetMatrixType       (SdifHashTableT  *MatrixTypesTable, 
					  SdifSignature   Signature);
void             SdifPutMatrixType	 (SdifHashTableT  *MatrixTypesTable, 
				          SdifMatrixTypeT *MatrixType);
SdifUInt2        SdifExistUserMatrixType (SdifHashTableT  *MatrixTypesTable);

#endif /* _SdifMatrixType_  */
