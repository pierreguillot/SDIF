/* $Id: SdifFrameType.c,v 2.1 1998-12-21 18:27:20 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * Frame Types management (interpreted sdif frame types)
 *
 * SdifHashTableT to stock Frame Types
 *         index : frame type name (HashChar, use _SdifNameLen for size)
 *          Data : Frame type (SdifFrameTypeT*)
 *        Killer : SdifKillFrameType
 *
 * Frame Components are into a hash table too
 *
 * author: Dominique Virolle 1997
 *
 */



#include "SdifFrameType.h"
#include <stdlib.h>

#include "SdifMatrixType.h"





SdifComponentT*
SdifCreateComponent(SdifSignature MtrxS, char *Name, SdifUInt4 Num)
{
  SdifComponentT *NewComponent = NULL;

  NewComponent = (SdifComponentT*) malloc (sizeof(SdifComponentT));
  if (NewComponent)
    {
      NewComponent->MtrxS = MtrxS;
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







SdifFrameTypeT*
SdifCreateFrameType(SdifSignature FramS, SdifFrameTypeT *PredefinedFrameType)
{
  SdifFrameTypeT *NewFrameType = NULL;
  
  NewFrameType = (SdifFrameTypeT*) malloc (sizeof(SdifFrameTypeT));
  if (NewFrameType)
    {
      NewFrameType->Signature = FramS;

      NewFrameType->ComponentUseHT = SdifCreateHashTable(13, eHashInt4, SdifKillComponent);
      NewFrameType->NbComponentUse   = 0;
      NewFrameType->ModifMode = eCanModif;

      if (PredefinedFrameType)
      {
	    if (PredefinedFrameType->Signature != FramS)
	    {
	      sprintf(gSdifErrorMess, "'%s'(Pre) != '%s'",
		      SdifSignatureToString(PredefinedFrameType->Signature),
		      SdifSignatureToString(FramS));
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
      SdifKillHashTable(FrameType->ComponentUseHT);
      free(FrameType);
    }
  else
    _SdifError(eFreeNull, "FrameType free");
}





SdifComponentT*
SdifFrameTypeGetComponent_MtrxS(SdifFrameTypeT *FrameType, SdifSignature MtrxS)
{
  SdifComponentT* Component;

  Component = NULL;
  if (FrameType->FrameTypePre)
    Component = SdifFrameTypeGetComponent_MtrxS(FrameType->FrameTypePre, MtrxS);

  if (! Component)
  {
    Component = (SdifComponentT*) SdifHashTableSearch(FrameType->ComponentUseHT,
                                                     &MtrxS,
                                                     1);
  }

  return Component;
}







SdifComponentT*
SdifFrameTypeGetComponent(SdifFrameTypeT *FrameType, char *NameC)
{
  unsigned int     iHTN;
  SdifHashNT*      pHTN;
  SdifHashTableT*  ComponentUseHT = FrameType->ComponentUseHT;
  SdifComponentT*  Component;
  SdifComponentT*  ComponentTmp;

  Component = NULL;
  if (FrameType->FrameTypePre)
    Component = SdifFrameTypeGetComponent(FrameType->FrameTypePre, NameC);

  for(iHTN=0; iHTN<ComponentUseHT->HashSize && !Component; iHTN++)
    for (pHTN = ComponentUseHT->Table[iHTN]; pHTN && !Component;  pHTN=pHTN->Next)
    {
      ComponentTmp = (SdifComponentT*) pHTN->Data;
      if (SdifStrCmp (ComponentTmp->Name, NameC) == 0)
      {
        Component = ComponentTmp;
      }
    }

  return Component;
}




SdifComponentT*
SdifFrameTypeGetNthComponent(SdifFrameTypeT *FrameType, SdifUInt4 NumC)
{
  unsigned int     iHTN;
  SdifHashNT*      pHTN;
  SdifHashTableT*  ComponentUseHT = FrameType->ComponentUseHT;
  SdifComponentT*  Component;
  SdifComponentT*  ComponentTmp;

  Component = NULL;
  if (FrameType->FrameTypePre)
    Component = SdifFrameTypeGetNthComponent(FrameType->FrameTypePre, NumC);

  for(iHTN=0; iHTN<ComponentUseHT->HashSize && !Component; iHTN++)
    for (pHTN = ComponentUseHT->Table[iHTN]; pHTN && !Component;  pHTN=pHTN->Next)
    {
      ComponentTmp = (SdifComponentT*) pHTN->Data;
      if (ComponentTmp->Num == NumC)
      {
        Component = ComponentTmp;
      }
    }

  return Component;
}



SdifFrameTypeT*
SdifFrameTypePutComponent(SdifFrameTypeT *FrameType,
                          SdifSignature MtrxS,
                          char *NameC)
{
  SdifComponentT *NewComponent = NULL;



  if (SdifFrameTypeGetComponent_MtrxS(FrameType, MtrxS))
    {
      _SdifError(eExistYet, SdifSignatureToString(MtrxS));
      return NULL;
    }
  else
    if (SdifFrameTypeGetComponent(FrameType, NameC))
      {
        _SdifError(eExistYet, NameC);
        return NULL;
      }
    else
    
  switch (FrameType->ModifMode)
    {
    case eNoModif:
      _SdifError(eNoModifErr, SdifSignatureToString(FrameType->Signature));
      return NULL;
      
    case eCanModif:
      NewComponent = SdifCreateComponent(MtrxS, NameC, FrameType->NbComponent+1);
      SdifHashTablePut(FrameType->ComponentUseHT, &MtrxS, 1, NewComponent);
      
      FrameType->NbComponentUse++;
      FrameType->NbComponent++;
      return FrameType;
      
    default :
      return NULL;
    }
}






SdifFrameTypeT*
SdifGetFrameType(SdifHashTableT *FrameTypeHT, SdifSignature FramS)
{
  return SdifHashTableSearch(FrameTypeHT, &FramS, 1);
}




void
SdifPutFrameType(SdifHashTableT *FrameTypeHT, SdifFrameTypeT *FrameType)
{
  SdifHashTablePut(FrameTypeHT, &(FrameType->Signature), 1, FrameType);
}







SdifUInt2
SdifExistUserFrameType(SdifHashTableT *FrameTypeHT)
{
  unsigned int     iName;
  SdifHashNT*      pName;
  SdifFrameTypeT*  FramT;
  
  for(iName=0; iName<FrameTypeHT->HashSize; iName++)
    for (pName = FrameTypeHT->Table[iName]; pName;  pName=pName->Next)
      {
        FramT = (SdifFrameTypeT*) pName->Data;
	    if (FramT->NbComponentUse > 0)
	      return 1;
      }
  return 0;
}
