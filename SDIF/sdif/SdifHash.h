/* $Id: SdifHash.h,v 1.4 1998-07-23 17:02:55 virolle Exp $
 *
 * SdifHash.h
 *
 * SdifHash is coded like a small independant library.
 * Every HashTable have its own size and data type.
 * Every data type must have a function to free memory : 
 *     void <KillerName>(<type>* ptr).
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.3  1998/05/04  15:44:18  schwarz
 * Replaced constant char* args by const char* (also some void*).
 *
 */

#ifndef _SdifHash_
#define _SdifHash_



typedef enum SdifHashIndexTypeE
{
  eHashChar,
  eHashInt4
} SdifHashIndexTypeET;





typedef union SdifHashIndexU SdifHashIndexUT;

union SdifHashIndexU
{
  char* Char[1]; /* tab of one pointer to fixe union size at 4 or 8 bytes */
  unsigned int  Int4;
} ;





typedef struct SdifHashNS SdifHashNT;

struct SdifHashNS 
{
  SdifHashNT *Next;
  SdifHashIndexUT Index;
  void* Data;
};




typedef struct SdifHashTableS SdifHashTableT;

struct SdifHashTableS
{
  SdifHashNT* *Table;
  unsigned int HashSize;
  SdifHashIndexTypeET IndexType;
  void (*Killer)();  /* no verification of arguments */
  unsigned int NbOfData;
} ;


extern SdifHashTableT* SdifCreateHashTable(unsigned int HashSize, SdifHashIndexTypeET IndexType, void (*Killer)());

extern void SdifMakeEmptyHashTable (SdifHashTableT* HTable);
extern void SdifKillHashTable      (SdifHashTableT* HTable);



/******************  eHashChar ****************/

extern unsigned int SdifHashChar(const char* s, unsigned int nchar, unsigned int HashSize);

extern void*           SdifHashTableSearchChar(SdifHashTableT* HTable, const char *s, unsigned int nchar);
extern SdifHashTableT* SdifHashTablePutChar   (SdifHashTableT* HTable, const char *s, unsigned int nchar, void* Data);


/***************** eHashInt4 **********************/

extern unsigned int SdifHashInt4(unsigned int i, unsigned int HashSize);

extern void*           SdifHashTableSearchInt4(SdifHashTableT* HTable, unsigned int i);
extern SdifHashTableT* SdifHashTablePutInt4   (SdifHashTableT* HTable, const unsigned int i, void* Data);


/*************************** for all ***********************/

extern void*           SdifHashTableSearch (SdifHashTableT* HTable, void *ptr, unsigned int nobj);
extern SdifHashTableT* SdifHashTablePut    (SdifHashTableT* HTable, const void *ptr, unsigned int nobj, void* Data);

#endif /* _SdifHash_ */
