/* SdifGlobals.h
 *
 *
 *
 */

#ifndef _SdifGlobals_
#define _SdifGlobals_

#include "SdifHash.h"
#include "SdifError.h"

#include <stdio.h>
#include "SdifUInt8.h"

#define _SdifLibraryVersion "1.00.alpha"
#define _SdifTypesVersion   "1.00.alpha"
#define _SdifTypesFileName  "SdifTypes.STYP"

#define _SdifGenHashSize 127
#define _SdifNameLen  4
#define _SdifUnknownSize 0xffffffff   /* except UnknowFileSize which is in 64 bits */
#define _SdifPadding 8

#define _SdifFloat8Error 0xffffffff

typedef short          SdifInt2;
typedef unsigned short SdifUInt2;
typedef int            SdifInt4;
typedef unsigned int   SdifUInt4;
typedef float          SdifFloat4;
typedef double         SdifFloat8;
/* SdifUInt8 type declaration into SdifUInt8.h depending machine */

#define _SdifFloatEps  1.0e-20

typedef enum SdifPredefinedE
{
  eUserdefined,
  ePredefined
} SdifPredefinedEnum;


/* DataTypeEnum
 * 8 bits at zero
 * 4 bits : 0-->float,  1-->int, 2->char
 * 4 bits : 0-->signed, 1-->unsigned
 * 8 bits : data width
 */
typedef enum SdifDataTypeE
{
  eFloat4 = 0x20,
  eFloat8 = 0x40,
  eInt2   = 0x1010,
  eUInt2  = 0x1110,
  eInt4   = 0x1020,
  eUInt4  = 0x1120,
  eChar4  = 0x2020
} SdifDataTypeEnum;


extern SdifHashTableType* gSdifMatrixTypesTable;
extern SdifHashTableType* gSdifFrameTypesTable;
extern SdifHashTableType* gSdifStreamIDsTable;

#define _SdifStringLen 1024

extern char gSdifString[_SdifStringLen];
extern char gSdifString2[_SdifStringLen];
extern char gSdifErrorMess[_SdifStringLen];

extern void
SdifInitAllHashTables(unsigned int HashSize);

extern void
SdifKillAllHashTables(void);

extern int
SdifPrintName(FILE* fw, char *Name);

extern int
SdifStrLen(const char *s);

extern int
SdifStrCmp(const char *s1, const char *s2);

extern int
SdifStrNCmp(const char *s1, const char *s2, unsigned int n);

extern int
SdifNameCmpWithoutVersion(const char NToTest[],
			  const char WithN[]);

extern char*
SdifStrNCpy(char *s1, const char *s2, unsigned int n);

extern char*
SdifCreateStrNCpy(char* Source, unsigned int Size);

extern void
SdifKillStr(char* String);

extern SdifUInt4
SdifSizeofDataType(SdifDataTypeEnum DataType);

extern unsigned int
SdifPaddingCalculate(int NbBytes);

extern unsigned int
SdifFPaddingCalculate(FILE *f, int NbBytes);


#define _SDIF "SDIF"
#define _FORM "FORM"
#define _SITC "SITC"
#define _STYP "STYP"
#define _SSIC "SSIC"
#define _SDFC "SDFC"
#define _Fram "Fram"
#define _Mtrx "Mtrx"
#define _ENDF "ENDF"
#define _ENDC "ENDC"



extern void
SdifGenInit(unsigned int HashSize, char* TypesFileName);

extern void
SdifGenKill(void);


/* (double f1) == (double f2) with _SdifFloatEps for error */
extern short
SdifFloat8Equ(SdifFloat8 f1, SdifFloat8 f2);

#endif /* _SdifGlobals_ */
