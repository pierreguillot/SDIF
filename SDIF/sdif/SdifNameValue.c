/* $Id: SdifNameValue.c,v 1.3 1998-04-24 12:40:35 schwarz Exp $
 *
 * SdifNameValue.c
 *
 * Name values management. For 1 SdifFileT*, we have one SdifNameValueLT*
 * which contains a list of hash tables. Each hach table contains name-values.
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */


#include "SdifNameValue.h"
#include <stdlib.h>






SdifNameValueT*
SdifCreateNameValue(const char *Name,  const char *Value)
{
  SdifNameValueT *NewNameValue;

  if (NewNameValue = (SdifNameValueT*) malloc (sizeof(SdifNameValueT)))
    {
      NewNameValue->Name  = SdifCreateStrNCpy(Name, SdifStrLen(Name)+1);
      NewNameValue->Value = SdifCreateStrNCpy(Value, SdifStrLen(Value)+1);

      return NewNameValue;
    }
  else
    {
      _SdifError(eAllocFail, "NameValue allocation");
      return NULL;
    }
}






void
SdifKillNameValue(SdifNameValueT *NameValue)
{
  if (NameValue)
    {
      SdifKillStr(NameValue->Name);
      SdifKillStr(NameValue->Value);
      free(NameValue);
    }
  else
    _SdifError(eFreeNull, "NameValue free");
}





SdifNameValueHTNT*
SdifCreateNameValueHTN(SdifNameValueHTNT *Next,
		       SdifHashTableT *NameValueHT,
		       SdifUInt4 NumHT)
{
  SdifNameValueHTNT *NewNVHTN;
  
  if (NewNVHTN = (SdifNameValueHTNT*) malloc (sizeof(SdifNameValueHTNT)))
    {
      NewNVHTN->Next = Next;
      NewNVHTN->NameValueHT = NameValueHT;
      NewNVHTN->NumHT = NumHT;
      
      return NewNVHTN;
    }
  else
    {
      _SdifError(eAllocFail, "NameValueHTN allocation");
      return NULL;
    }
}




SdifNameValueHTNT*
SdifKillNameValueHTN(SdifNameValueHTNT *NVHTN)
{
  SdifNameValueHTNT *Next;
  
  if (NVHTN)
    {
      Next = NVHTN->Next;
      SdifKillHashTable(NVHTN->NameValueHT);
      free(NVHTN);
      
      return Next;
    }
  else
    {
      _SdifError(eFreeNull, "NameValueHTN free");
      return NULL;
    }
}






SdifNameValuesLT*
SdifCreateNameValuesL(SdifUInt4  HashSize)
{
  SdifNameValuesLT *NewNameValuesL;
  
  if (NewNameValuesL = (SdifNameValuesLT*) malloc (sizeof(SdifNameValuesLT)))
    {
      NewNameValuesL->HeadHTN = NULL;
      NewNameValuesL->TailHTN = NULL;
      NewNameValuesL->NbHTN = 0;
      NewNameValuesL->CurrHT = NULL;
      NewNameValuesL->HashSize = HashSize;
      
      return NewNameValuesL;
    }
  else
    {
      _SdifError(eAllocFail, "NameValues List allocation");
      return NULL;
    }
}











void
SdifKillNameValuesL(SdifNameValuesLT *NameValuesL)
{
  if (NameValuesL)
    {
      while (NameValuesL->HeadHTN)
	NameValuesL->HeadHTN = SdifKillNameValueHTN(NameValuesL->HeadHTN);
      
      free(NameValuesL);
    }
  else
    _SdifError(eFreeNull, "NameValues List free");
}














SdifNameValuesLT*
SdifNameValuesLNewHT(SdifNameValuesLT *NameValuesL)
{
  SdifNameValueHTNT
    *NewNode;

  NewNode = SdifCreateNameValueHTN(NULL,
				   SdifCreateHashTable(NameValuesL->HashSize,
						       eHashChar,
						       SdifKillNameValue),
				   ++NameValuesL->NbHTN);

  if (! NameValuesL->HeadHTN)
    NameValuesL->HeadHTN = NewNode;
  else
    NameValuesL->TailHTN->Next = NewNode;
  
  NameValuesL->TailHTN = NewNode;
  NameValuesL->CurrHT = NewNode->NameValueHT;

  return NameValuesL;
}












SdifHashTableT*
SdifNameValuesLSetCurrHT(SdifNameValuesLT *NameValuesL, SdifUInt4 NumCurrHT)
{
  SdifNameValueHTNT 
    *pNode;
  
  for(pNode = NameValuesL->HeadHTN; pNode; pNode = pNode->Next)
    if (pNode->NumHT == NumCurrHT)
      NameValuesL->CurrHT = pNode->NameValueHT;

  return NameValuesL->CurrHT;
}











SdifNameValueT*
SdifNameValuesLGet(SdifNameValuesLT *NameValuesL, char *Name)
{
  SdifNameValueT
    *NameValue;
  SdifNameValueHTNT
    *pNode;

  for(pNode = NameValuesL->HeadHTN; pNode; pNode = pNode->Next)
    if (NameValue = SdifHashTableSearch(pNode->NameValueHT, Name, SdifStrLen(Name)+1))
      return NameValue;

  return NULL;
}









SdifNameValueT*
SdifNameValuesLGetFromCurrHT(SdifNameValuesLT *NameValuesL, char *Name)
{
  return SdifHashTableSearch(NameValuesL->CurrHT, Name, SdifStrLen(Name)+1);
}










SdifNameValueT*
SdifNameValuesLPut(SdifNameValuesLT *NameValuesL, 
		   const char *Name,  const char *Value)
{
  SdifNameValueT
    *NewNameValue;
  
  NewNameValue = SdifCreateNameValue(Name, Value);

  if (NewNameValue)
    SdifHashTablePut(NameValuesL->CurrHT, Name, SdifStrLen(Name)+1, NewNameValue);

  return NewNameValue;
}



SdifUInt2
SdifNameValuesLIsNotEmpty(SdifNameValuesLT *NameValuesL)
{
  SdifNameValueHTNT
    *pNode;
  
  for(pNode = NameValuesL->HeadHTN; pNode; pNode = pNode->Next)
    if (pNode->NameValueHT->NbOfData > 0)
      return 1;

  return 0;
}
