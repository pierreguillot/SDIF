/* $Id: SdifHard_OS.h,v 2.0 1998-11-29 11:41:50 virolle Exp $
 *
 * SdifHard_OS.h
 *
 *
 *
 *
 * author: Dominique Virolle 1998
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.2  1998/11/10  15:31:49  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */

#ifndef _SdifHard_OS_
#define _SdifHard_OS_


#include <stdio.h>
#include <float.h>




#if defined(__mips64) || defined(__alpha)
#define _LONG64BITS_
#else
#define _LONG32BITS_
#endif

/* to do fpos_t compatible on MacinTosh */
#if defined(MACINTOSH) || defined(WIN32)
#define SdiffPosT long
#define SdiffGetPos(f,p)    ((((*(p)) = ftell(f)) == -1) ? -1 : 0)
#define SdiffSetPos(f,p)    fseek(f, (long)(*(p)), SEEK_SET)
#else
#define SdiffPosT fpos_t
#define SdiffGetPos(f,p)    fgetpos((f),(p))
#define SdiffSetPos(f,p)    fsetpos((f),(p))
#endif

#define _Sdif_MIN_DOUBLE_ (- DBL_MAX)

typedef short          SdifInt2;
typedef unsigned short SdifUInt2;
typedef int            SdifInt4;
typedef unsigned int   SdifUInt4;
typedef float          SdifFloat4;
typedef double         SdifFloat8;
typedef unsigned int   SdifSignature;


typedef enum SdifMachineE
{
  eUndefinedMachine,
  eBigEndian,
  eLittleEndian,
  eLittleEndianLittleConst,
  eBigEndian64,
  eLittleEndian64,
  eLittleEndianLittleConst64,
  ePDPEndian
} SdifMachineET;

SdifMachineET SdifGetMachineType(void);
extern SdifMachineET gSdifMachineType;
SdifMachineET SdifInitMachineType(void);

void SdifLittleToBig(void *BigPtr, void *LittlePtr, size_t size);
void SdifBigToLittle(void *InOutPtr, size_t size);


int       SdifStrLen  (const char *s);
int       SdifStrCmp  (const char *s1, const char *s2);
int       SdifStrNCmp (const char *s1, const char *s2, unsigned int n);
char*     SdifStrNCpy (char *s1, const char *s2, unsigned int n);
char*     SdifCreateStrNCpy (const char* Source, size_t Size);
void      SdifKillStr (char* String);



typedef enum SdifBinaryMode
{
  eBinaryModeUnknown,
  eBinaryModeWrite,
  eBinaryModeRead,
  eBinaryModeStdInput,
  eBinaryModeStdOutput,
  eBinaryModeStdError
} SdifBinaryModeET ;


void     SdifSetStdIOBinary (void);
FILE*    SdiffBinOpen       (const char * Name, SdifBinaryModeET Mode);
SdifInt4 SdiffBinClose      (FILE *f);

#endif /* _SdifHard_OS_ */
