/* $Id: SdifRWLowLevel.h,v 2.0 1998-11-29 11:42:05 virolle Exp $
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

size_t SdiffReadInt2   (SdifInt2   *ptr, size_t nobj, FILE *stream);
size_t SdiffReadUInt2  (SdifUInt2  *ptr, size_t nobj, FILE *stream);
size_t SdiffReadInt4   (SdifInt4   *ptr, size_t nobj, FILE *stream);
size_t SdiffReadUInt4  (SdifUInt4  *ptr, size_t nobj, FILE *stream);
size_t SdiffReadFloat4 (SdifFloat4 *ptr, size_t nobj, FILE *stream);
size_t SdiffReadFloat8 (SdifFloat8 *ptr, size_t nobj, FILE *stream);

/*size_t SdiffReadSignature (SdifSignature *Signature, FILE *stream);*/



/* Write, return the number of objects */

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

size_t SdiffScanFloat4  (FILE *stream, SdifFloat4 *ptr, size_t nobj);
size_t SdiffScanFloat8  (FILE *stream, SdifFloat8 *ptr, size_t nobj);

#endif /* _SdifRWLowLevel_ */
