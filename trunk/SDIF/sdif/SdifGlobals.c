/* $Id: SdifGlobals.c,v 3.3 1999-09-28 13:09:00 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifGlobals.c
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.2  1999/06/18  16:23:55  schwarz
 * SdifSignatureCmpNoVersion dropped LAST byte on alpha, because the mask
 * 0x00ffffff was not byteswapped.  Introduced gSdifSignatureVersionMask,
 * which is initialised in SdifInitMachineType to appropriate value.
 *
 * Revision 3.1  1999/03/14  10:56:57  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:49  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:34  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:22  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:47  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/04/24  12:40:33  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 *
 */

#include <preincluded.h>
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

  S1 = Signature1 & gSdifSignatureVersionMask;
  S2 = Signature2 & gSdifSignatureVersionMask;

  return (short)(S1 == S2);
}


SdifUInt4
SdifSizeofDataType(SdifDataTypeET DataType)
{
    switch (DataType)
	{
    case eUnicode :
    case eChar :
        return 1;
    case eInt2 :
    case eUInt2 :
        return 2;
    case eFloat8 :
        return 8;
    case eFloat4 :
    case eInt4 :
    case eUInt4 :
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
