/* SdifHard_OS.c
 *
 *
 * author: Dominique Virolle 1998
 *
 */


#include "SdifHard_OS.h"
#include "SdifError.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif



SdifMachineET
SdifGetMachineType(void)
{
  unsigned int MachineType;
  long *LongTest;
  char CharTab[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

  /* for 2nd part of tests if Little */
  const char ConstCharTab[] = {'A', 'B', 'C', 'D'};
  const unsigned int ConstDCBA = 'DCBA';
  unsigned int *ConstUIntTest;

  LongTest = (long*) CharTab;

#ifdef _LONG64BITS_
  switch (*LongTest)
    {
    case 0x0102030405060708 :
      MachineType = eBigEndian64;
	  break;
    case 0x0807060504030201 :
      MachineType = eLittleEndian64;
	  break;
    default :
      MachineType = eUndefinedMachine;
	  break;
    }

#else
  switch (*LongTest)
    {
    case 0x01020304 :
      MachineType = eBigEndian;
	  break;
    case 0x04030201 :
      MachineType = eLittleEndian;
	  break;
   /*case 0x03040102 :
    *MachineType = ePDPEndian;
	*break;
    */
    default :
      MachineType = eUndefinedMachine;
      break;
    }
#endif /* _LONG64BITS */

  
  if (    (MachineType == eLittleEndian)
       || (MachineType == eLittleEndian64)  )
    {
      /* there is supposed that sizeof(int)== 4 */
      ConstUIntTest = (unsigned int*) ConstCharTab;
      if ((*ConstUIntTest) == ConstDCBA)
        {
          if (MachineType == eLittleEndian)
            MachineType = eLittleEndianLittleConst;
          else
          if (MachineType == eLittleEndian64)
            MachineType = eLittleEndianLittleConst64;
        }
    }
  return MachineType;
}




SdifMachineET gSdifMachineType;




SdifMachineET
SdifInitMachineType(void)
{
  gSdifMachineType = SdifGetMachineType();
  return gSdifMachineType;
}




/* SdifLittleToBig keeps the little input for other use.
 */
void
SdifLittleToBig(void *BigPtr, void *LittlePtr, size_t size)
{
    unsigned int i;
    size_t sizediv2;
    unsigned char * BigS = BigPtr;
    unsigned char * LittleS = LittlePtr;
    unsigned char ctemp; /* if BigPtr == LittlePtr */

    sizediv2 = size / 2;

    for (i=0 ; i< sizediv2 ; i++)
      {
        ctemp            = LittleS[i];
        BigS[i]          = LittleS[size -1 -i];
        BigS[size -1 -i] = ctemp;
      }
}

/* SdifBigToLittle replace the big by the little.
 */
void
SdifBigToLittle(void *InOutPtr, size_t size)
{
    unsigned int i;
    size_t sizediv2;
    unsigned char * s = InOutPtr;
    unsigned char ctemp;

    sizediv2 = size / 2;

    for (i=0 ; i< sizediv2 ; i++)
      {
        ctemp = s[i];
        s[i] = s[size -1 -i];
        s[size -1 -i] = ctemp;
      }
}




int
SdifStrLen(const char *s)
{
  return (int) strlen(s);
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




char*
SdifCreateStrNCpy(const char* Source, size_t Size)
{
  char *NewString;

  NewString = (char*) malloc (sizeof(char) * Size);
  if (NewString)
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





/* Only for WIN32 */
void SdifSetStdIOBinary (void)
{
#ifdef WIN32
    _setmode( _fileno( stdin  ), _O_BINARY );
    _setmode( _fileno( stdout ), _O_BINARY );
    _setmode( _fileno( stderr ), _O_BINARY );
#endif
}







FILE* SdiffBinOpen(const char * Name, SdifBinaryModeET Mode)
{
    switch (Mode)
    {
    case eBinaryModeRead :
        return fopen (Name, "rb");
    case eBinaryModeWrite :
        return fopen (Name, "wb");
    case eBinaryModeStdInput:
        return stdin;
    case eBinaryModeStdOutput:
        return stdout;
    case eBinaryModeStdError:
        return stderr;
    default :
        return NULL;
    }
}

    
SdifInt4 SdiffBinClose(FILE *f)
{
    return (SdifInt4) fclose (f);
}

