/* SdifRWLowLevel.h
 *
 *
 * Read Write Low Level. Machine sex, little-big endian control
 *
 *
 * author: Dominique Virolle 1997
 *
 */

#ifndef _SdifRWLowLevel_
#define _SdifRWLowLevel_

#if defined(__mips64) || defined(__alpha)
#define _LONG64BITS_
#else
#define _LONG32BITS_
#endif

#include "SdifGlobals.h"
#include <stdio.h>

#define _SdifBSLittleE 4096

#define _SdifPaddingChar  '\0'
#define _SdifReservedChars  ",;{}:"

typedef enum SdifMachineE
{
  eUndefinedMachine,
  eBigEndian,
  eLittleEndian,
  eBigEndian64,
  eLittleEndian64,
  ePDPEndian
} SdifMachineET;

extern SdifMachineET SdifGetMachineType(void);
extern SdifMachineET gSdifMachineType;
extern SdifMachineET SdifInitMachineType(void);

extern size_t Sdiffread  (void *ptr, size_t size, size_t nobj, FILE *stream);
extern size_t Sdiffwrite (void *ptr, size_t size, size_t nobj, FILE *stream);

/* Read, return the number of objects */

extern size_t SdiffReadInt2   (SdifInt2   *ptr, size_t nobj, FILE *stream);
extern size_t SdiffReadUInt2  (SdifUInt2  *ptr, size_t nobj, FILE *stream);
extern size_t SdiffReadInt4   (SdifInt4   *ptr, size_t nobj, FILE *stream);
extern size_t SdiffReadUInt4  (SdifUInt4  *ptr, size_t nobj, FILE *stream);
extern size_t SdiffReadFloat4 (SdifFloat4 *ptr, size_t nobj, FILE *stream);
extern size_t SdiffReadFloat8 (SdifFloat8 *ptr, size_t nobj, FILE *stream);

extern size_t SdiffReadSignature (SdifSignature *Signature, FILE *stream);



/* Write, return the number of objects */

extern size_t SdiffWriteInt2   (SdifInt2   *ptr, size_t nobj, FILE *stream);
extern size_t SdiffWriteUInt2  (SdifUInt2  *ptr, size_t nobj, FILE *stream);
extern size_t SdiffWriteInt4   (SdifInt4   *ptr, size_t nobj, FILE *stream);
extern size_t SdiffWriteUInt4  (SdifUInt4  *ptr, size_t nobj, FILE *stream);
extern size_t SdiffWriteFloat4 (SdifFloat4 *ptr, size_t nobj, FILE *stream);
extern size_t SdiffWriteFloat8 (SdifFloat8 *ptr, size_t nobj, FILE *stream);

extern size_t SdiffWriteSignature (SdifSignature *Signature, FILE *stream);
extern size_t SdiffWriteString (char* ptr, FILE *stream);

/*
 *extern size_t SdiffReadUInt8  (SdifUInt8  *ptr, size_t nobj, FILE *stream);
 *extern size_t SdiffWriteUInt8  (SdifUInt8  *ptr, size_t nobj, FILE *stream);
 */

/**Ascii**/
/* fGet --> return the last char
 */
extern size_t SdiffReadSpace   (FILE* fr);

extern int SdifIsAReservedChar (char c);
extern int SdiffGetString      (FILE* fr, char* s, size_t ncMax, size_t *NbCharRead);
extern int SdiffGetSignature   (FILE* fr, SdifSignature *Signature, size_t *NbCharRead);
extern int SdiffGetWordUntil   (FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd);
extern int SdiffGetStringUntil (FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd);
extern int SdiffGetStringWeakUntil(FILE* fr, char* s, size_t ncMax, size_t *NbCharRead, char *CharsEnd);
extern int SdifSkipASCIIUntil  (FILE* fr, size_t *NbCharRead, char *CharsEnd);

extern size_t SdiffScanFloat4  (FILE *stream, SdifFloat4 *ptr, size_t nobj);
extern size_t SdiffScanFloat8  (FILE *stream, SdifFloat8 *ptr, size_t nobj);

#endif /* _SdifRWLowLevel_ */
