/* SdifNameValue.c
 *
 *
 *
 *
 */


#include "SdifNameValue.h"

#include "SdifRWLowLevel.h"

#include <stdlib.h>
#include <ctype.h>





SdifNameValueType*
SdifCreateNameValue(char *Name,  char *Value)
{
  SdifNameValueType *NewNameValue;

  if (NewNameValue = (SdifNameValueType*) malloc (sizeof(SdifNameValueType)))
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
SdifKillNameValue(SdifNameValueType *NameValue)
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










int
SdifFWriteOneNameValue(SdifNameValueType *NameValue, FILE *fw)
{
  int NbBytesWrite = 0;
  
  NbBytesWrite += SdifFWriteString(NameValue->Name, fw);
  NbBytesWrite += fprintf(fw, "\t");
  NbBytesWrite += SdifFWriteString(NameValue->Value, fw);
  NbBytesWrite += fprintf(fw, ";\n");

  return  NbBytesWrite;
}










SdifNameValueHTNType*
SdifCreateNameValueHTN(SdifNameValueHTNType *Next,
		       SdifHashTableType *NameValueHT,
		       SdifUInt4 NumHT)
{
  SdifNameValueHTNType *NewNVHTN;
  
  if (NewNVHTN = (SdifNameValueHTNType*) malloc (sizeof(SdifNameValueHTNType)))
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











SdifNameValueHTNType*
SdifKillNameValueHTN(SdifNameValueHTNType *NVHTN)
{
  SdifNameValueHTNType *Next;
  
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














SdifNameValuesLType*
SdifCreateNameValuesL(SdifUInt4  HashSize)
{
  SdifNameValuesLType *NewNameValuesL;
  
  if (NewNameValuesL = (SdifNameValuesLType*) malloc (sizeof(SdifNameValuesLType)))
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
SdifKillNameValuesL(SdifNameValuesLType *NameValuesL)
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














SdifNameValuesLType*
SdifNameValuesLNewHT(SdifNameValuesLType *NameValuesL)
{
  SdifNameValueHTNType
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












SdifHashTableType*
SdifNameValuesLSetCurrHT(SdifNameValuesLType *NameValuesL, SdifUInt4 NumCurrHT)
{
  SdifNameValueHTNType 
    *pNode;
  
  for(pNode = NameValuesL->HeadHTN; pNode; pNode = pNode->Next)
    if (pNode->NumHT == NumCurrHT)
      NameValuesL->CurrHT = pNode->NameValueHT;

  return NameValuesL->CurrHT;
}











SdifNameValueType*
SdifNameValuesLGet(SdifNameValuesLType *NameValuesL, char *Name)
{
  SdifNameValueType
    *NameValue;
  SdifNameValueHTNType
    *pNode;

  for(pNode = NameValuesL->HeadHTN; pNode; pNode = pNode->Next)
    if (NameValue = SdifHashTableSearch(pNode->NameValueHT, Name, SdifStrLen(Name)+1))
      return NameValue;

  return NULL;
}









SdifNameValueType*
SdifNameValuesLGetFromCurrHT(SdifNameValuesLType *NameValuesL, char *Name)
{
  return SdifHashTableSearch(NameValuesL->CurrHT, Name, SdifStrLen(Name)+1);
}










SdifNameValueType*
SdifNameValuesLPut(SdifNameValuesLType *NameValuesL, char *Name,  char *Value)
{
  SdifNameValueType
    *NewNameValue;
  
  NewNameValue = SdifCreateNameValue(Name, Value);

  if (NewNameValue)
    SdifHashTablePut(NameValuesL->CurrHT, Name, SdifStrLen(Name)+1, NewNameValue);

  return NewNameValue;
}











int
SdifFScanOneNameValue(FILE *fr, int *NbBytesRead)
{
  int
    CharEnd;
  static char 
    CharsEnd[] = {' ', '\t', '\n', '\f', '\r', '\v', '{', '}', ',', ';',':','\0'};

  
  /* Name */
  CharEnd = SdifFGetStringUntil(gSdifString, _SdifStringLen, fr, NbBytesRead, CharsEnd);

  if ( (CharEnd == '}') && (SdifStrLen(gSdifString) == 0) ) /* no more NameValue */
    return  CharEnd;
  if (! isspace(CharEnd))
    {
      sprintf(gSdifErrorMess,
	      "Wait a space_char after \"%s\", read char : '%c' = %d",
	      gSdifString,
	      CharEnd,
	      CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return  CharEnd;
    }
  
  if (SdifNameValuesLGet(gSdifNameValues, gSdifString))
    {
      sprintf(gSdifErrorMess, "NameValue : %s ", gSdifString);
      _SdifError(eReDefined, gSdifErrorMess);
      CharEnd = SdifFGetStringUntil(gSdifString, _SdifStringLen, fr, NbBytesRead, ";");
      return  CharEnd;
    }
  
  
  
  /* Value */
  CharEnd = SdifFGetStringUntil(gSdifString2, _SdifStringLen, fr, NbBytesRead, _SdifReservedChars);

  if (CharEnd != (unsigned) ';')
    {
      sprintf(gSdifErrorMess,
	      "Attempt to read ';' : \"%s%c\" ",
	      gSdifString2,
	      CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return  CharEnd;
    }
  
  SdifNameValuesLPut(gSdifNameValues, gSdifString, gSdifString2);
  return  CharEnd;
}
