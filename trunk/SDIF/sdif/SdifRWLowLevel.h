/* $Id: SdifRWLowLevel.h,v 3.5 1999-10-13 16:05:57 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifRWLowLevel.h
 *
 *
 * Read Write Low Level. Machine sex, little-big endian control
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.4  1999/09/28  13:09:11  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.3  1999/09/20  13:23:35  schwarz
 * Optimized SdifStringToSignature.
 *
 * Revision 3.2  1999/08/31  10:05:48  schwarz
 * Extracted function SdifStringToSignature from SdiffGetSignature.
 *
 * Revision 3.1  1999/03/14  10:57:19  virolle
 * SdifStdErr add
 *
 * Revision 2.1  1998/12/21  18:27:38  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:42:05  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.5  1998/11/10  15:31:55  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */

#ifndef _SdifRWLowLevel_
#define _SdifRWLowLevel_


#include "SdifGlobals.h"
#include <stdio.h>

#define _SdifBSLittleE 4096

#define _SdifPaddingChar  '\0'
#define _SdifReservedChars  ",;{}:"


size_t Sdiffread  (void *ptr, size_t size, size_t nobj, FILE *stream);
size_t Sdiffwrite (void *ptr, size_t size, size_t nobj, FILE *stream);

/* Read, return the number of objects */

size_t SdiffReadChar   (SdifChar   *ptr, size_t nobj, FILE *stream);
size_t SdiffReadInt2   (SdifInt2   *ptr, size_t nobj, FILE *stream);
size_t SdiffReadUInt2  (SdifUInt2  *ptr, size_t nobj, FILE *stream);
size_t SdiffReadInt4   (SdifInt4   *ptr, size_t nobj, FILE *stream);
size_t SdiffReadUInt4  (SdifUInt4  *ptr, size_t nobj, FILE *stream);
size_t SdiffReadFloat4 (SdifFloat4 *ptr, size_t nobj, FILE *stream);
size_t SdiffReadFloat8 (SdifFloat8 *ptr, size_t nobj, FILE *stream);

/*size_t SdiffReadSignature (SdifSignature *Signature, FILE *stream);*/



/* Write, return the number of objects */

size_t SdiffWriteChar   (SdifChar   *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteInt2   (SdifInt2   *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteUInt2  (SdifUInt2  *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteInt4   (SdifInt4   *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteUInt4  (SdifUInt4  *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteFloat4 (SdifFloat4 *ptr, size_t nobj, FILE *stream);
size_t SdiffWriteFloat8 (SdifFloat8 *ptr, size_t nobj, FILE *stream);

size_t SdiffWriteSignature (SdifSignature *Signature, FILE *stream);
size_t SdiffWriteString (char* ptr, FILE *stream);

/*
 *size_t SdiffReadUInt8  (SdifUInt8  *ptr, size_t nobj, FILE *stream);
 *size_t SdiffWriteUInt8  (SdifUInt8  *ptr, size_t nobj, FILE *stream);
 */

/**Ascii**/
/* fGet --> return the last char
 */
size_t SdiffReadSpace   (FILE* fr);

int SdifIsAReservedChar (char c);
int SdiffGetString      (FILE* fr, char* s, size_t ncMax, size_t *NbCharRead);
int SdiffGetSignature   (FILE* fr, SdifSignature *Signature, size_t *NbCharRead);
int SdiffGetWordUntil   (FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd);
int SdiffGetStringUntil (FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd);
int SdiffGetStringWeakUntil(FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd);
int SdifSkipASCIIUntil  (FILE* fr, size_t *NbCharRead, char *CharsEnd);


#if 0	/* for cocoon */
/* scan nobj items of TYPE from stream, return number sucessfully read */
size_t SdiffScan_TYPE   (FILE *stream, Sdif_TYPE  *ptr, size_t nobj);
size_t SdiffScanFloat4  (FILE *stream, SdifFloat4 *ptr, size_t nobj);
size_t SdiffScanFloat8  (FILE *stream, SdifFloat8 *ptr, size_t nobj);
#endif


#ifdef __STDC__  /* Is the compiler ANSI? */

#define sdif_scanproto(type) \
size_t SdiffScan##type (FILE *stream, Sdif##type *ptr, size_t nobj)

sdif_proto_foralltypes (sdif_scanproto)

#endif /* __STDC__ */


/* Unsafe but optimized version of SdifStringToSignature:
   Exactly 4 chars are considered, so make sure *str has at least that many! 
   The str pointer MUST be word (at least 4 byte or so) aligned.
*/
SdifSignature _SdifStringToSignature (char *str);

/*DOC:
  Convert a string to an SDIF signature (in proper endianness).
  str can point to any string position of any length.  
*/
SdifSignature SdifStringToSignature (char *str);

#endif /* _SdifRWLowLevel_ */
