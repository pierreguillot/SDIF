// $Id: sdifdefines.i,v 1.1 2003-04-18 15:41:18 schwarz Exp $ -*-c-*-
//
// sdif_basics.h		16.04.2003		Diemo Schwarz
//
// define only the basic SDIF stuff for easdif swig from sdif.h
//
// $Log: not supported by cvs2svn $


/* SdifHard_OS.h */

typedef char           SdifChar;
typedef short          SdifInt2;
typedef unsigned short SdifUInt2;
typedef int            SdifInt4;
typedef unsigned int   SdifUInt4;
typedef float          SdifFloat4;
typedef double         SdifFloat8;
typedef unsigned int   SdifSignature;


/* SdifGlobals.h */

typedef enum SdifDataTypeE
{
  eText	    = 0x0301,
  eChar     = 0x0301,
  eFloat4   = 0x0004,
  eFloat8   = 0x0008,
  eInt2     = 0x0102,
  eInt4     = 0x0104,
  eInt8     = 0x0108,
  eUInt2    = 0x0202,
  eUInt4    = 0x0204,
  eUInt8    = 0x0208,
 	    
  eFloat4a  = 0x0001,	/* =  1 */    /* Backwards compatibility with old */
  eFloat4b  = 0x0020,	/* = 32 */    /* IRCAM versions < 3 of SDIF */
  eFloat8a  = 0x0002,	/* =  2 */    /* IN TEXT MODE ONLY! */
  eFloat8b  = 0x0040	/* = 64 */
} SdifDataTypeET;


/* SdifFileStruct.h */

typedef enum SdifFileModeE
{
  eUnknownFileMode,	/* 0 */
  eWriteFile,
  eReadFile,
  ePredefinedTypes,	/* 3 */
} SdifFileModeET;

