/* $Id: SdifGlobals.c,v 2.0 1998-11-29 11:41:47 virolle Exp $
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

  switch (gSdifMachineType)
    {     
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      SdifLittleToBig(&SignW, &Signature, sizeof(SdifSignature));
      break;
    default :
	  SignW = Signature; 
      break;
    }

  pS = (char*) &SignW;
  /* string affectation with 'sprintf' is important for release in Win/Dos32
   */
  sprintf(gSdifStringSignature[CurrStringPosSignature],
			"%c%c%c%c", pS[0], pS[1], pS[2], pS[3]);

  Ret = gSdifStringSignature[CurrStringPosSignature];
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


SdifUInt4
SdifSizeofDataType(SdifDataTypeET DataType)
{
    switch (DataType)
	{
    case eInt2 :
    case eUInt2 :
        return 2;
    case eFloat8 :
        return 8;
    case eFloat4 :
    case eInt4 :
    case eUInt4 :
    case eChar4 :
	default :
        return 4;
	}
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
