/* SdifMatrixType.h
 *
 *
 *
 */


#ifndef _SdifMatrixType_
#define _SdifMatrixType_

#include "SdifGlobals.h"

#include <stdio.h>

typedef struct SdifColumnDefT
{
  char *Name;
  SdifUInt4 Num;
} SdifColumnDefType;

extern SdifColumnDefType*
SdifCreateColumnDef(char *Name,  unsigned int Num);

extern void
SdifKillColumnDef(SdifColumnDefType *ColumnDef);




typedef struct SdifColumnDefNodeT SdifColumnDefNodeType;

struct SdifColumnDefNodeT
{
  SdifColumnDefNodeType *Next;
  SdifColumnDefType *ColumnDef;
};

extern SdifColumnDefNodeType*
SdifCreateColumnDefNode(SdifColumnDefNodeType *Next, SdifColumnDefType *ColumnDef);

extern SdifColumnDefNodeType*
SdifKillColumnDefNode(SdifColumnDefNodeType *ColDefNode);


typedef struct SdifMatrixTypeT
{
  char Name[_SdifNameLen];

  SdifColumnDefNodeType *HeadColumnDefPre;
  SdifColumnDefNodeType *TailColumnDefPre;
  SdifUInt4             NbColumnDefPre;

  SdifColumnDefNodeType *HeadColumnDefUse;
  SdifColumnDefNodeType *TailColumnDefUse;
  SdifUInt4             NbColumnDefUse;

  SdifUInt4             NbColumnDef;
  SdifPredefinedEnum    Predefined;
} SdifMatrixTypeType;

extern SdifMatrixTypeType*
SdifCreateMatrixType(char *Name,
		     SdifPredefinedEnum Predefined);

extern void
SdifKillMatrixType(SdifMatrixTypeType *MatrixType);

extern SdifMatrixTypeType*
SdifGetMatrixType(char *Name);

extern void
SdifPutMatrixType(char *Name, SdifMatrixTypeType* MatrixType);

extern SdifMatrixTypeType*
SdifMatrixTypeInsertTailColumnDef(SdifMatrixTypeType *MatrixType,
				  char *NameCD,
				  SdifPredefinedEnum Predefined);

extern SdifUInt4
SdifMatrixTypeGetNumColumnDef(SdifMatrixTypeType *MatrixType,
			      char *NameCD);

extern SdifColumnDefType*
SdifMatrixTypeGetColumnDef(SdifMatrixTypeType *MatrixType,
			   char *NameCD);

extern SdifColumnDefType*
SdifMatrixTypeGetNthColumnDef(SdifMatrixTypeType *MatrixType,
			      SdifUInt4 NumCD);

extern int
SdifFWriteOneMatrixType(SdifMatrixTypeType *MatrixType,
			SdifPredefinedEnum Predefined, FILE *fw);

extern int
SdifFWriteAllMatrixType(SdifPredefinedEnum Predefined,
			FILE *fw);

extern int
SdifFReadMatrixType(SdifPredefinedEnum Predefined,
		    FILE *fr);

#endif /* _SdifMatrixType_  */





