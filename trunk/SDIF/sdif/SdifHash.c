/* $Id: SdifHash.c,v 3.1 1999-03-14 10:57:01 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifHash.c
 *
 * SdifHash is coded like a small independant library.
 * Every HashTable have its own size and data type.
 * Every data type must have a function to free memory : 
 *     void <KillerName>(<type>* ptr).
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.2  1999/02/28  12:16:48  virolle
 * memory report
 *
 * Revision 2.1  1998/12/21  18:27:26  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:51  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/05/04  15:44:17  schwarz
 * Replaced constant char* args by const char* (also some void*).
 *
 */

#include "SdifHash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SdifMemory.h"
#include "SdifError.h"

SdifHashTableT*
SdifCreateHashTable(unsigned int HashSize,
		    SdifHashIndexTypeET IndexType,
		    void (*Killer)())
{
/*
 * The hash table size is better if it is a prime number like : 31, 61, 127, 251...
 * Killer is the name of the hash table data type destructor.
 *  ( void (*Killer)() ) No verification of arguments.
 * This can occasion a warning at the compilation :
 *     Warning <...>: SdifHash.c, line <...>:
 *     No prototype for the call to an anonymous function
 *                    (*HTable->Killer)(pNode->Data);
 *                    -----------------^
 *     To achieve better type-checking, there should be a full prototype for
 *     the function being called.
 */
  SdifHashTableT *NewTable = NULL;
  SdifHashNT **CurrTable;
  unsigned int i;

  NewTable = SdifMalloc(SdifHashTableT);
  if (NewTable)
    {
      NewTable->HashSize = HashSize;
      NewTable->NbOfData = 0;
      NewTable->IndexType = IndexType;
      NewTable->Killer = Killer;
      NewTable->Table = SdifCalloc(SdifHashNT*, HashSize);
      if (NewTable->Table)
	    {
	      CurrTable = NewTable->Table;
	      for(i=0; i<HashSize; i++)
	       *CurrTable++ = NULL;
	    }
      else
	    {
	      _SdifError(eAllocFail, "HashTable allocation");
	     return NULL;
        }
      return NewTable;
    }
  else
    {
      _SdifError(eAllocFail, "HashTable allocation");
      return NULL;
    }
}






void
SdifMakeEmptyHashTable(SdifHashTableT* HTable)
{
  unsigned int i;
  SdifHashNT *pNode;

  for(i=0; i<HTable->HashSize; i++)
    {
      while (HTable->Table[i])
        {
          pNode = HTable->Table[i];
          if (HTable->IndexType == eHashChar)
            SdifFree(pNode->Index.Char[0]);
          if (HTable->Killer)
            (*(HTable->Killer))(pNode->Data); /* this line can occasion a warning */
          else
            {
              /* if (pNode->Data)
              *   SdifFree(pNode->Data);
	          *else
	          *   fprintf(stderr, "HashTable->Data kill memory error : \n");
	          */
	          /* consider that the object pNode->Data cannot be kill because static
	           */
	        }
	      HTable->Table[i] = pNode->Next;
	      SdifFree(pNode);
	    }
    }
}






void
SdifKillHashTable(SdifHashTableT* HTable)
{
  if (HTable)
    {
      SdifMakeEmptyHashTable(HTable);
      SdifFree(HTable->Table);
      SdifFree(HTable);
    }
  else
    {
      _SdifError(eFreeNull, "HashTable free");
    }
}







/******************  eHashChar ****************/
/* nchar represente le nombre de caracteres significatifs pour Hasher */
unsigned int
SdifHashChar(const char* s, unsigned int nchar, unsigned int HashSize)
{
  unsigned int valHash;
  unsigned int i;

  valHash = 0;
  for(i=0; i<nchar; i++)
    valHash = (valHash*256 + ((unsigned) *s++)) % HashSize;
  return valHash;
}

  





void*
SdifHashTableSearchChar(SdifHashTableT* HTable, const char *s, unsigned int nchar)
{
  SdifHashNT *pNode;
  
  
  for (pNode = HTable->Table[SdifHashChar(s, nchar, HTable->HashSize)]; pNode!=NULL; pNode = pNode->Next)
    {
      if (strncmp((char*) s, (char*) pNode->Index.Char[0], nchar)== 0)
	    {
         return pNode->Data;     /* trouve */
        }
    }

  return NULL; /* pas trouve */
}






SdifHashTableT*
SdifHashTablePutChar(SdifHashTableT* HTable,
		     const char *s,
		     unsigned int nchar,
		     void* Data)
{
  SdifHashNT *pNode;
  unsigned int valHash;

  if ( ! SdifHashTableSearchChar(HTable, s, nchar))
    {
      pNode = SdifMalloc(SdifHashNT);
      if (pNode)
        {
	      pNode->Index.Char[0] = SdifCalloc(char, nchar);
          if (pNode->Index.Char[0])
	        strncpy((char*) pNode->Index.Char[0], (char*) s, nchar);
	      else
	        return NULL; /* erreur memoire */
	      valHash = SdifHashChar(s, nchar, HTable->HashSize);
	      pNode->Data = Data; /* Attention : copie de pointeur */
	      HTable->NbOfData++;
	      pNode->Next = HTable->Table[valHash];
	      HTable->Table[valHash] = pNode;
	    }
    }
  return HTable;
}








/***************** eHashInt4 **********************/

unsigned int
SdifHashInt4(unsigned int i, unsigned int HashSize)
{
  return i % HashSize;
}





void*
SdifHashTableSearchInt4(SdifHashTableT* HTable, unsigned int i)
{
  SdifHashNT *pNode;
  
  for (pNode = HTable->Table[SdifHashInt4(i, HTable->HashSize)]; pNode!=NULL; pNode = pNode->Next)
    if (pNode->Index.Int4 == i)
      return pNode->Data;     /* trouve */
  return NULL; /* pas trouve */
}





SdifHashTableT*
SdifHashTablePutInt4(SdifHashTableT* HTable, const unsigned int i, void* Data)
{
  SdifHashNT *pNode;
  SdifHashNT *CurrNode;
  unsigned int valHash;
  
  if ( ! SdifHashTableSearchInt4(HTable, i))
    {
      pNode = SdifMalloc(SdifHashNT);
      if (pNode)
	    {
	      pNode->Index.Int4 = i;
	      valHash = SdifHashInt4(i, HTable->HashSize);
	      pNode->Data = Data; /* Attention : copy de pointeur */
	      HTable->NbOfData++;
	  
          if (! HTable->Table[valHash])
            {
	          pNode->Next = NULL;
	          HTable->Table[valHash] = pNode;
	        }
	      else
	        {
	          if (pNode->Index.Int4 < HTable->Table[valHash]->Index.Int4)
		        {
		          pNode->Next = HTable->Table[valHash];
		          HTable->Table[valHash] = pNode;
		        }
	          else
		        {
		          for(CurrNode = HTable->Table[valHash]; CurrNode->Next; CurrNode = CurrNode->Next)
		            if (pNode->Index.Int4 < CurrNode->Next->Index.Int4)
		              break;
		  
		          pNode->Next = CurrNode->Next;
		          CurrNode->Next = pNode;
		        }
	        }
	    }
    }
  return HTable;
}






/*************** for all ******************/



void*
SdifHashTableSearch(SdifHashTableT* HTable, void *ptr, unsigned int nobj)
{
  switch (HTable->IndexType)
    {
    case eHashChar :
      return SdifHashTableSearchChar(HTable, ptr, nobj);
    case eHashInt4 :
      return SdifHashTableSearchInt4(HTable, *((unsigned int*) ptr));
    default :
      return NULL;
    }
}






SdifHashTableT*
SdifHashTablePut(SdifHashTableT* HTable, const void *ptr, unsigned int nobj, void* Data)
{
  switch (HTable->IndexType)
    {
    case eHashChar :
      return SdifHashTablePutChar(HTable, ptr, nobj, Data);
    case eHashInt4 :
      return SdifHashTablePutInt4(HTable, *((const unsigned int*)ptr), Data);
    default :
      return NULL;
    }
}
