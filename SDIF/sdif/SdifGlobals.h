/* $Id: SdifGlobals.h,v 2.1 1998-12-21 18:27:23 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifGlobals.h
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.0  1998/11/29  11:41:48  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.6  1998/11/10  15:31:48  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 * Revision 1.5  1998/07/23  17:02:53  virolle
 * *** empty log message ***
 *
 * Revision 1.4  1998/04/24  12:40:34  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 */

#ifndef _SdifGlobals_
#define _SdifGlobals_

#include <stdio.h>
#include <float.h>
#include "SdifError.h"
#include "SdifHard_OS.h"

#define _SdifFormatVersion 2
#define _SdifTypesVersion  1


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

#define _SdifFloat8Error  0xffffffff
#define _SdifNoStreamID   0xffffffff
#define _SdifUnknownUInt4 0xffffffff

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




#define _SdifFloatEps  FLT_EPSILON

typedef enum SdifModifModeE
{
  eNoModif,
  eCanModif
} SdifModifModeET;

/* DataTypeEnum
 */
typedef enum SdifDataTypeE
{
  eFloat4 = 1,
  eFloat8 = 2,
  eInt4   = 3,
  eUInt4  = 4,
  eChar4  = 5,
  eInt2   = 6,
  eUInt2  = 7,
  eFloat4Old = 32
} SdifDataTypeET;


#define _SdifStringLen 1024

extern char gSdifString[_SdifStringLen];
extern char gSdifString2[_SdifStringLen];
extern char gSdifErrorMess[_SdifStringLen];

#define _SdifNbMaxPrintSignature 8
extern char gSdifStringSignature[_SdifNbMaxPrintSignature][5];
extern int  CurrStringPosSignature;

char*     SdifSignatureToString(SdifSignature Signature);
short     SdifSignatureCmpNoVersion(SdifSignature Signature1, SdifSignature Signature2);
SdifUInt4 SdifSizeofDataType (SdifDataTypeET DataType);
size_t    SdifPaddingCalculate  (size_t NbBytes);
size_t    SdifFPaddingCalculate (FILE *f, size_t NbBytes);

/* (double f1) == (double f2) with _SdifFloatEps for error */
short SdifFloat8Equ(SdifFloat8 f1, SdifFloat8 f2);


#endif /* _SdifGlobals_ */
