/* $Id: SdifGlobals.c,v 1.4 1998-07-23 17:02:52 virolle Exp $
 *
 * SdifGlobals.c
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.3  1998/04/24  12:40:33  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 *
 */

#include "SdifGlobals.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char gSdifString[_SdifStringLen];
char gSdifString2[_SdifStringLen];
char gSdifErrorMess[_SdifStringLen];

char gSdifStringSignature[_SdifNbMaxPrintSignature][5];
/* [_SdifNbMaxPrintSignature] : it's possible to print  _SdifNbMaxPrintSignature
 * signatures on stream in an unique expression but not more.
 */

/*static*/ int  CurrStringPosSignature = 0;



#include "SdifRWLowLevel.h"
char *
SdifSignatureToString(SdifSignature Signature)
{
  char *pS, *Ret;
  SdifSignature SignW;

  SignW = Signature; 
  switch (gSdifMachineType)
    {     
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      SdifLittleToBig(&SignW, sizeof(Signature));
      break;
    default :
      break;
    }

  pS = (char*) &SignW;
  sprintf(gSdifStringSignature[CurrStringPosSignature],
			"%c%c%c%c", pS[0], pS[1], pS[2], pS[3]);
  /*gSdifStringSignature[CurrStringPosSignature][0] = pS[0];
  gSdifStringSignature[CurrStringPosSignature][1] = pS[1];
  gSdifStringSignature[CurrStringPosSignature][2] = pS[2];
  gSdifStringSignature[CurrStringPosSignature][3] = pS[3];
  gSdifStringSignature[CurrStringPosSignature][4] = '\0';*/

  Ret = gSdifStringSignature[CurrStringPosSignature];
  /*fprintf(stderr,"%s|", Ret);*/
  CurrStringPosSignature = (CurrStringPosSignature+1) % _SdifNbMaxPrintSignature;
  return Ret;
}


short
SdifSignatureCmpNoVersion(SdifSignature Signature1, SdifSignature Signature2)
{
  SdifUInt4 S1, S2;

  S1 = Signature1 & 0x00ffffff;
  S2 = Signature2 & 0x00ffffff;

  return (short)(S1 == S2);
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
SdifSizeofDataType(SdifDataTypeET DataType)
{
  return (DataType & 0xff)>>3;  /* 8 last bits of DataType DIV 8 */
}




size_t
SdifPaddingCalculate(size_t NbBytes)
{
  size_t mod;

  mod = NbBytes % _SdifPadding;
  return mod ? (_SdifPadding - mod) : 0;
}




size_t
SdifFPaddingCalculate(FILE *f, size_t NbBytes)
{
  size_t mod;

  if ((f != stdin) && (f != stdout) && (f != stderr))
    {
      mod = (size_t) ftell(f) % _SdifPadding;
      return mod ? (_SdifPadding - mod) : 0;
    }
  else
    return SdifPaddingCalculate(NbBytes);
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
