/* $Id: SdifNameValue.c,v 3.1 1999-03-14 10:57:12 virolle Exp $
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
 * Revision 2.4  1999/02/28  12:16:54  virolle
 * memory report
 *
 * Revision 2.3  1999/01/23  15:55:56  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:41  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:32  schwarz
 * Inserted copyright message.
 *
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




/*
 * NameValue
 */

SdifNameValueT*
SdifCreateNameValue(const char *Name,  const char *Value)
{
  SdifNameValueT *NewNameValue = NULL;

  NewNameValue = SdifMalloc(SdifNameValueT);
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
      SdifFree(NameValue);
    }
  else
    _SdifError(eFreeNull, "NameValue free");
}






/*
 * NameValueTable
 */



SdifNameValueTableT*
SdifCreateNameValueTable (SdifUInt4 NumIDLink,
                          SdifFloat8 Time,
                          SdifUInt4 HashSize,
                          SdifUInt4 NumTable)
{
    SdifNameValueTableT* NewNVTable;

    NewNVTable = SdifMalloc(SdifNameValueTableT);
    if (NewNVTable)
    {
        NewNVTable->NumIDLink  = NumIDLink;
        NewNVTable->Time       = Time;
        NewNVTable->NVHT       = SdifCreateHashTable(HashSize, eHashChar, SdifKillNameValue);
        NewNVTable->NumTable   = NumTable;
        return NewNVTable;
    }
    else
    {
      _SdifError(eAllocFail, "NameValueTable allocation");
      return NULL;
    }
}



void
SdifKillNameValueTable  (SdifNameValueTableT* NVTable)
{
  if (NVTable)
    {
      SdifKillHashTable(NVTable->NVHT);
      SdifFree(NVTable);
    }
  else
    _SdifError(eFreeNull, "NameValueTable free");
}





SdifNameValueT*
SdifNameValueTableGetNV(SdifNameValueTableT* NVTable, const char *Name)
{
    return (SdifNameValueT*) SdifHashTableSearch(NVTable->NVHT, (char*) Name, SdifStrLen(Name)+1);
}



SdifNameValueT*
SdifNameValueTablePutNV(SdifNameValueTableT* NVTable, const char *Name,  const char *Value)
{
  SdifNameValueT *NewNameValue = NULL;
  
  NewNameValue = SdifCreateNameValue(Name, Value);

  if (NewNameValue)
    SdifHashTablePut(NVTable->NVHT, Name, SdifStrLen(Name)+1, NewNameValue);

  return NewNameValue;
}


SdifFloat8
SdifNameValueTableGetTime(SdifNameValueTableT* NVTable)
{
    return NVTable->Time;
}



SdifUInt4
SdifNameValueTableGetNumTable(SdifNameValueTableT* NVTable)
{
    return NVTable->NumTable;
}


SdifUInt4
SdifNameValueTableGetNumIDLink(SdifNameValueTableT* NVTable)
{
    return NVTable->NumIDLink;
}






/*
 * NameValueTableList
 */




SdifNameValuesLT*
SdifCreateNameValuesL(SdifUInt4  HashSize)
{
  SdifNameValuesLT *NewNameValuesL = NULL;
  
  NewNameValuesL = SdifMalloc(SdifNameValuesLT);

  if (NewNameValuesL)
    {
      NewNameValuesL->NVTList   = SdifCreateList(SdifKillNameValueTable);
      NewNameValuesL->CurrNVT   = NULL;
      NewNameValuesL->HashSize  = HashSize;
      
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
      SdifKillList(NameValuesL->NVTList);
      SdifFree(NameValuesL);
    }
  else
    _SdifError(eFreeNull, "NameValues List free");
}





SdifNameValuesLT*
SdifNameValuesLNewTable(SdifNameValuesLT *NameValuesL, SdifUInt4 NumIDLink, SdifFloat8 Time)
{
    SdifNameValueTableT* NewNVT;

    NewNVT = SdifCreateNameValueTable(  NumIDLink,
                                        Time,
                                        NameValuesL->HashSize,
                                        SdifListGetNbData(NameValuesL->NVTList) + 1);

    SdifListPutTail(NameValuesL->NVTList, NewNVT);

    NameValuesL->CurrNVT = NewNVT;
    
    return NameValuesL;
}




SdifNameValueTableT*
SdifNameValuesLSetCurrNVT(SdifNameValuesLT *NameValuesL, SdifUInt4 NumCurrNVT)
/* condition of succes : NumCurrNVT < SdifListGetNbData(NameValuesL->NVTList)
 */
{
    SdifNameValueTableT* NVT=NULL;

    NVT = SdifListGetCurr(NameValuesL->NVTList);
    if (NVT)
    {
        if (    (NVT->NumTable == NumCurrNVT-1)
             && (NumCurrNVT < SdifListGetNbData(NameValuesL->NVTList))    )
        {
            NameValuesL->CurrNVT = SdifListGetNext(NameValuesL->NVTList);
            return NameValuesL->CurrNVT;
        }
    }


    NVT=NULL;
    SdifListInitLoop(NameValuesL->NVTList);
    while (SdifListIsNext(NameValuesL->NVTList))
    {
        NVT = SdifListGetNext(NameValuesL->NVTList);
        if (NVT->NumTable = NumCurrNVT)
        {
            NameValuesL->CurrNVT = NVT;
            break;
        }
    }
  return NameValuesL->CurrNVT;
}




SdifNameValueT*
SdifNameValuesLGet(SdifNameValuesLT *NameValuesL, char *Name)
{
  SdifNameValueT
    *NameValue = NULL;
  SdifNameValueTableT * CurrTable;

  SdifListInitLoop(NameValuesL->NVTList);
  while (   (!NameValue)  && (SdifListIsNext(NameValuesL->NVTList)) )
  {
      CurrTable = SdifListGetNext(NameValuesL->NVTList);
      NameValue = SdifNameValueTableGetNV(CurrTable, Name);
  }

  return NameValue;
}



SdifNameValueT*
SdifNameValuesLGetCurrNVT(SdifNameValuesLT *NameValuesL, const char *Name)
{
  return SdifNameValueTableGetNV(NameValuesL->CurrNVT, Name);
}



SdifNameValueT*
SdifNameValuesLPutCurrNVT(SdifNameValuesLT *NameValuesL, 
		   const char *Name,  const char *Value)
{
    return SdifNameValueTablePutNV(NameValuesL->CurrNVT, Name, Value);
}



SdifUInt2
SdifNameValuesLIsNotEmpty(SdifNameValuesLT *NameValuesL)
{
    SdifNameValueTableT* NVT;

    if (SdifListIsEmpty(NameValuesL->NVTList))
        return 0;
    else
    {
        SdifListInitLoop(NameValuesL->NVTList);
        while (SdifListIsNext(NameValuesL->NVTList))
        {
            NVT = SdifListGetNext(NameValuesL->NVTList);
            if (NVT->NVHT->NbOfData > 0)
                return 1;
        }
    }
  return 0;
}




/*
 * Obsolete
 */



SdifNameValuesLT*
SdifNameValuesLNewHT(SdifNameValuesLT *NameValuesL)
{
    /* obsolete */
    _Debug("SdifNameValuesLNewHT is obsolete, use SdifNameValuesLNewTable(<args have changed!!>) ");
    return SdifNameValuesLNewTable(NameValuesL, _SdifNoStreamID, _Sdif_MIN_DOUBLE_);
}



SdifHashTableT*
SdifNameValuesLSetCurrHT(SdifNameValuesLT *NameValuesL, SdifUInt4 NumCurrHT)
{
    /* obsolete */
    SdifNameValueTableT* NVT=NULL;

    _Debug("SdifNameValuesLSetCurrHT is obsolete, use SdifNameValuesLSetCurrNVT(<same args>) : result has changed ");

    NVT = SdifNameValuesLSetCurrNVT(NameValuesL, NumCurrHT);
    return NVT->NVHT;
}



SdifNameValueT*
SdifNameValuesLGetCurrHT(SdifNameValuesLT *NameValuesL, char *Name)
{
    /* obsolete */
    _Debug("SdifNameValuesLGetCurrHT is obsolete, use SdifNameValuesLGetCurrNVT");
    return SdifNameValuesLGetCurrNVT(NameValuesL, Name);
}


SdifNameValueT*
SdifNameValuesLPutCurrHT(SdifNameValuesLT *NameValuesL, 
		   const char *Name,  const char *Value)
{
    /* obsolete */
    _Debug("SdifNameValuesLPutCurrHT is obsolete, use SdifNameValuesLPutCurrNVT");
    return SdifNameValuesLPutCurrNVT(NameValuesL, Name, Value);
}


