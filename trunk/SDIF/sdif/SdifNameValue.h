/* $Id: SdifNameValue.h,v 1.4 1998-07-23 17:02:57 virolle Exp $
 *
 * SdifNameValue.h
 *
 * Name values management. For 1 SdifFileT*, we have one SdifNameValueLT*
 * which contains a list of hash tables. Each hach table contains name-values.
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.3  1998/04/24  12:40:37  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 *
 */

#ifndef _SdifNameValue_
#define _SdifNameValue_


#include "SdifGlobals.h"
#include "SdifHash.h"

#define _SdifNameValueHashSize 31


typedef struct SdifNameValueS SdifNameValueT;
struct SdifNameValueS
{
  char *Name;
  char *Value;
} ;


extern SdifNameValueT* SdifCreateNameValue(const char *Name, const char *Value);
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



typedef struct SdifNameValuesLS SdifNameValuesLT;
struct SdifNameValuesLS
{
  SdifNameValueHTNT *HeadHTN;
  SdifNameValueHTNT *TailHTN;
  SdifUInt4          NbHTN;
  SdifHashTableT    *CurrHT;
  SdifUInt4          HashSize;
} ;

extern SdifNameValuesLT* SdifCreateNameValuesL       (SdifUInt4  HashSize);
extern void              SdifKillNameValuesL         (SdifNameValuesLT *NameValuesL);
extern SdifNameValuesLT* SdifNameValuesLNewHT        (SdifNameValuesLT *NameValuesL);
extern SdifHashTableT*   SdifNameValuesLSetCurrHT    (SdifNameValuesLT *NameValuesL, SdifUInt4 NumCurrHT);
extern SdifNameValueT*   SdifNameValuesLGet          (SdifNameValuesLT *NameValuesL, char *Name);
extern SdifNameValueT*   SdifNameValuesLGetFromCurrHT(SdifNameValuesLT *NameValuesL, char *Name);
extern SdifNameValueT*   SdifNameValuesLPut          (SdifNameValuesLT *NameValuesL, const char *Name,  const char *Value);
extern SdifUInt2         SdifNameValuesLIsNotEmpty   (SdifNameValuesLT *NameValuesL);

#endif /* _SdifNameValue_ */
