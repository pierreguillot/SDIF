/* SdifFrameType.c
 *
 * Frame Types management (interpreted sdif frame types)
 *
 * SdifHashTableT to stock Frame Types
 *         index : frame type name (HashChar, use _SdifNameLen for size)
 *          Data : Frame type (SdifFrameTypeT*)
 *        Killer : SdifKillFrameType
 *
 * author: Dominique Virolle 1997
 *
 */



#include "SdifFrameType.h"
#include <stdlib.h>

#include "SdifMatrixType.h"





SdifComponentT*
SdifCreateComponent(SdifSignature MatrixSignature, char *Name, SdifUInt4 Num)
{
  SdifComponentT *NewComponent;

  if (NewComponent = (SdifComponentT*) malloc(sizeof(SdifComponentT)))
    {
      NewComponent->MatrixSignature = MatrixSignature;
      NewComponent->Name = SdifCreateStrNCpy(Name, SdifStrLen(Name)+1);
      NewComponent->Num = Num;
      
      return NewComponent;
    }
  else
    {
      _SdifError(eAllocFail, "Component allocation");
      return NULL;
    }
}







void
SdifKillComponent(SdifComponentT *Component)
{
  if (Component)
    {
      SdifKillStr(Component->Name);
      free(Component);
    }
  else
    _SdifError(eFreeNull, "Component free");
}








SdifComponentNT*
SdifCreateComponentN(SdifComponentNT *Next, SdifComponentT *Component)
{
  SdifComponentNT *NewComponentNode;

  if (NewComponentNode = (SdifComponentNT*) malloc (sizeof(SdifComponentNT)))
    {
      NewComponentNode->Next = Next;
      NewComponentNode->Component = Component;

      return NewComponentNode;
    }
  else
    {
      _SdifError(eAllocFail, "ComponentNode allocation");
      return NULL;
    }
}







SdifComponentNT*
SdifKillComponentN(SdifComponentNT *ComponentNode)
{
  SdifComponentNT *Next;

  if (ComponentNode)
    {
      Next = ComponentNode->Next;
      SdifKillComponent(ComponentNode->Component);
      free(ComponentNode);
      
      return Next;
    }
  else
    {
      _SdifError(eFreeNull, "ComponentNode free");
      return NULL;
    }
}








SdifFrameTypeT*
SdifCreateFrameType(SdifSignature FrameSignature, SdifFrameTypeT *PredefinedFrameType)
{
  SdifFrameTypeT *NewFrameType;
  
  if (NewFrameType = (SdifFrameTypeT*) malloc (sizeof(SdifFrameTypeT)))
    {
      NewFrameType->Signature = FrameSignature;

      NewFrameType->HeadUse = NULL;
      NewFrameType->TailUse = NULL;
      NewFrameType->NbComponentUse   = 0;
      NewFrameType->ModifMode = eCanModif;

      if (PredefinedFrameType)
	{
	  if (PredefinedFrameType->Signature != FrameSignature)
	    {
	      sprintf(gSdifErrorMess, "'%s'(Pre) != '%s'",
		      SdifSignatureToString(PredefinedFrameType->Signature),
		      SdifSignatureToString(FrameSignature));
	      _SdifError(eInvalidPreType, gSdifErrorMess);
	    }
	  else
	    {
	      NewFrameType->FrameTypePre = PredefinedFrameType;
	      NewFrameType->NbComponent  = PredefinedFrameType->NbComponent;
	    }
	}
      else
	{
	  NewFrameType->FrameTypePre = NULL;
	  NewFrameType->NbComponent  = 0;
	}
          
      return NewFrameType;
    }
  else
    {
      _SdifError(eAllocFail, "FrameType allocation");
      return NULL;
    }
}








void
SdifKillFrameType(SdifFrameTypeT *FrameType)
{
  if (FrameType)
    {
      while (FrameType->HeadUse)
	FrameType->HeadUse = SdifKillComponentN(FrameType->HeadUse);

      free(FrameType);
    }
  else
    _SdifError(eFreeNull, "FrameType free");
}










SdifUInt4
SdifFrameTypeGetNumComponent(SdifFrameTypeT *FrameType, char *NameC)
{
  SdifComponentNT *Node;
  SdifUInt4 Num = 0;
  
  if (FrameType->FrameTypePre)
    Num = SdifFrameTypeGetNumComponent(FrameType->FrameTypePre, NameC);


  if (Num == 0)
    {
      for (Node = FrameType->HeadUse; Node; Node = Node->Next)
	if (SdifStrCmp (Node->Component->Name, NameC) == 0)
	  {
	    Num = Node->Component->Num;
	    break;
	  }
    }

  return Num;
}







SdifComponentT*
SdifFrameTypeGetComponent(SdifFrameTypeT *FrameType, char *NameC)
{
  SdifComponentNT *Node;
  SdifComponentT* Component = NULL;

  if (FrameType->FrameTypePre)
    Component = SdifFrameTypeGetComponent(FrameType->FrameTypePre, NameC);

  if (! Component)
    {
      for (Node = FrameType->HeadUse; Node; Node = Node->Next)
	if (SdifStrCmp (Node->Component->Name, NameC) == 0)
	  {
	    Component = Node->Component;
	    break;
	  }
    }

  return Component;
}






SdifComponentT*
SdifFrameTypeGetNthComponent(SdifFrameTypeT *FrameType, SdifUInt4 NumC)
{
  SdifComponentNT *Node;
  SdifComponentT* Component = NULL;

  if (FrameType->FrameTypePre)
    Component = SdifFrameTypeGetNthComponent(FrameType->FrameTypePre, NumC);

  if (! Component)
    {
      for (Node = FrameType->HeadUse; Node; Node = Node->Next)
	if (Node->Component->Num == NumC)
	  {
	    Component = Node->Component;
	    break;
	  }
    }
  return Component;
}






SdifFrameTypeT*
SdifFrameTypeInsertTailComponent(SdifHashTableT *MatrixTypesTable,
				 SdifFrameTypeT *FrameType,
				 SdifSignature MatrixSignature,
				 char *NameC)
{
  SdifComponentNT *NewDefNode;
  
  if (SdifFrameTypeGetComponent(FrameType, NameC))
    {
      _SdifError(eExistYet, NameC);
      return NULL;
    }
  else
    
  if (! SdifGetMatrixType(MatrixTypesTable, MatrixSignature))
    {
      sprintf(gSdifErrorMess, "Matrix Type '%s'", SdifSignatureToString(MatrixSignature));
      _SdifError(eNotFound, gSdifErrorMess);
      return NULL;
    }
  else
    
  switch (FrameType->ModifMode)
    {
    case eNoModif:
      _SdifError(eNoModifErr, SdifSignatureToString(FrameType->Signature));
      return NULL;
      
    case eCanModif:
      NewDefNode = SdifCreateComponentN(NULL,
					SdifCreateComponent(MatrixSignature,
							    NameC,
							    FrameType->NbComponent+1));
      if (! FrameType->HeadUse)
	FrameType->HeadUse = NewDefNode;
      else
	FrameType->TailUse->Next = NewDefNode;
      
      FrameType->TailUse = NewDefNode;
      
      FrameType->NbComponentUse++;
      FrameType->NbComponent++;
      return FrameType;
      
    default :
      return NULL;
    }
}






SdifFrameTypeT*
SdifGetFrameType(SdifHashTableT *FrameTypesTable, SdifSignature FrameSignature)
{
  return SdifHashTableSearch(FrameTypesTable, &FrameSignature, 1);
}




void
SdifPutFrameType(SdifHashTableT *FrameTypesTable, SdifFrameTypeT *FrameType)
{
  SdifHashTablePut(FrameTypesTable, &(FrameType->Signature), 1, FrameType);
}




SdifUInt2
SdifExistUserFrameType(SdifHashTableT *FrameTypesTable)
{
  unsigned int     iName;
  SdifHashNT*      pName;
  SdifFrameTypeT*  FramT;
  
  for(iName=0; iName<FrameTypesTable->HashSize; iName++)
    for (pName = FrameTypesTable->Table[iName]; pName;  pName=pName->Next)
      {
	FramT = (SdifFrameTypeT*) pName->Data;
	if (FramT->NbComponentUse > 0)
	  return 1;
      }
  return 0;
}
