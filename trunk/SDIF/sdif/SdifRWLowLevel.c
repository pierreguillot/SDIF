/* SdifRWLowLevel.c
 *
 * Read Write Low Level. Machine sex, little-big endian control
 *
 * author: Dominique Virolle 1997
 *
 */


#include "SdifRWLowLevel.h"
#include "SdifHard_OS.h"
#include "SdifError.h"


#include <string.h>
#include <ctype.h>



static char gSdifLittleToBig [_SdifBSLittleE];



/* fread encapsule */
size_t
Sdiffread(void *ptr, size_t size, size_t nobj, FILE *stream)
{
  size_t nobjread;

  nobjread = fread(ptr, size, nobj, stream);
  if (nobjread != nobj)
    {
      sprintf(gSdifErrorMess, "Sdiffread %d", ftell(stream));
      _SdifError(eEof, gSdifErrorMess);
    }
  
  return nobjread;
}






/* fwrite encapsule */
size_t
Sdiffwrite(void *ptr, size_t size, size_t nobj, FILE *stream)
{
  size_t nobjwrite;

  nobjwrite = fwrite(ptr, size, nobj, stream);
  if (nobjwrite != nobj)
    {
      sprintf(gSdifErrorMess, "Sdiffwrite %d", ftell(stream));
      _SdifError(eEof, gSdifErrorMess);
    }

  return nobjwrite;
}








/****************************************************/
/************ little endian machine *****************/


size_t
SdiffreadLittleEndian2 (void *ptr, size_t nobj, FILE *stream)
{
  size_t nobjread = 0;
  size_t
    i2bytes,
    Mi2Bytes;
  unsigned char *ptrChar;

  ptrChar = (unsigned char*) ptr;

  if (nobj > _SdifBSLittleE / 2)
    {
      nobjread += SdiffreadLittleEndian2(ptr, _SdifBSLittleE / 2, stream);
      nobjread += SdiffreadLittleEndian2(((unsigned char*) ptr)+(_SdifBSLittleE),
					   nobj - (_SdifBSLittleE / 2),
					   stream);
      return nobjread;
    }
  else
    {
      nobjread = Sdiffread(gSdifLittleToBig, 2, nobj, stream);
      Mi2Bytes = nobj << 1;         /****  (nobj << 1)  <=>  (nobj*2)  *****/
      for(i2bytes=0; i2bytes<Mi2Bytes; i2bytes+=2)
	{
	  ptrChar[i2bytes+1] = gSdifLittleToBig[i2bytes];
	  ptrChar[i2bytes]   = gSdifLittleToBig[i2bytes + 1];
	}
      
      return nobjread;
    }
}





size_t
SdiffreadLittleEndian4 (void *ptr, size_t nobj, FILE *stream)
{
  /* little : [3] [2] [1] [0] --->  big : [0] [1] [2] [3] */
  size_t
    nobjread = 0,
    i4bytes,
    Mi4Bytes;
  unsigned char *ptrChar;

  ptrChar = (unsigned char*) ptr;

  if (nobj > _SdifBSLittleE / 4)
    {
      nobjread += SdiffreadLittleEndian4(ptr, _SdifBSLittleE / 4, stream);
      nobjread += SdiffreadLittleEndian4(((unsigned char*) ptr)+(_SdifBSLittleE),
					   nobj - (_SdifBSLittleE / 4),
					   stream);
      return nobjread;
    }
  else
    {
      nobjread = Sdiffread(gSdifLittleToBig, 4, nobj, stream);
      Mi4Bytes = nobj << 2;         /****  (nobj << 2)  <=>  (nobj*4)  *****/
      for(i4bytes=0; i4bytes<Mi4Bytes; i4bytes+=4)
        {
	      ptrChar[i4bytes+3] = gSdifLittleToBig[i4bytes];
	      ptrChar[i4bytes+2] = gSdifLittleToBig[i4bytes+1];
	      ptrChar[i4bytes+1] = gSdifLittleToBig[i4bytes+2];
	      ptrChar[i4bytes]   = gSdifLittleToBig[i4bytes+3];
        }
      
      return nobjread;
    }
}




size_t
SdiffreadLittleEndian8 (void *ptr, size_t nobj, FILE *stream)
{
  size_t
    nobjread = 0,
    i8bytes,
    Mi8Bytes;
  unsigned char *ptrChar;

  ptrChar = (unsigned char*) ptr;

  if (nobj > _SdifBSLittleE / 8)
    {
      nobjread += SdiffreadLittleEndian8(ptr, _SdifBSLittleE / 8, stream);
      nobjread += SdiffreadLittleEndian8(((unsigned char*) ptr)+(_SdifBSLittleE),
					   nobj - (_SdifBSLittleE / 8),
					   stream);
      return nobjread;
    }
  else
    {
      nobjread = Sdiffread(gSdifLittleToBig, 8, nobj, stream);
      Mi8Bytes = nobj << 3;         /****  (nobj << 2)  <=>  (nobj*8)  *****/
      for(i8bytes=0; i8bytes<Mi8Bytes; i8bytes+=8)
	{
	   ptrChar[i8bytes+7] = gSdifLittleToBig[i8bytes];
	   ptrChar[i8bytes+6] = gSdifLittleToBig[i8bytes+1];
	   ptrChar[i8bytes+5] = gSdifLittleToBig[i8bytes+2];
	   ptrChar[i8bytes+4] = gSdifLittleToBig[i8bytes+3];
	   ptrChar[i8bytes+3] = gSdifLittleToBig[i8bytes+4];
	   ptrChar[i8bytes+2] = gSdifLittleToBig[i8bytes+5];
	   ptrChar[i8bytes+1] = gSdifLittleToBig[i8bytes+6];
	   ptrChar[i8bytes]   = gSdifLittleToBig[i8bytes+7];
	}
      
      return nobjread;
    }
}







size_t
SdiffwriteLittleEndian2 (void *ptr, size_t nobj, FILE *stream)
{
  size_t
    nobjwrite = 0,
    i2bytes,
    Mi2Bytes;
  unsigned char *ptrChar;

  ptrChar = (unsigned char*) ptr;

  if (nobj > _SdifBSLittleE / 2)
    {
      nobjwrite += SdiffwriteLittleEndian2(ptr, _SdifBSLittleE / 2, stream);
      nobjwrite += SdiffwriteLittleEndian2(((unsigned char*) ptr)+(_SdifBSLittleE),
					   nobj - (_SdifBSLittleE / 2),
					   stream);
      return nobjwrite;
    }
  else
    {
      Mi2Bytes = nobj << 1;         /****  (nobj << 1)  <=>  (nobj*2)  *****/
      for(i2bytes=0; i2bytes<Mi2Bytes; i2bytes+=2)
	{
	  gSdifLittleToBig[i2bytes]     = ptrChar[i2bytes+1];
	  gSdifLittleToBig[i2bytes + 1] = ptrChar[i2bytes];
	}
      
      nobjwrite = Sdiffwrite(gSdifLittleToBig, 2, nobj, stream);
      return nobjwrite;
    }
}







size_t
SdiffwriteLittleEndian4 (void *ptr, size_t nobj, FILE *stream)
{
  /* little : [3] [2] [1] [0] --->  big : [0] [1] [2] [3] */
  size_t
    nobjwrite = 0,
    i4bytes,
    Mi4Bytes;
  unsigned char *ptrChar;

  ptrChar = (unsigned char*) ptr;

  if (nobj > _SdifBSLittleE / 4)
    {
      nobjwrite += SdiffwriteLittleEndian4(ptr, _SdifBSLittleE / 4, stream);
      nobjwrite += SdiffwriteLittleEndian4(((unsigned char*) ptr)+(_SdifBSLittleE),
					   nobj - (_SdifBSLittleE / 4),
					   stream);
      return nobjwrite;
    }
  else
    {
      Mi4Bytes = nobj << 2;         /****  (nobj << 2)  <=>  (nobj*4)  *****/
      for(i4bytes=0; i4bytes<Mi4Bytes; i4bytes+=4)
	{
	  gSdifLittleToBig[i4bytes]   = ptrChar[i4bytes+3];
	  gSdifLittleToBig[i4bytes+1] = ptrChar[i4bytes+2];
	  gSdifLittleToBig[i4bytes+2] = ptrChar[i4bytes+1];
	  gSdifLittleToBig[i4bytes+3] = ptrChar[i4bytes];
	}
      
      nobjwrite = Sdiffwrite(gSdifLittleToBig, 4, nobj, stream);
      return nobjwrite;
    }
}






size_t
SdiffwriteLittleEndian8 (void *ptr, size_t nobj, FILE *stream)
{
  size_t
    nobjwrite = 0,
    i8bytes,
    Mi8Bytes;
  unsigned char *ptrChar;

  ptrChar = (unsigned char*) ptr;

  if (nobj > _SdifBSLittleE / 8)
    {
      nobjwrite += SdiffwriteLittleEndian8(ptr, _SdifBSLittleE / 8, stream);
      nobjwrite += SdiffwriteLittleEndian8(((unsigned char*) ptr)+(_SdifBSLittleE),
					   nobj - (_SdifBSLittleE / 8),
					   stream);
      return nobjwrite;
    }
  else
    {
      Mi8Bytes = nobj << 3;         /****  (nobj << 2)  <=>  (nobj*8)  *****/
      for(i8bytes=0; i8bytes<Mi8Bytes; i8bytes+=8)
	{
	  gSdifLittleToBig[i8bytes]   = ptrChar[i8bytes+7];
	  gSdifLittleToBig[i8bytes+1] = ptrChar[i8bytes+6];
	  gSdifLittleToBig[i8bytes+2] = ptrChar[i8bytes+5];
	  gSdifLittleToBig[i8bytes+3] = ptrChar[i8bytes+4];
	  gSdifLittleToBig[i8bytes+4] = ptrChar[i8bytes+3];
	  gSdifLittleToBig[i8bytes+5] = ptrChar[i8bytes+2];
	  gSdifLittleToBig[i8bytes+6] = ptrChar[i8bytes+1];
	  gSdifLittleToBig[i8bytes+7] = ptrChar[i8bytes];
	}
      
      nobjwrite = Sdiffwrite(gSdifLittleToBig, 8, nobj, stream);
      return nobjwrite;
    }
}






/***************************************************************/
/***************************************************************/
/* Read */







size_t
SdiffReadInt2 (SdifInt2 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffreadLittleEndian2(ptr, nobj, stream);
    default :
    return Sdiffread(ptr, sizeof(SdifInt2),  nobj, stream);
    }  
}






size_t
SdiffReadUInt2(SdifUInt2 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffreadLittleEndian2(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifUInt2),  nobj, stream);
    }  
}





size_t
SdiffReadInt4(SdifInt4 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffreadLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifInt4),  nobj, stream);
    }  
}






size_t
SdiffReadUInt4(SdifUInt4 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffreadLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifUInt4),  nobj, stream);
    }  
}






/*
 *size_t
 *SdiffReadUInt8(SdifUInt8 *ptr, size_t nobj, FILE *stream)
 *{
 * switch (gSdifMachineType)
 *   {
 *   case eLittleEndian   :
 *   case eLittleEndian64 :
 *   case eLittleEndianLittleConst :
 *   case eLittleEndianLittleConst64 :
 *     return SdiffreadLittleEndian8(ptr, nobj, stream);
 *   default :
 *     return Sdiffread(ptr, sizeof(SdifUInt8),  nobj, stream);
 *   }  
 *}
 */



size_t
SdiffReadFloat4(SdifFloat4 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffreadLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifFloat4),  nobj, stream);
    }  
}






size_t
SdiffReadFloat8(SdifFloat8 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffreadLittleEndian8(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifFloat8),  nobj, stream);
    }  
}




/*
 * size_t
 * SdiffReadSignature(SdifSignature *Signature, FILE *stream)
 * {
 *   return Sdiffread(Signature, sizeof(Signature), 1, stream);
 * }
 */












/* Write */


size_t
SdiffWriteInt2 (SdifInt2 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffwriteLittleEndian2(ptr, nobj, stream);
      
    default :
      return Sdiffwrite(ptr, sizeof(SdifInt2),  nobj, stream);
    }  
}





size_t
SdiffWriteUInt2(SdifUInt2 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffwriteLittleEndian2(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifUInt2),  nobj, stream);
    }
    
}






size_t
SdiffWriteInt4(SdifInt4 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffwriteLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifInt4),  nobj, stream);
    }  
}






size_t
SdiffWriteUInt4(SdifUInt4 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffwriteLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifUInt4),  nobj, stream);
    }  
}







/*
 *size_t
 *SdiffWriteUInt8(SdifUInt8 *ptr, size_t nobj, FILE *stream)
 *{
 * switch (gSdifMachineType)
 *   {
 *     
 *   case eLittleEndian   :
 *   case eLittleEndian64 :
 *     return SdiffwriteLittleEndian8(ptr, nobj, stream);
 *   default :
 *     return Sdiffwrite(ptr, sizeof(SdifUInt8),  nobj, stream);
 *   }  
 *}
 */





size_t
SdiffWriteFloat4(SdifFloat4 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffwriteLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifFloat4),  nobj, stream);
    }  
}





size_t
SdiffWriteFloat8(SdifFloat8 *ptr, size_t nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      return SdiffwriteLittleEndian8(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifFloat8),  nobj, stream);
    }  
}






size_t
SdiffWriteSignature(SdifSignature *Signature, FILE *stream)
{
    SdifSignature SignW;

    SignW = *Signature ;
    switch (gSdifMachineType)
    {     
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      SdifLittleToBig(&SignW, sizeof(Signature));
      break;
    default :
      break;
    }
  return Sdiffwrite(&SignW, sizeof(SdifSignature),  1, stream);
}






size_t
SdiffWriteString(char* ptr, FILE *stream)
{
  return Sdiffwrite(ptr, sizeof(char), SdifStrLen(ptr), stream);
}








/*************** ASCII **************/



int
SdifIsAReservedChar(char c)
{
  if (memchr (_SdifReservedChars, c, SdifStrLen(_SdifReservedChars)))
    return (int) c;
  else
    return -1; 
}







/* SdiffGetString lit un fichier jusqu'a un caractere reserve,
 * ne rempli s que des caracteres non-espacement,
 * renvoie le caractere reserve,
 * saute les premiers caracteres espacement lus.
 * Il y a erreur 
 *     si fin de fichier
 *  ou si un caractere non-espacement et non-reseve
 *        est lu apres un caractere espacement.
 * ncMax est typiquement strlen(s)+1.
 */
int
SdiffGetString(FILE* fr, char* s, size_t ncMax, size_t *NbCharRead)
{
  int cint;
  char c;
  char *cs;
  int bool;

  cs = s;
  bool = 1;
  
  do
    {
      cint = fgetc(fr);
      c = (char) cint;
      (*NbCharRead)++;
    }
  while ( isspace(c) && (!feof(fr)) );


  while ( (ncMax-- > 0) && (!feof(fr)) )
    {
      if (SdifIsAReservedChar(c) != -1)
	{
	  *cs++ = '\0';
	  return cint;
	}
      else
	if (isspace(c)) /* end of word */
	  bool = 0;
	else
	  if (bool == 0) /* word finished and another word : word cut */
	    {
	      *cs++ = '\0';
	      _SdifError(eWordCut, s);
	      return -1;
	    }
          else
	    *cs++ = c;

      cint=fgetc(fr);
      c = (char) cint ;
      (*NbCharRead)++;
    }
  

  if (feof(fr))
    return eEof;
  else
    {
      *cs = '\0';
      _SdifError(eTokenLength, s);
      return eTokenLength;
    }
}










/* retourne le caractere d'erreur */
int
SdiffGetSignature(FILE* fr, SdifSignature *Signature, size_t *NbCharRead)
{
  char Name[4] = "\0\0\0";
  char c;
  int cint;
  char *pCS;
  unsigned int i;
  
  do
    {
      cint = fgetc(fr); 
      c = (char) cint ;
      (*NbCharRead)++;
    }
  while (isspace(c) && (!feof(fr)) );
  
  for(i=0; ((i<4) && (!feof(fr))) ; i++)
    {
      if ( (SdifIsAReservedChar(c) != -1) || (isspace(c)) )
	break;
      else
	{
	  Name[i] = c;
	  if (i < 4-1)
	    {
	      cint =fgetc(fr);
          c = (char) cint;
	      (*NbCharRead)++;
	    }
	}
    }
  
  pCS = (char*) Signature;
  pCS[0] = Name[0];
  pCS[1] = Name[1];
  pCS[2] = Name[2];
  pCS[3] = Name[3];/* effet de bord sur signature */


  switch (gSdifMachineType)
    {     
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      SdifBigToLittle(Signature, sizeof(Signature));
      break;
    default :
      break;
    }
  

  if (feof(fr))
    {
      *Signature = eEmptySignature;
      (*NbCharRead)--; /* to not count EOF read */
      return eEof;
    }
  else
    return cint;
}





size_t
SdiffReadSpace(FILE* fr)
{
  size_t NbCharRead = 0;
  SdiffPosT Pos = 0;
  char c;

  while ( isspace(c= (char) fgetc(fr)) )
  {
    NbCharRead++;
  }

  if (feof(fr))
    _SdifError(eEof, "SdiffReadSpace");
  else
    {
      SdiffGetPos(fr, &Pos);
      Pos --;
      if (SdiffSetPos(fr, &Pos)==0)
        /* if (c = (char) ungetc(c, fr)) */
        return NbCharRead;
      else
        {
	      sprintf(gSdifErrorMess, "ungetc failed : (%d,%c) ", c, c);
	      _SdifError(eEof, gSdifErrorMess);
        }
    }
  return NbCharRead;
}







/* fGetWordUntil : read word until a char which is a member of "CharsEnd".
 * the file "fr" must be set at the begining of the word (spaces before not read).
 * the spaces chars between the word and final char are ignored.
 * it return the final char.
 * "(*NbCharRead)" is increased by each byte read into "fr".
 */
int
SdiffGetWordUntil(FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd)
{
  char c=0;
  char *cs;
  int CharsEndLen;
  
  CharsEndLen = SdifStrLen(CharsEnd);
  cs = s;
  
  while( (c = (char) fgetc(fr)) && (ncMax-- > 0) && (!feof(fr)))
    {
      (*NbCharRead)++;
      
      if (memchr(CharsEnd, c, CharsEndLen))
	{
	  *cs = '\0';
	  return (int) c;
	}

      if (isspace(c))
	{
	  *NbCharRead += SdiffReadSpace(fr);
	  c = (char) fgetc(fr);
	  if (memchr(CharsEnd, c, CharsEndLen))
	    {
	      *cs = '\0';
	      return (int) c;
	    }
	  else
	    {
	      *cs++ = '\0';
	      /*_SdifError(eWordCut, s);*/
	      return -1;
	    }
	}

      *cs++ = c;
    }
    
  if (feof(fr))
    return eEof;
  
  if (ncMax <= 0)
    {
      *cs = '\0';
      _SdifError(eTokenLength, s);
      return eTokenLength;
    }
  return eFalse;
}









/* fGetStringUntil : is like fGetWordUntil.
 * but the space chars before the word are ignored.
 */
int
SdiffGetStringUntil(FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd)
{
  *NbCharRead += SdiffReadSpace(fr);
  return SdiffGetWordUntil(fr, s, ncMax, NbCharRead, CharsEnd);
}






/* fGetStringWeakUntil : is like fGetWordUntil.
 * but all chars (spaces too) are valid before CharsEnd.
 */
int
SdiffGetStringWeakUntil(FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd)
{
  char c=0;
  char *cs;
  int CharsEndLen;
  
  CharsEndLen = SdifStrLen(CharsEnd);
  cs = s;
  
  while( (c = (char) fgetc(fr)) && (ncMax-- > 0) && (!feof(fr)))
    {
      (*NbCharRead)++;
      
      if (memchr(CharsEnd, c, CharsEndLen))
	{
	  *cs = '\0';
	  return (int) c;
	}
      *cs++ = c;
    }
  
  if (feof(fr))
    return eEof;
  
  if (ncMax <= 0)
    {
      *cs = '\0';
      _SdifError(eTokenLength, s);
      return eTokenLength;
    }
  return eFalse;
}







/* read ASCII */

size_t
SdiffScanFloat4(FILE *stream, SdifFloat4 *ptr, size_t nobj)
{
  SdifUInt4 iobj;
  size_t NbObjR = 0;

  for(iobj=0; iobj<nobj; iobj++)
    NbObjR += fscanf(stream, "%f", &(ptr[iobj]));

  return NbObjR;
}



size_t
SdiffScanFloat8(FILE *stream, SdifFloat8 *ptr, size_t nobj)
{
  size_t iobj;
  size_t NbObjR = 0;

  for(iobj=0; iobj<nobj; iobj++)
    NbObjR += fscanf(stream, "%f", &(ptr[iobj]));

  return NbObjR;
}







int
SdifSkipASCIIUntil(FILE* fr, size_t *NbCharRead, char *CharsEnd)
{
  int c=0;
  int CharsEndLen;
  
  CharsEndLen = SdifStrLen(CharsEnd);
  
  while(   (c = fgetc(fr)) && (!feof(fr))   )
    {
      (*NbCharRead)++;
      
      if (memchr(CharsEnd, (char) c, CharsEndLen))
	return c;
      else
	if ((!isspace(c)) && (!isprint(c)))
	  {
	    break;
	  }
    }

  if (feof(fr))
    return eEof;
  else
    return c;

}
