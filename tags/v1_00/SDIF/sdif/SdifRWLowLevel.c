/* SdifRWLowLevel.c
 *
 *
 *
 */


#include "SdifRWLowLevel.h"

#include "SdifError.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>





static char gSdifLittleToBig [_SdifBSLittleE];






SdifMachineEnum
SdifGetMachineType(void)
{
  long
    *LongTest;
  char
    CharTab[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

  LongTest = (long*) CharTab;

#ifdef _LONG64BITS_
  switch (*LongTest)
    {
    case 0x0102030405060708 :
      return eBigEndian64;
    case 0x0807060504030201 :
      return eLittleEndian64;
    default :
      return eUndefinedMachine;
    }

#else
  switch (*LongTest)
    {
    case 0x01020304 :
      return eBigEndian;
    case 0x04030201 :
      return eLittleEndian;
   /*case 0x03040102 :
    *return ePDPEndian;
    */
    default :
      return eUndefinedMachine;
    }
#endif /* _LONG64BITS */
}




SdifMachineEnum gSdifMachineType;




SdifMachineEnum
SdifInitMachineType(void)
{
  gSdifMachineType = SdifGetMachineType();
  return gSdifMachineType;
}






/* fread encapsule */
int
Sdiffread(void *ptr, unsigned int size, unsigned int nobj, FILE *stream)
{
  static int nobjread;

  nobjread = fread(ptr, size, nobj, stream);
  if (nobjread != nobj)
    {
      sprintf(gSdifErrorMess, "Sdiffread %d", ftell(stream));
      _SdifError(eEof, gSdifErrorMess);
    }
  
  return nobjread;
}






/* fwrite encapsule */
int
Sdiffwrite(void *ptr, unsigned int size, unsigned int nobj, FILE *stream)
{
  static int nobjwrite;

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


int
SdiffreadLittleEndian2 (void *ptr, unsigned int nobj, FILE *stream)
{
  int nobjread = 0;
  unsigned int
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





int
SdiffreadLittleEndian4 (void *ptr, unsigned int nobj, FILE *stream)
{
  /* little : [3] [2] [1] [0] --->  big : [0] [1] [2] [3] */
   int nobjread = 0;
  unsigned int
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




int
SdiffreadLittleEndian8 (void *ptr, unsigned int nobj, FILE *stream)
{
  int nobjread = 0;
  unsigned int
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







int
SdiffwriteLittleEndian2 (void *ptr, unsigned int nobj, FILE *stream)
{
  int nobjwrite = 0;
  unsigned int
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







int
SdiffwriteLittleEndian4 (void *ptr, unsigned int nobj, FILE *stream)
{
  /* little : [3] [2] [1] [0] --->  big : [0] [1] [2] [3] */
   int nobjwrite = 0;
  unsigned int
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






int
SdiffwriteLittleEndian8 (void *ptr, unsigned int nobj, FILE *stream)
{
  int nobjwrite = 0;
  unsigned int
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







int
SdifFReadInt2 (SdifInt2 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffreadLittleEndian2(ptr, nobj, stream);
    default :
    return Sdiffread(ptr, sizeof(SdifInt2),  nobj, stream);
    }  
}






int
SdifFReadUInt2(SdifUInt2 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffreadLittleEndian2(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifUInt2),  nobj, stream);
    }  
}





int
SdifFReadInt4(SdifInt4 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffreadLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifInt4),  nobj, stream);
    }  
}






int
SdifFReadUInt4(SdifUInt4 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffreadLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifUInt4),  nobj, stream);
    }  
}







int
SdifFReadUInt8(SdifUInt8 *ptr, unsigned int nobj, FILE *stream)
{
  SdifUInt4 Hig, Low;

  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
      /*return SdiffreadLittleEndian4(ptr, 2*nobj, stream);*/
      return SdiffreadLittleEndian8(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifUInt8),  nobj, stream);
    }  
}




int
SdifFReadFloat4(SdifFloat4 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffreadLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifFloat4),  nobj, stream);
    }  
}






int
SdifFReadFloat8(SdifFloat8 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffreadLittleEndian8(ptr, nobj, stream);
    default :
      return Sdiffread(ptr, sizeof(SdifFloat8),  nobj, stream);
    }  
}




int
SdifFRead4Char(char *ptr, FILE *stream)
{
  return Sdiffread(ptr, sizeof(char), _SdifNameLen, stream);
}














/* Write */


int
SdifFWriteInt2 (SdifInt2 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffwriteLittleEndian2(ptr, nobj, stream);
      
    default :
      return Sdiffwrite(ptr, sizeof(SdifInt2),  nobj, stream);
    }  
}





int
SdifFWriteUInt2(SdifUInt2 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffwriteLittleEndian2(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifUInt2),  nobj, stream);
    }
    
}






int
SdifFWriteInt4(SdifInt4 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffwriteLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifInt4),  nobj, stream);
    }  
}






int
SdifFWriteUInt4(SdifUInt4 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffwriteLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifUInt4),  nobj, stream);
    }  
}








int
SdifFWriteUInt8(SdifUInt8 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
      /*return SdiffwriteLittleEndian4(ptr, 2*nobj, stream);*/
      return SdiffwriteLittleEndian8(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifUInt8),  nobj, stream);
    }  
}






int
SdifFWriteFloat4(SdifFloat4 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffwriteLittleEndian4(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifFloat4),  nobj, stream);
    }  
}





int
SdifFWriteFloat8(SdifFloat8 *ptr, unsigned int nobj, FILE *stream)
{
  switch (gSdifMachineType)
    {
      
    case eLittleEndian   :
    case eLittleEndian64 :
      return SdiffwriteLittleEndian8(ptr, nobj, stream);
    default :
      return Sdiffwrite(ptr, sizeof(SdifFloat8),  nobj, stream);
    }  
}






int
SdifFWrite4Char(char *ptr, FILE *stream)
{
  return Sdiffwrite(ptr, sizeof(char),  _SdifNameLen, stream);
}






int
SdifFWriteString(char* ptr, FILE *stream)
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







/* SdifFGetString lit un fichier jusqu'a un caractere reserve,
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
SdifFGetString(char* s, int ncMax, FILE* fr, int *NbCharRead)
{
  char c;
  char *cs;
  int bool;

  cs = s;
  bool = 1;
  
  do
    {
      c = fgetc(fr);
      (*NbCharRead)++;
    }
  while ( isspace(c) && (!feof(fr)) );


  while ( (ncMax-- > 0) && (!feof(fr)) )
    {
      if (SdifIsAReservedChar(c) != -1)
	{
	  *cs++ = '\0';
	  return (int) c;
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

      c=fgetc(fr);
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
SdifFGetName(char *Name, FILE* fr, int *NbCharRead)
{
  char c;
  unsigned int i;
  
  for(i=0; i<_SdifNameLen; i++)
    Name[i] = '\0';
  
  do
    {
      c = fgetc(fr); 
      (*NbCharRead)++;
    }
  while (isspace(c) && (!feof(fr)) );
  
  for(i=0; ((i<_SdifNameLen) && (!feof(fr))) ; i++)
    {
      if ( (SdifIsAReservedChar(c) != -1) || (isspace(c)) )
	return (unsigned) c;
      else
	{
	  Name[i] = c;
	  if (i < _SdifNameLen-1)
	    {
	      c=fgetc(fr);
	      (*NbCharRead)++;
	    }
	}
    }
  
  if (feof(fr))
    return eEof;
  else
    return (unsigned) c;
}








int
SdifFWritePadding(unsigned int Padding, FILE *fw)
{
  int NbBytesWrite = 0;

  if (Padding > 0)
    {
      memset(gSdifString, _SdifPaddingChar, Padding);
      NbBytesWrite += sizeof(char) * Sdiffwrite(gSdifString, sizeof(char), Padding, fw);
    }

  return  NbBytesWrite;
}







int
SdifFReadPadding (unsigned int Padding, FILE *fr)
{
  return sizeof(char) * Sdiffread(gSdifString, sizeof(char), Padding, fr);
}









int
SdifFReadUndeterminatedPadding(FILE *fr)
{
  int NbBytesRead = 0;
  int c;

  while ((c = fgetc(fr) == (int)' ') && (!feof(fr)))
    NbBytesRead++;
  
  if (feof(fr))
    return NbBytesRead;
  else
    {
      ungetc(c, fr);
      return NbBytesRead;
    }
}












int
SdifFReadSpace(FILE* fr)
{
  int NbCharRead = 0;
  char c;

  while ( isspace(c= fgetc(fr)) )
    NbCharRead++;

  if (feof(fr))
    _SdifError(eEof, "SdifFReadSpace");
  else
    if (fseek(fr, -1, SEEK_CUR)==0)
      /* if (c = (char) ungetc(c, fr)) */
      return NbCharRead;
    else
      {
	sprintf(gSdifErrorMess, "ungetc failed : (%d,%c) ", c, c);
	_SdifError(eEof, gSdifErrorMess);
      }
     
  return NbCharRead;
}







/* FGetWordUntil : read word until a char which is a member of "CharsEnd".
 * the file "fr" must be set at the begining of the word (spaces before not read).
 * the spaces chars between the word and final char are ignored.
 * it return the final char.
 * "(*NbCharRead)" is increased by each byte read into "fr".
 */
int
SdifFGetWordUntil(char* s, int ncMax, FILE* fr, int *NbCharRead, char *CharsEnd)
{
  static char c=0;
  char *cs;
  int CharsEndLen;
  
  CharsEndLen = SdifStrLen(CharsEnd);
  cs = s;
  
  while( (c = fgetc(fr)) && (ncMax-- > 0) )
    {
      (*NbCharRead)++;
      
      if (memchr(CharsEnd, c, CharsEndLen))
	{
	  *cs = '\0';
	  return (int) c;
	}

      if (isspace(c))
	{
	  *NbCharRead += SdifFReadSpace(fr);
	  c = fgetc(fr);
	  if (memchr(CharsEnd, c, CharsEndLen))
	    {
	      *cs = '\0';
	      return (int) c;
	    }
	  else
	    {
	      *cs++ = '\0';
	      _SdifError(eWordCut, s);
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









/* FGetStringUntil : is like FGetWordUntil.
 * but the space chars before the word are ignored.
 */
int
SdifFGetStringUntil(char* s, int ncMax, FILE* fr, int *NbCharRead, char *CharsEnd)
{
  *NbCharRead += SdifFReadSpace(fr);
  return SdifFGetWordUntil(s, ncMax, fr, NbCharRead, CharsEnd);
}



/* read ASCII */

int
SdifFScanFloat4(FILE *stream, SdifFloat4 *ptr, unsigned int nobj)
{
  SdifUInt4 iobj;
  int NbObjR = 0;

  for(iobj=0; iobj<nobj; iobj++)
    NbObjR += fscanf(stream, "%f", &(ptr[iobj]));

  return NbObjR;
}



int
SdifFScanFloat8(FILE *stream, SdifFloat8 *ptr, unsigned int nobj)
{
  SdifUInt4 iobj;
  int NbObjR = 0;

  for(iobj=0; iobj<nobj; iobj++)
    NbObjR += fscanf(stream, "%f", &(ptr[iobj]));

  return NbObjR;
}
