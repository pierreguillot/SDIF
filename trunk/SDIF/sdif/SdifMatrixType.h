/* SdifMatrixType.h
 *
 * Matrix Types management (interpreted sdif frame types)
 *
 * author: Dominique Virolle 1997
 *
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




extern SdifColumnDefT*  SdifCreateColumnDef (char *Name,  unsigned int Num);
extern void             SdifKillColumnDef   (SdifColumnDefT *ColumnDef);

extern SdifColumnDefNT* SdifCreateColumnDefN (SdifColumnDefNT *Next, SdifColumnDefT *ColumnDef);
extern SdifColumnDefNT* SdifKillColumnDefN   (SdifColumnDefNT *ColDefNode);

extern SdifMatrixTypeT* SdifCreateMatrixType              (SdifSignature Signature,
							   SdifMatrixTypeT *PredefinedMatrixType);
extern void             SdifKillMatrixType                (SdifMatrixTypeT *MatrixType);
extern SdifMatrixTypeT* SdifMatrixTypeInsertTailColumnDef (SdifMatrixTypeT *MatrixType, char *NameCD);
extern SdifUInt4        SdifMatrixTypeGetNumColumnDef     (SdifMatrixTypeT *MatrixType, char *NameCD);
extern SdifColumnDefT*  SdifMatrixTypeGetColumnDef        (SdifMatrixTypeT *MatrixType, char *NameCD);
extern SdifColumnDefT*  SdifMatrixTypeGetNthColumnDef     (SdifMatrixTypeT *MatrixType, SdifUInt4 NumCD);

extern SdifMatrixTypeT* SdifGetMatrixType(SdifHashTableT *MatrixTypesTable, SdifSignature Signature);
extern void             SdifPutMatrixType(SdifHashTableT *MatrixTypesTable, SdifMatrixTypeT* MatrixType);
extern SdifUInt2        SdifExistUserMatrixType(SdifHashTableT *MatrixTypesTable);

#endif /* _SdifMatrixType_  */
