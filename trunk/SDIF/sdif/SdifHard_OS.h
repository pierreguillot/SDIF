/* $Id: SdifHard_OS.h,v 1.2 1998-11-10 15:31:49 schwarz Exp $
 *
 * SdifHard_OS.h
 *
 *
 *
 *
 * author: Dominique Virolle 1998
 *
 * $Log: not supported by cvs2svn $
 */

#ifndef _SdifHard_OS_
#define _SdifHard_OS_


#include <stdio.h>



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

extern SdifMachineET gSdifMachineType;
SdifMachineET SdifGetMachineType(void);
SdifMachineET SdifInitMachineType(void);

void SdifLittleToBig(void *ptr, size_t size);
void SdifBigToLittle(void *ptr, size_t size);

#endif /* _SdifHard_OS_ */
