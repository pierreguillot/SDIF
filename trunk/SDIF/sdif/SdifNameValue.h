/* SdifNameValue.h
 *
 *
 *
 *
 */

#ifndef _SdifNameValue_
#define _SdifNameValue_

#include "SdifGlobals.h"

#include "SdifHash.h"

#include <stdio.h>



#define _SdifNameValueHashSize 31


typedef struct SdifNameValueT
{
  char *Name;
  char *Value;
} SdifNameValueType;


extern SdifNameValueType*
SdifCreateNameValue(char *Name,  char *Value);

extern void
SdifKillNameValue(SdifNameValueType *NameValue);

extern int
SdifFWriteOneNameValue(SdifNameValueType *NameValue, FILE *fw);

/* SdifFScanOneNameValue after the NameValuesLType functions 
 */






typedef struct SdifNameValueHTNT SdifNameValueHTNType;

struct SdifNameValueHTNT
{
  SdifNameValueHTNType *Next;
  SdifHashTableType *NameValueHT;
  SdifUInt4 NumHT;
};

extern SdifNameValueHTNType*
SdifCreateNameValueHTN(SdifNameValueHTNType *Next,
		       SdifHashTableType *NameValueHT,
		       SdifUInt4 NumHT);

extern SdifNameValueHTNType*
SdifKillNameValueHTN(SdifNameValueHTNType *NVHTN);







typedef struct SdifNameValuesLT
{
  SdifNameValueHTNType *HeadHTN;
  SdifNameValueHTNType *TailHTN;
  SdifUInt4            NbHTN;
  SdifHashTableType    *CurrHT;
  SdifUInt4            HashSize;
} SdifNameValuesLType;

extern SdifNameValuesLType* gSdifNameValues;

extern SdifNameValuesLType*
SdifCreateNameValuesL(SdifUInt4  HashSize);

extern void
SdifKillNameValuesL(SdifNameValuesLType *NameValuesL);

extern SdifNameValuesLType*
SdifNameValuesLNewHT(SdifNameValuesLType *NameValuesL);

extern SdifHashTableType*
SdifNameValuesLSetCurrHT(SdifNameValuesLType *NameValuesL, SdifUInt4 NumCurrHT);

extern SdifNameValueType*
SdifNameValuesLGet(SdifNameValuesLType *NameValuesL, char *Name);

extern SdifNameValueType*
SdifNameValuesLGetFromCurrHT(SdifNameValuesLType *NameValuesL, char *Name);

extern SdifNameValueType*
SdifNameValuesLPut(SdifNameValuesLType *NameValuesL, char *Name,  char *Value);





extern int
SdifFScanOneNameValue(FILE *fr, int *NbBytesRead);





#endif /* _SdifNameValue_ */


