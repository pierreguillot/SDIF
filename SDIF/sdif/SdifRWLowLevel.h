/* SdifRWLowLevel.h
 *
 *
 */

#ifndef _SdifRWLowLevel_
#define _SdifRWLowLevel_

#include "SdifGlobals.h"
#include "SdifUInt8.h"

#include <stdio.h>

#define _SdifBSLittleE 4096

#define _SdifPaddingChar  '\0'
#define _SdifReservedChars  ",;{}[]:"

typedef enum SdifMachineE
{
  eUndefinedMachine,
  eBigEndian,
  eLittleEndian,
  eBigEndian64,
  eLittleEndian64,
  ePDPEndian
} SdifMachineEnum;

extern SdifMachineEnum
SdifGetMachineType(void);

extern SdifMachineEnum gSdifMachineType;

extern SdifMachineEnum
SdifInitMachineType(void);


/* Read */

extern int
SdifFReadInt2   (SdifInt2 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFReadUInt2  (SdifUInt2 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFReadInt4   (SdifInt4 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFReadUInt4  (SdifUInt4 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFReadUInt8  (SdifUInt8 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFReadFloat4 (SdifFloat4 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFReadFloat8 (SdifFloat8 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFRead4Char (char *ptr, FILE *stream);



/* Write */

extern int
SdifFWriteInt2   (SdifInt2 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFWriteUInt2  (SdifUInt2 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFWriteInt4   (SdifInt4 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFWriteUInt4  (SdifUInt4 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFWriteUInt8  (SdifUInt8 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFWriteFloat4 (SdifFloat4 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFWriteFloat8 (SdifFloat8 *ptr, unsigned int nobj, FILE *stream);

extern int
SdifFWrite4Char (char *ptr, FILE *stream);

extern int
SdifFWriteString(char* ptr, FILE *stream);




/**Ascii**/
/* FGet --> return the last char
 * FRead --> return the number of read char
 */


extern int
SdifIsAReservedChar(char c);

extern int
SdifFGetString(char* s, int ncMax, FILE* fr, int *NbCharRead);

extern int
SdifFGetName(char *Name, FILE* fr, int *NbCharRead);

extern int
SdifFWritePadding(unsigned int Padding, FILE *fw);

extern int
SdifFReadPadding(unsigned int Padding, FILE *fr);

extern int
SdifFReadUndeterminatedPadding(FILE *fr);

extern int
SdifFReadSpace(FILE* fr);

extern int
SdifFGetWordUntil(char* s, int ncMax, FILE* fr, int *NbCharRead,char *CharsEnd);

extern int
SdifFGetStringUntil(char* s, int ncMax, FILE* fr, int *NbCharRead, char *CharsEnd);

extern int
SdifFScanFloat4(FILE *stream, SdifFloat4 *ptr, unsigned int nobj);

extern int
SdifFScanFloat8(FILE *stream, SdifFloat8 *ptr, unsigned int nobj);

#endif /* _SdifRWLowLevel_ */
