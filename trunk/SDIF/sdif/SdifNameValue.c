/* $Id: SdifNameValue.c,v 2.1 1998-12-21 18:27:32 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
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
 * Revision 2.0  1998/11/29  11:41:58  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/04/24  12:40:35  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 *
 */


#include "SdifNameValue.h"
#include <stdlib.h>






SdifNameValueT*
SdifCreateNameValue(const char *Name,  const char *Value)
{
  SdifNameValueT *NewNameValue = NULL;

  NewNameValue = (SdifNameValueT*) malloc (sizeof(SdifNameValueT));
  if (NewNameValue)
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
  SdifNameValueHTNT *NewNVHTN = NULL;
  
  NewNVHTN = (SdifNameValueHTNT*) malloc (sizeof(SdifNameValueHTNT));
  if (NewNVHTN)
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
  SdifNameValuesLT *NewNameValuesL = NULL;
  
  NewNameValuesL = (SdifNameValuesLT*) malloc (sizeof(SdifNameValuesLT));
  if (NewNameValuesL)
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
  SdifNameValueHTNT *NewNode = NULL;

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
    {
      NameValue = SdifHashTableSearch(pNode->NameValueHT, Name, SdifStrLen(Name)+1);
      if (NameValue)
        return NameValue;
    }

  return NULL;
}









SdifNameValueT*
SdifNameValuesLGetCurrHT(SdifNameValuesLT *NameValuesL, char *Name)
{
  return SdifHashTableSearch(NameValuesL->CurrHT, Name, SdifStrLen(Name)+1);
}










SdifNameValueT*
SdifNameValuesLPutCurrHT(SdifNameValuesLT *NameValuesL, 
		   const char *Name,  const char *Value)
{
  SdifNameValueT *NewNameValue = NULL;
  
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
