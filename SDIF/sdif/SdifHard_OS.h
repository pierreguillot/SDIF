/* $Id: SdifHard_OS.h,v 3.3 1999-10-07 15:12:25 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifHard_OS.h
 *
 *
 *
 *
 * author: Dominique Virolle 1998
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.2  1999/06/18  16:23:58  schwarz
 * SdifSignatureCmpNoVersion dropped LAST byte on alpha, because the mask
 * 0x00ffffff was not byteswapped.  Introduced gSdifSignatureVersionMask,
 * which is initialised in SdifInitMachineType to appropriate value.
 *
 * Revision 3.1  1999/03/14  10:57:00  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:52  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:37  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:25  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:50  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
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

/* _Sdif_MIN_DOUBLE_ tested on SGI, DEC alpha, PCWin95 as 0xffefffffffffffff
 * include may be limits.h (float.h is sure with VisualC++5 Win 95 or NT)
 */
#define _Sdif_MIN_DOUBLE_ (- DBL_MAX)



#if defined(__mips64) || defined(__alpha)
#define _LONG64BITS_
#else
#define _LONG32BITS_
#endif

/* to do fpos_t compatible on MacinTosh */
#if defined(MACINTOSH) || defined(WIN32)
/* on mac or windows, seeking on a stream is always considered
   successful (return 0)! */
#define SdiffPosT long
#define SdiffGetPos(f,p) ((((*(p)) = ((f!= stdin) && (f!= stdout) && (f!= stderr)) ? ftell(f) : 0 ) == -1 ) ? -1 : 0)
#define SdiffSetPos(f,p) ((f!= stdin) && (f!= stdout) && (f!= stderr)) ? fseek(f, (long)(*(p)), SEEK_SET) : 0
#else
#define SdiffPosT fpos_t
#define SdiffGetPos(f,p)    fgetpos((f),(p))
#define SdiffSetPos(f,p)    fsetpos((f),(p))
#endif


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
extern SdifUInt4 gSdifSignatureVersionMask;

void SdifLittleToBig(void *BigPtr, void *LittlePtr, size_t size);
void SdifBigToLittle(void *InOutPtr, size_t size);


int       SdifStrLen  (const char *s);

/* returns 0 if strings are equal */
int       SdifStrCmp  (const char *s1, const char *s2);

/* returns true if strings are equal */
int	  SdifStrEq(const char *s1, const char *s2);
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
