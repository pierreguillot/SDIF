/* SdifHard_OS.c
 *
 *
 * author: Dominique Virolle 1998
 *
 */


#include "SdifHard_OS.h"

#include <string.h>
#include <ctype.h>


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






/* SdifLittleToBig and SdifBigToLittle are
 * exactly the same function.
 * It's only to know what it does.
 */

void
SdifLittleToBig(void *ptr, size_t size)
{
    unsigned int i;
    size_t sizediv2;
    unsigned char * s = ptr;
    unsigned char ctemp;

    sizediv2 = size / 2;

    for (i=0 ; i< sizediv2 ; i++)
      {
        ctemp = s[i];
        s[i] = s[size -1 -i];
        s[size -1 -i] = ctemp;
      }
}

void
SdifBigToLittle(void *ptr, size_t size)
{
    unsigned int i;
    size_t sizediv2;
    unsigned char * s = ptr;
    unsigned char ctemp;

    sizediv2 = size / 2;

    for (i=0 ; i< sizediv2 ; i++)
      {
        ctemp = s[i];
        s[i] = s[size -1 -i];
        s[size -1 -i] = ctemp;
      }
}
