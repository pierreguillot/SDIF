/* $Id: SdifGlobals.h,v 1.6 1998-11-10 15:31:48 schwarz Exp $
 *
 * SdifGlobals.h
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.5  1998/07/23  17:02:53  virolle
 * *** empty log message ***
 *
 * Revision 1.4  1998/04/24  12:40:34  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 */

#ifndef _SdifGlobals_
#define _SdifGlobals_

#include <stdio.h>
#include "SdifError.h"
#include "SdifHard_OS.h"

#define _SdifLibraryVersion "1.00"
#define _SdifTypesVersion   "1.00"


/* _SdifEnvVar : Environnement variable which contains the name
 * of the file which contains predefined types (the name contains the path).
 * _SdifEnvVar is used in SdifFile.c SdifGenInit, the user can
 * reference predefined types by this envvar name.
 */
#define _SdifEnvVar "SDIFTYPES"

/* Default predefined types : _SdifTypesFileName see SdifFile.c
 */


#define _SdifGenHashSize 127
#define _SdifUnknownSize 0xffffffff
#define _SdifPadding 8
#define _SdifGranule 1024 /* for OneRow allocation in bytes */

#define _SdifFloat8Error 0xffffffff

typedef short          SdifInt2;
typedef unsigned short SdifUInt2;
typedef int            SdifInt4;
typedef unsigned int   SdifUInt4;
typedef float          SdifFloat4;
typedef double         SdifFloat8;
typedef unsigned int   SdifSignature;





typedef enum SdifSignatureE
{
  eSDIF = 'SDIF' ,         /* SDIF header */
  e1NVT = '1NVT' ,         /* Name Value Table */
  e1TYP = '1TYP' ,         /* TYPe declarations */
  e1MTD = '1MTD' ,         /* Matrix Type Declaration */
  e1FTD = '1FTD' ,         /* Frame Type Declaration */
  e1IDS = '1IDS' ,         /* ID Stream Table */
  eSDFC = 'SDFC' ,         /* Start Data Frame Chunk (text files) */
  eENDC = 'ENDC' ,         /* END Chunk (text files) */
  eENDF = 'ENDF' ,         /* END File (text files) */
  eFORM = 'FORM' ,         /* FORM for IFF compatibility (obsolete ?) */
  eEmptySignature = '\0\0\0\0'
} SdifSignatureET;




#define _SdifFloatEps  1.0e-20

typedef enum SdifModifModeE
{
  eNoModif,
  eCanModif
} SdifModifModeET;

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
} SdifDataTypeET;


#define _SdifStringLen 1024

extern char gSdifString[_SdifStringLen];
extern char gSdifString2[_SdifStringLen];
extern char gSdifErrorMess[_SdifStringLen];

#define _SdifNbMaxPrintSignature 8
extern char gSdifStringSignature[_SdifNbMaxPrintSignature][5];
extern int  CurrStringPosSignature;

char*     SdifSignatureToString     (SdifSignature Signature);
short     SdifSignatureCmpNoVersion (SdifSignature Signature1, 
				     SdifSignature Signature2);
int       SdifStrLen  (const char *s);
int       SdifStrCmp  (const char *s1, const char *s2);
int       SdifStrNCmp (const char *s1, const char *s2, unsigned int n);
char*     SdifStrNCpy (char *s1, const char *s2, unsigned int n);
char*     SdifCreateStrNCpy (const char* Source, size_t Size);
void      SdifKillStr (char* String);
SdifUInt4 SdifSizeofDataType (SdifDataTypeET DataType);
size_t    SdifPaddingCalculate  (size_t NbBytes);
size_t    SdifFPaddingCalculate (FILE *f, size_t NbBytes);

/* (double f1) == (double f2) with _SdifFloatEps for error */
extern short SdifFloat8Equ(SdifFloat8 f1, SdifFloat8 f2);


#endif /* _SdifGlobals_ */
