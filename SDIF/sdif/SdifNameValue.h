/* $Id: SdifNameValue.h,v 2.0 1998-11-29 11:41:58 virolle Exp $
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
 * Revision 1.5  1998/11/10  15:31:52  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 * Revision 1.4  1998/07/23  17:02:57  virolle
 * *** empty log message ***
 *
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


SdifNameValueT* SdifCreateNameValue(const char *Name, const char *Value);
void            SdifKillNameValue  (SdifNameValueT *NameValue);



typedef struct SdifNameValueHTNS SdifNameValueHTNT;

struct SdifNameValueHTNS
{
  SdifNameValueHTNT *Next;
  SdifHashTableT *NameValueHT;
  SdifUInt4 NumHT;
};


SdifNameValueHTNT* SdifCreateNameValueHTN(SdifNameValueHTNT *Next,
						 SdifHashTableT *NameValueHT,
						 SdifUInt4 NumHT);
SdifNameValueHTNT* SdifKillNameValueHTN  (SdifNameValueHTNT *NVHTN);



typedef struct SdifNameValuesLS SdifNameValuesLT;
struct SdifNameValuesLS
{
  SdifNameValueHTNT *HeadHTN;
  SdifNameValueHTNT *TailHTN;
  SdifUInt4          NbHTN;
  SdifHashTableT    *CurrHT;
  SdifUInt4          HashSize;
} ;

SdifNameValuesLT* SdifCreateNameValuesL     (SdifUInt4  HashSize);
void              SdifKillNameValuesL       (SdifNameValuesLT *NameValuesL);
SdifNameValuesLT* SdifNameValuesLNewHT      (SdifNameValuesLT *NameValuesL);
SdifHashTableT*   SdifNameValuesLSetCurrHT  (SdifNameValuesLT *NameValuesL, SdifUInt4 NumCurrHT);
SdifNameValueT*   SdifNameValuesLGet        (SdifNameValuesLT *NameValuesL, char *Name);
SdifNameValueT*   SdifNameValuesLGetCurrHT  (SdifNameValuesLT *NameValuesL, char *Name);
SdifNameValueT*   SdifNameValuesLPutCurrHT  (SdifNameValuesLT *NameValuesL, const char *Name,  const char *Value);
SdifUInt2         SdifNameValuesLIsNotEmpty (SdifNameValuesLT *NameValuesL);

#endif /* _SdifNameValue_ */
