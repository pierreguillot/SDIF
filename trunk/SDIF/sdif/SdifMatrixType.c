/* SdifMatrixType.c
 *
 * Matrix Types management (interpreted sdif frame types)
 *  
 * author: Dominique Virolle 1997
 *
 *
 * SdifHashTableType to stock Matrix Types
 *      index : matrix type signature (HashInt4)
 *       Data : matrix type (SdifMatrixTypeT*)
 *     Killer : SdifKillMatrixType (if types are coded, Killer  == NULL)
 */


#include "SdifMatrixType.h"

#include <stdlib.h>





SdifColumnDefT*
SdifCreateColumnDef(char *Name, SdifUInt4 Num)
{
  SdifColumnDefT *NewColumnDef;
  
  NewColumnDef = (SdifColumnDefT*) malloc (sizeof(SdifColumnDefT));
  if (NewColumnDef != NULL)
    {
      NewColumnDef->Name = SdifCreateStrNCpy(Name, SdifStrLen(Name)+1);
      NewColumnDef->Num = Num;
      
      return NewColumnDef;
    }
  else
    {
      _SdifError(eAllocFail, "ColumnDef allocation");
      return NULL;
    }
}






void
SdifKillColumnDef(SdifColumnDefT *ColumnDef)
{
  if (ColumnDef)
    {
      SdifKillStr(ColumnDef->Name);
      free(ColumnDef);
    }
  else
    _SdifError(eFreeNull, "ColumnDef free");
}







SdifColumnDefNT*
SdifCreateColumnDefN(SdifColumnDefNT *Next, SdifColumnDefT *ColumnDef)
{
  SdifColumnDefNT *NewColDefN;

  if (NewColDefN = (SdifColumnDefNT*) malloc (sizeof(SdifColumnDefNT)))
    {
      NewColDefN->Next = Next;
      NewColDefN->ColumnDef = ColumnDef;

      return NewColDefN;
    }
  else
    {
      _SdifError(eAllocFail, "ColumnDefN allocation");
      return NULL;
    }
}






SdifColumnDefNT*
SdifKillColumnDefN(SdifColumnDefNT *ColDefN)
{
  SdifColumnDefNT *Next;

  if (ColDefN)
    {
      Next = ColDefN->Next;
      SdifKillColumnDef(ColDefN->ColumnDef);
      free(ColDefN);
      
      return Next;
    }
  else
    {
      _SdifError(eFreeNull, "ColumnDefN free");
      return NULL;
    }
}







SdifMatrixTypeT*
SdifCreateMatrixType(SdifSignature Signature, SdifMatrixTypeT *PredefinedMatrixType)
{
  SdifMatrixTypeT *NewMatrixType;

  if (NewMatrixType = (SdifMatrixTypeT*) malloc (sizeof(SdifMatrixTypeT)))
    {
      NewMatrixType->Signature      = Signature;
      NewMatrixType->HeadUse        = NULL;
      NewMatrixType->TailUse        = NULL;
      NewMatrixType->NbColumnDefUse = 0;
      NewMatrixType->ModifMode      = eCanModif;

      if (PredefinedMatrixType)
	{
	  if (PredefinedMatrixType->Signature != Signature)
	    {
	      sprintf(gSdifErrorMess, "'%s'(Pre) != '%s'",
		      SdifSignatureToString(PredefinedMatrixType->Signature),
		      SdifSignatureToString(Signature));
	      _SdifError(eInvalidPreType, gSdifErrorMess);
	    }
	  else
	    {
	      NewMatrixType->MatrixTypePre  = PredefinedMatrixType;
	      NewMatrixType->NbColumnDef    = PredefinedMatrixType->NbColumnDef;
	    }
	}
      else
	{
	  NewMatrixType->MatrixTypePre  = NULL;
	  NewMatrixType->NbColumnDef    = 0;
	}
     
      return NewMatrixType;
    }
  else
    {
      _SdifError(eAllocFail, "MatrixType allocation");
      return NULL;
    }
}






void
SdifKillMatrixType(SdifMatrixTypeT *MatrixType)
{

  /* don't kill the predfined type because it is one of another bank of types (Predefined Types)*/

  if (MatrixType)
    {
      while (MatrixType->HeadUse)
	MatrixType->HeadUse = SdifKillColumnDefN(MatrixType->HeadUse);

      free(MatrixType);
    }
  else
    _SdifError(eFreeNull, "MatrixType free");
}







SdifUInt4
SdifMatrixTypeGetNumColumnDef(SdifMatrixTypeT *MatrixType, char *NameCD)
{
  SdifColumnDefNT *Node;
  int Num = 0;

  if (MatrixType->MatrixTypePre)
    Num = SdifMatrixTypeGetNumColumnDef(MatrixType->MatrixTypePre, NameCD);

  if (Num == 0)
    {
      for (Node = MatrixType->HeadUse; Node; Node = Node->Next)
	if (SdifStrCmp (Node->ColumnDef->Name, NameCD) == 0)
	  {
	    Num = Node->ColumnDef->Num;
	    break;
	  }
    }
  
  return Num;
}









SdifColumnDefT*
SdifMatrixTypeGetColumnDef(SdifMatrixTypeT *MatrixType, char *NameCD)
{
  SdifColumnDefNT *Node;
  SdifColumnDefT *ColumnDef = NULL;

  if (MatrixType->MatrixTypePre)
    ColumnDef = SdifMatrixTypeGetColumnDef(MatrixType->MatrixTypePre, NameCD);
  
  if (! ColumnDef)
    {
      for (Node = MatrixType->HeadUse; Node; Node = Node->Next)
	if (SdifStrCmp (Node->ColumnDef->Name, NameCD) == 0)
	  {
	    ColumnDef = Node->ColumnDef;
	    break;
	  }
    }
  
  return ColumnDef;
}






SdifColumnDefT*
SdifMatrixTypeGetNthColumnDef(SdifMatrixTypeT *MatrixType, SdifUInt4 NumCD)
{
  SdifColumnDefNT *Node;
  SdifColumnDefT *ColumnDef = NULL;

  if (MatrixType->MatrixTypePre)
    ColumnDef = SdifMatrixTypeGetNthColumnDef(MatrixType->MatrixTypePre, NumCD);

  if (! ColumnDef)
    {
      for (Node = MatrixType->HeadUse; Node; Node = Node->Next)
	if (Node->ColumnDef->Num == NumCD)
	  {
	    ColumnDef = Node->ColumnDef;
	    break;
	  }
    }
  
  return ColumnDef;
}






SdifMatrixTypeT*
SdifMatrixTypeInsertTailColumnDef(SdifMatrixTypeT *MatrixType, char *NameCD)
{
  SdifColumnDefNT *NewDefN;
  
  if (SdifMatrixTypeGetColumnDef(MatrixType, NameCD))
    {
      _SdifError(eExistYet, NameCD);
      return NULL;
    }

  switch (MatrixType->ModifMode)
    {
    case eNoModif:
      _SdifError(eNoModifErr, SdifSignatureToString(MatrixType->Signature));
      return NULL;
    case eCanModif:
      /* then add ColumnDef in the Columns list */
      NewDefN = SdifCreateColumnDefN(NULL, SdifCreateColumnDef(NameCD, MatrixType->NbColumnDef+1));

      if (! MatrixType->HeadUse)
	MatrixType->HeadUse = NewDefN;
      else
	MatrixType->TailUse->Next = NewDefN;
      
      MatrixType->TailUse = NewDefN;
      MatrixType->NbColumnDefUse++;
      break;
      
    default :
      return NULL;
    }
  MatrixType->NbColumnDef++;
  return MatrixType;
}







SdifMatrixTypeT*
SdifGetMatrixType(SdifHashTableT *MatrixTypesTable, SdifSignature Signature)
{
  return SdifHashTableSearch(MatrixTypesTable, &Signature, 1);
}


void
SdifPutMatrixType(SdifHashTableT *MatrixTypesTable, SdifMatrixTypeT* MatrixType)
{
  SdifHashTablePut(MatrixTypesTable, &(MatrixType->Signature), 1, MatrixType);
}




SdifUInt2
SdifExistUserMatrixType(SdifHashTableT *MatrixTypesTable)
{
  unsigned int     iName;
  SdifHashNT*      pName;
  SdifMatrixTypeT*  MtrxT;
  
  for(iName=0; iName<MatrixTypesTable->HashSize; iName++)
    for (pName = MatrixTypesTable->Table[iName]; pName;  pName=pName->Next)
      {
	MtrxT = (SdifMatrixTypeT*) pName->Data;
	if (MtrxT->NbColumnDefUse > 0)
	  return 1;
      }
  return 0;
}
