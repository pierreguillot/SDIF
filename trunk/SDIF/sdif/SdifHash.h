/* SdifHash.h
 *
 *
 *
 */

#ifndef _SdifHash_
#define _SdifHash_



typedef enum SdifHashIndexTypeE
{
  eHashChar,
  eHashInt4
} SdifHashIndexTypeEnum;


typedef union SdifHashIndexU
{
  char* Char[1]; /* tab of one pointer to fixe union size at 4 or 8 bytes */
  unsigned int  Int4;
} SdifHashIndexUnion;


typedef struct SdifHashNodeT SdifHashNodeType;

struct SdifHashNodeT 
{
  SdifHashNodeType *Next;
  SdifHashIndexUnion Index;
  void* Data;
};


typedef struct SdifHashTableT
{
  SdifHashNodeType* *Table;
  unsigned int HashSize;
  SdifHashIndexTypeEnum IndexType;
  void (*Killer)();  /* no verification of arguments */
  unsigned int NbOfData;
} SdifHashTableType;


extern SdifHashTableType*
SdifCreateHashTable(unsigned int HashSize, SdifHashIndexTypeEnum IndexType, void (*Killer)());

extern void
SdifMakeEmptyHashTable(SdifHashTableType* HTable);

extern void
SdifKillHashTable(SdifHashTableType* HTable);




/******************  eHashChar ****************/

extern unsigned int
SdifHashChar(char* s, unsigned int nchar, unsigned int HashSize);

extern void*
SdifHashTableSearchChar(SdifHashTableType* HTable,
			char *s,
			unsigned int nchar);

extern SdifHashTableType*
SdifHashTablePutChar(SdifHashTableType* HTable,
		     char *s,
		     unsigned int nchar,
		     void* Data);


/***************** eHashInt4 **********************/

extern unsigned int
SdifHashInt4(unsigned int i, unsigned int HashSize);

extern void*
SdifHashTableSearchInt4(SdifHashTableType* HTable, unsigned int i);

extern SdifHashTableType*
SdifHashTablePutInt4(SdifHashTableType* HTable, unsigned int i, void* Data);


/*************************** for all ***********************/

extern void*
SdifHashTableSearch(SdifHashTableType* HTable, void *ptr, unsigned int nobj);

extern SdifHashTableType*
SdifHashTablePut(SdifHashTableType* HTable, void *ptr, unsigned int nobj, void* Data);

#endif /* _SdifHash_ */
