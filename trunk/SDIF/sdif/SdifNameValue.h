/* SdifNameValue.h
 *
 * Name values management. For 1 SdifFileT*, we have one SdifNameValueLT*
 * which contains a list of hash tables. Each hach table contains name-values.
 *
 *
 * author: Dominique Virolle 1997
 *
 */

#ifndef _SdifNameValue_
#define _SdifNameValue_


#include "SdifGlobals.h"
#include "SdifHash.h"

#define _SdifNameValueHashSize 31


typedef struct SdifNameValueS
{
  char *Name;
  char *Value;
} SdifNameValueT;


extern SdifNameValueT* SdifCreateNameValue(char *Name,  char *Value);
extern void            SdifKillNameValue  (SdifNameValueT *NameValue);



typedef struct SdifNameValueHTNS SdifNameValueHTNT;

struct SdifNameValueHTNS
{
  SdifNameValueHTNT *Next;
  SdifHashTableT *NameValueHT;
  SdifUInt4 NumHT;
};

extern SdifNameValueHTNT* SdifCreateNameValueHTN(SdifNameValueHTNT *Next,
						 SdifHashTableT *NameValueHT,
						 SdifUInt4 NumHT);
extern SdifNameValueHTNT* SdifKillNameValueHTN  (SdifNameValueHTNT *NVHTN);



typedef struct SdifNameValuesLS
{
  SdifNameValueHTNT *HeadHTN;
  SdifNameValueHTNT *TailHTN;
  SdifUInt4          NbHTN;
  SdifHashTableT    *CurrHT;
  SdifUInt4          HashSize;
} SdifNameValuesLT;

extern SdifNameValuesLT* SdifCreateNameValuesL       (SdifUInt4  HashSize);
extern void              SdifKillNameValuesL         (SdifNameValuesLT *NameValuesL);
extern SdifNameValuesLT* SdifNameValuesLNewHT        (SdifNameValuesLT *NameValuesL);
extern SdifHashTableT*   SdifNameValuesLSetCurrHT    (SdifNameValuesLT *NameValuesL, SdifUInt4 NumCurrHT);
extern SdifNameValueT*   SdifNameValuesLGet          (SdifNameValuesLT *NameValuesL, char *Name);
extern SdifNameValueT*   SdifNameValuesLGetFromCurrHT(SdifNameValuesLT *NameValuesL, char *Name);
extern SdifNameValueT*   SdifNameValuesLPut          (SdifNameValuesLT *NameValuesL, char *Name,  char *Value);
extern SdifUInt2         SdifNameValuesLIsNotEmpty   (SdifNameValuesLT *NameValuesL);

#endif /* _SdifNameValue_ */
