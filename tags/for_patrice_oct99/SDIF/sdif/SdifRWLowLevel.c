/* $Id: SdifRWLowLevel.c,v 3.6 1999-10-07 15:05:53 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * Read Write Low Level. Machine sex, little-big endian control
 *
 * author: Dominique Virolle 1997
 *
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.5  1999/09/28  13:09:10  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.4  1999/09/23  16:06:50  schwarz
 * Fixed bug with size of signature (was 8 on alpha!).
 *
 * Revision 3.3  1999/09/20  13:23:35  schwarz
 * Optimized SdifStringToSignature.
 *
 * Revision 3.2  1999/08/31  10:05:46  schwarz
 * Extracted function SdifStringToSignature from SdiffGetSignature.
 *
 * Revision 3.1  1999/03/14  10:57:18  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:59  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:45  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 */


#include <preincluded.h>
#include "SdifRWLowLevel.h"
#include "SdifHard_OS.h"
#include "SdifError.h"


#include <string.h>
#include <ctype.h>
#include <assert.h>


extern int        gSdifInitialised;	/* can't include SdifFile.h */
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


/** fread **/

size_t
SdiffreadLittleEndianN (void *ptr, size_t size, size_t nobj, FILE *stream)
{
	/* size must be a power of 2 */
  size_t
    nobjread = 0,
	iNbytes,
    MiNBytes;
  unsigned char *ptrChar;

  ptrChar = (unsigned char*) ptr;


  nobjread = Sdiffread(ptrChar, size, nobj, stream);
  MiNBytes = nobjread * size;
  for (iNbytes=0; iNbytes < MiNBytes; iNbytes += size)
	  SdifBigToLittle(ptrChar+iNbytes, size);
  return nobjread ;
}




size_t
SdiffreadLittleEndian2 (void *ptr, size_t nobj, FILE *stream)
{
  return SdiffreadLittleEndianN (ptr, 2, nobj, stream) ;
}



size_t
SdiffreadLittleEndian4 (void *ptr, size_t nobj, FILE *stream)
{
  return SdiffreadLittleEndianN (ptr, 4, nobj, stream) ;
}



size_t
SdiffreadLittleEndian8 (void *ptr, size_t nobj, FILE *stream)
{
  return SdiffreadLittleEndianN (ptr, 8, nobj, stream) ;
}



/** fwrite **/


size_t
SdiffwriteLittleEndianN (void *ptr, size_t size, size_t nobj, FILE *stream)
{
	/* size must be a power of 2 */
  size_t
    nobjwrite = 0,
	iNbytes,
    MiNBytes,
	nobjInOneBuf;
  unsigned char *ptrChar;

  ptrChar = (unsigned char*) ptr;
  nobjInOneBuf = _SdifBSLittleE / size;
  if (nobj > nobjInOneBuf)
    {
      nobjwrite += SdiffwriteLittleEndianN(ptrChar, size, nobjInOneBuf, stream);
      nobjwrite += SdiffwriteLittleEndianN(ptrChar+_SdifBSLittleE, size,
		                                   nobj - nobjInOneBuf,
					                       stream);
      return nobjwrite;
    }
  else
    {
       MiNBytes = nobj * size;
       for (iNbytes=0; iNbytes < MiNBytes; iNbytes += size)
	      SdifLittleToBig(gSdifLittleToBig+iNbytes, ptrChar+iNbytes, size);
      
      nobjwrite = Sdiffwrite(gSdifLittleToBig, size, nobj, stream);
      return nobjwrite;
    }
}




size_t
SdiffwriteLittleEndian2 (void *ptr, size_t nobj, FILE *stream)
{
  return SdiffwriteLittleEndianN (ptr, 2, nobj, stream);
}



size_t
SdiffwriteLittleEndian4 (void *ptr, size_t nobj, FILE *stream)
{
  return SdiffwriteLittleEndianN (ptr, 4, nobj, stream);
}



size_t
SdiffwriteLittleEndian8 (void *ptr, size_t nobj, FILE *stream)
{
  return SdiffwriteLittleEndianN (ptr, 8, nobj, stream);
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

    switch (gSdifMachineType)
    {     
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      SdifLittleToBig(&SignW, Signature, sizeof(SdifSignature));
      break;
    default :
      SignW = *Signature ;
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



/* Unsafe but optimized version of SdifStringToSignature:
   Exactly 4 chars are considered, so make sure *str has at least that many! 
   The str pointer MUST be longword aligned (to the nearest 4 byte boundary).
*/
SdifSignature
_SdifStringToSignature (char *str)
{
  assert (gSdifInitialised  &&  "SDIF library not initialised!");

  switch (gSdifMachineType)
  {     
    case eLittleEndianLittleConst :
    case eLittleEndianLittleConst64 :
      {
	SdifSignature Signature = *((SdifSignature *) str);
	SdifBigToLittle((void *) &Signature, sizeof(SdifSignature));
	return (Signature);
      }
	
    default:
      return (*((SdifSignature *) str));
  }
}  



/* Safe version of SdifStringToSignature: str can point to any string
   position of any length.  
*/
SdifSignature
SdifStringToSignature (char *str)
{
  SdifSignature sigarr = eEmptySignature;  /* force 4 byte alignment */
  char		*sig   = (char *) &sigarr;

  if (*sig++ = *str++)
    if (*sig++ = *str++)
      if (*sig++ = *str++)
	if (*sig++ = *str++)
	  /* do nothing */;
  return _SdifStringToSignature ((char *) &sigarr);
}  



/* retourne le caractere d'erreur */
int
SdiffGetSignature(FILE* fr, SdifSignature *Signature, size_t *NbCharRead)
{
  SdifSignature sig   = eEmptySignature;  /* force 4 byte alignment */
  char		*Name = (char *) &sig;
  char c = 0;
  int cint = 0;
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
  
  if (feof(fr))
    {
      *Signature = eEmptySignature;
      (*NbCharRead)--; /* to not count EOF read */
      return eEof;
    }
  else
    {
      *Signature = _SdifStringToSignature (Name);
      return cint;
    }
}





size_t
SdiffReadSpace(FILE* fr)
{
  size_t NbCharRead = 0;
  char c;

  while ( isspace(c= (char) fgetc(fr)) )
  {
    NbCharRead++;
  }

  if (feof(fr))
    _SdifError(eEof, "SdiffReadSpace");
  else
    {
/*
      SdiffPosT Pos = 0;
      SdiffGetPos(fr, &Pos);
      Pos --;
      if (SdiffSetPos(fr, &Pos)==0) ::: it is a bad solution with stdin, virolle 99/01 
*/
      if (c = (char) ungetc(c, fr))
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
