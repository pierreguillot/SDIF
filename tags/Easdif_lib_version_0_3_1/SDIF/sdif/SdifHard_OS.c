/* $Id: SdifHard_OS.c,v 3.10 2002-05-24 19:37:52 ftissera Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
 *
 *
 *
 * author: Dominique Virolle 1998
 * $Log: not supported by cvs2svn $
 * Revision 3.9  2001/05/02 09:34:44  tisseran
 * Change License from GNU Public License to GNU Lesser Public License.
 *
 * Revision 3.8  2000/11/21 16:34:49  roebel
 * New SdifSignatureConst builds integer signature according to
 * endianess of machine. Multicharacter constants are no longer
 * supported by the library. Cleaned up sdif.h/SdifGlobals.h a bit.
 * Test for Multicharacter conversion is removed from configure.in.
 *
 * Revision 3.7  2000/11/15 14:53:30  lefevre
 * no message
 *
 * Revision 3.6  2000/10/27  20:03:34  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.5.2.2  2000/08/21  21:35:25  tisseran
 * *** empty log message ***
 *
 * Revision 3.5.2.1  2000/08/21  14:04:19  tisseran
 * *** empty log message ***
 *
 * Revision 3.5  2000/07/06  19:01:48  tisseran
 * Add function for frame and matrix type declaration
 * Remove string size limitation for NameValueTable
 * TODO: 1TYP and 1IDS frame must contain an 1NVT (text) matrix
 *       Actually, data are written with fprintf.
 *
 * Revision 3.4  1999/10/07  15:12:24  schwarz
 * Added isSeekable flag in SdifFileT struct.  This allows to simplify the
 * many tests for stdio on opening the stream.
 * Added SdifStrEq utility function.
 *
 * Revision 3.3  1999/09/28  13:09:01  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.2  1999/06/18  16:23:57  schwarz
 * SdifSignatureCmpNoVersion dropped LAST byte on alpha, because the mask
 * 0x00ffffff was not byteswapped.  Introduced gSdifSignatureVersionMask,
 * which is initialised in SdifInitMachineType to appropriate value.
 *
 * Revision 3.1  1999/03/14  10:56:59  virolle
 * SdifStdErr add
 *
 * Revision 2.4  1999/02/28  12:16:47  virolle
 * memory report
 *
 * Revision 2.3  1999/01/23  15:55:51  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:36  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 */


#include <preincluded.h>

#include "SdifHard_OS.h"
#include "SdifError.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif


#include "SdifMemory.h"

/* To know what hardware we have (check endian and sizeof long ) 
   Endian and size of long are checked in ./configure => config.h */
SdifMachineET
SdifGetMachineType(void)
{
  unsigned int MachineType;
/*    long *LongTest; */
/*    char CharTab[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}; */

  /* for 2nd part of tests if Little */
/*    const char ConstCharTab[] = {'A', 'B', 'C', 'D'}; */
/*    const unsigned int ConstDCBA = 'DCBA'; */
/*    unsigned int *ConstUIntTest; */

/*    LongTest = (long*) CharTab; */

/*#ifdef _LONG64BITS_
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
  default :
  MachineType = eUndefinedMachine;
  break;
  }
  #endif */ /* _LONG64BITS */
  
/* WORDS_BIG_ENDIAN and SIZEOF_LONG are defined in config.h */
#ifdef WORDS_BIGENDIAN
  if (SIZEOF_LONG == 4)
      MachineType = eBigEndian;
  else
      MachineType = eBigEndian64;
#else /* LITTLEENDIAN */
  if (SIZEOF_LONG == 4)
      MachineType = eLittleEndian;
  else
      MachineType = eLittleEndian64;
#endif /* WORDS_BIGENDIAN */
  
/*    if (    (MachineType == eLittleEndian) */
/*         || (MachineType == eLittleEndian64)  ) */
/*      { */
/*        there is supposed that sizeof(int)== 4 */
/*        ConstUIntTest = (unsigned int*) ConstCharTab; */
/*        if ((*ConstUIntTest) == ConstDCBA) */
/*          { */
/*            if (MachineType == eLittleEndian) */
/*              MachineType = eLittleEndianLittleConst; */
/*            else */
/*            if (MachineType == eLittleEndian64) */
/*              MachineType = eLittleEndianLittleConst64; */
/*          } */
/*      } */
  return (SdifMachineET) MachineType;
}




SdifMachineET gSdifMachineType;
SdifUInt4     gSdifSignatureVersionMask;



SdifMachineET
SdifInitMachineType(void)
{
  gSdifMachineType = SdifGetMachineType();

  switch (gSdifMachineType)
  {     
    case eLittleEndian :
    case eLittleEndian64 :
      gSdifSignatureVersionMask = 0xffffff00;
    break;

    default :
      gSdifSignatureVersionMask = 0x00ffffff;
    break;
  }

  return gSdifMachineType;
}




/* SdifLittleToBig keeps the little input for other use.
 */
void
SdifLittleToBig(void *BigPtr, void *LittlePtr, size_t size)
{
    unsigned int i;
    size_t sizediv2;
    unsigned char * BigS = (unsigned char * ) BigPtr;
    unsigned char * LittleS = (unsigned char * ) LittlePtr;
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
    unsigned char * s = (unsigned char * ) InOutPtr;
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
SdifStrEq(const char *s1, const char *s2)
{
  return strcmp(s1, s2) == 0;
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

  NewString = SdifCalloc(char, Size);
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
    SdifFree(String);
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

