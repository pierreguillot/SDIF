/* SdifGlobals.c
 *
 *
 *
 */

#include "SdifGlobals.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/* !attention! recursive include */
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifNameValue.h"

SdifHashTableType *gSdifMatrixTypesTable;
SdifHashTableType *gSdifFrameTypesTable;
SdifHashTableType *gSdifStreamIDsTable;
SdifNameValuesLType *gSdifNameValues;

char gSdifString[_SdifStringLen];
char gSdifString2[_SdifStringLen];
char gSdifErrorMess[_SdifStringLen];


void
SdifInitAllHashTables(unsigned int HashSize)
{
  gSdifMatrixTypesTable = SdifCreateHashTable(HashSize, eHashChar, SdifKillMatrixType);
  gSdifFrameTypesTable  = SdifCreateHashTable(HashSize, eHashChar, SdifKillFrameType);
  gSdifStreamIDsTable   = SdifCreateHashTable(1, eHashInt4, SdifKillStreamID);
}








void
SdifKillAllHashTables(void)
{
  SdifKillHashTable(gSdifMatrixTypesTable);
  SdifKillHashTable(gSdifFrameTypesTable);
  SdifKillHashTable(gSdifStreamIDsTable);
}









int
SdifPrintName(FILE* fw, char *Name)
{
  unsigned i;
  int NbCharPrint = 0;

  for(i=0; i<_SdifNameLen; i++)
    if (fputc((int) Name[i], fw))
      NbCharPrint++;
  return NbCharPrint;
}




int
SdifStrLen(const char *s)
{
  return strlen(s);
}


int
SdifStrCmp(const char *s1, const char *s2)
{
  return strcmp(s1, s2);
}


int
SdifStrNCmp(const char *s1, const char *s2, unsigned int n)
{
  return strncmp(s1, s2, n);
}



char*
SdifStrNCpy(char *s1, const char *s2, unsigned int n)
{
  return strncpy (s1, s2, n);
}






int
SdifNameCmpWithoutVersion(const char NToTest[],
			  const char WithN[])
{
  int
    iC,
    Dif = 0;

  if (isdigit(NToTest[0]) && isdigit(WithN[0]))
    {
      for(iC=1; iC<_SdifNameLen; iC++)
	if (NToTest[iC]!=WithN[iC])
	  Dif++;
      return Dif;
    }
  else
    return SdifStrNCmp(NToTest, WithN, _SdifNameLen);
}








char*
SdifCreateStrNCpy(char* Source, unsigned int Size)
{
  char *NewString;

  if (NewString = (char*) malloc (sizeof(char) * Size))
    {
      SdifStrNCpy(NewString, Source, Size);
      
      return NewString;
    }
  else
    {
      _SdifError(eAllocFail, Source);
      return NULL;
    }
}






void
SdifKillStr(char* String)
{
  if (String)
    free(String);
  else
    _SdifError(eFreeNull, "String free");
}





SdifUInt4
SdifSizeofDataType(SdifDataTypeEnum DataType)
{
  return (DataType & 0xff)>>3;  /* 8 last bits of DataType DIV 8 */
}




unsigned int
SdifPaddingCalculate(int NbBytes)
{
  unsigned int mod;

  mod = NbBytes % _SdifPadding;
  return mod ? (_SdifPadding - mod) : 0;
}




unsigned int
SdifFPaddingCalculate(FILE *f, int NbBytes)
{
  unsigned int mod;

  if ((f != stdin) && (f != stdout) && (f != stderr))
    {
      mod = ftell(f) % _SdifPadding;
      return mod ? (_SdifPadding - mod) : 0;
    }
  else
    return SdifPaddingCalculate(NbBytes);
}






/* !attention! recursive include */
#include "SdifRWHighLevel.h"
#include "SdifRWLowLevel.h"

void
SdifGenInit(unsigned int HashSize, char* TypesFileName)
{
  SdifInitMachineType();
  SdifInitAllHashTables(HashSize);
  SdifLoadPredefinedTypes(TypesFileName);
  gSdifNameValues = SdifCreateNameValuesL(_SdifNameValueHashSize);
}



void
SdifGenKill(void)
{
  SdifKillAllHashTables();
  SdifKillNameValuesL(gSdifNameValues);
}






short
SdifFloat8Equ(SdifFloat8 f1, SdifFloat8 f2)
{
  if (f1 >= f2)
    if ((f1 - f2)  < _SdifFloatEps)
      return 1;
    else
      return 0;
  else
    return SdifFloat8Equ(f2, f1);
}
