/* $Id: SdifGlobals.h,v 3.11 2000-11-21 14:51:49 schwarz Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
 *
 *
 * SdifGlobals.h
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.10  2000/11/15 14:53:30  lefevre
 * no message
 *
 * Revision 3.9  2000/10/27  20:03:34  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.8.2.2  2000/08/21  21:35:24  tisseran
 * *** empty log message ***
 *
 * Revision 3.8.2.1  2000/08/21  14:04:18  tisseran
 * *** empty log message ***
 *
 * Revision 3.8  2000/04/11  14:44:11  schwarz
 * Use Char for Text in SDIF types.
 *
 * Revision 3.7  2000/03/01  11:17:37  schwarz
 * Backwards compatibility of data types for reading text.
 *
 * Revision 3.6  1999/11/03  16:42:35  schwarz
 * Use _SdifNVTStreamID for stream ID of 1NVT frames because of CNMAT
 * restriction of only one frame type per stream.
 * (See SdifNameValuesLNewTable)
 *
 * Revision 3.5  1999/10/15  12:27:08  schwarz
 * Changed min/max to upper case.
 *
 * Revision 3.4  1999/10/13  16:05:49  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
 * Revision 3.3  1999/09/28  10:39:26  schwarz
 * Introduced SdifSignatureConst for signature constants across different
 * compilers/architectures.
 *
 * Revision 3.2  1999/09/20  13:28:05  schwarz
 * Introduced min/max macros.  (Why oh why is something so basic not part
 * of the standard? --- there is no macros.h on sgi!)
 *
 * Revision 3.1  1999/03/14  10:56:58  virolle
 * SdifStdErr add
 *
 * Revision 2.4  1999/02/28  12:16:46  virolle
 * memory report
 *
 * Revision 2.3  1999/01/23  15:55:50  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:35  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:23  schwarz
 * Inserted copyright message.
 *
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
#include "SdifMemory.h"
#include "SdifHard_OS.h"



/* set default if not overridden from makefile */
#ifndef _SdifFormatVersion
#define	_SdifFormatVersion 3
#endif

#define _SdifTypesVersion  1


/* _SdifEnvVar : Environnement variable which contains the name
 * of the file which contains predefined types (the name contains the path).
 * _SdifEnvVar is used in SdifFile.c SdifGenInit, the user can
 * reference predefined types by this envvar name.
 */
#define _SdifEnvVar "SDIFTYPES"

/* Default predefined types : _SdifTypesFileName see SdifFile.c
 */

#define _SdifListNodeStockSize 0x400 /*1024*/
#define _SdifGenHashSize 127
#define _SdifUnknownSize 0xffffffff
#define _SdifPadding 8
#define _SdifGranule 1024 /* for OneRow allocation in bytes */

#define _SdifFloat8Error  0xffffffff
#define _SdifNoTime	  _Sdif_MIN_DOUBLE_	/* for header ASCII frames */
#define _SdifNoStreamID   0xfffffffe		/* used for 1TYP */
#define _SdifAllStreamID  0xffffffff		/* used for 1IDS */
#define _SdifUnknownUInt4 0xffffffff

/* CNMAT restriction: only one frame type per stream.  
   Therefore we have to use unique IDs for all 'header' frames. */
#define _SdifNVTStreamID  0xfffffffd		/* used for 1NVT */
#define _SdifIDSStreamID  0xfffffffc		/* unused */
#define _SdifTYPStreamID  0xfffffffb		/* unused */



#if COCOONSEYESONLY
/*DOC:
  Macro to generate proper-endianed 4 char SDIF signature from 
  something like 'ABCD'.
 */
SdifUInt4 SdifSignatureConst (SdifUInt4 four_char_code);
#endif

/* This seems to be no longer necessary with gcc version 2.8.1 on alpha */
#if 0 && defined (__GNUC__)  &&  defined (__alpha__)
/* Swap multibyte char constant s with gcc on alpha,
   because gcc builds multi-character constants in inverse oder, as can
   be seen in cexp.y (variable c is the next character parsed):
 
   // Merge character into result; ignore excess chars. 
   if (num_chars <= max_chars)
   {
     if (width < HOST_BITS_PER_WIDE_INT)
       result = (result << width) | c;
     else
       result = c;
     token_buffer[num_chars - 1] = c;
   } */
#   define isolate(s, n)    ((s) & (0xff << ((n) * 8)))   /* s is char const */
#   define shiftto0(s, n)   (s >> ((n) * 8))
#   define pick(s, n)	    (shiftto0 (isolate (s, n), n) << ((3 - (n)) * 8))
#   define SdifSignatureConst(s) (pick(s,0)|pick(s,1)|pick(s,2)|pick(s,3))
#else
#   define SdifSignatureConst(s) (s)
#endif



#define _SdifFloatEps  FLT_EPSILON


#if (_SdifFormatVersion >= 3)

#ifdef STDC_HEADERS  /* Is the compiler ANSI? */

/* generate template for all types */
#define sdif__foralltypes(macro, post)	macro(Float4)post \
					macro(Float8)post \
					macro(Int2  )post \
					macro(Int4  )post \
					macro(UInt2 )post \
					macro(UInt4 )post \
					macro(Char  )post \
				     /* macro(Int8  )post \
					macro(UInt8 )post \
				      */

/* generate template for all types */
#define sdif_foralltypes(macro)		sdif__foralltypes(macro,)

/* generate prototype template for all types */
#define sdif_proto_foralltypes(macro)	sdif__foralltypes(macro,;)

#endif /* STDC_HEADERS */

#else

typedef enum SdifDataTypeE
{
  eUnicode  = 0,
  eFloat4   = 1,
  eFloat8   = 2,
  eInt4     = 3,
  eUInt4    = 4,
  eChar     = 5,
  eInt2     = 6,
  eUInt2    = 7,
  eFloat4Old = 32
} SdifDataTypeET;
#endif


#define _SdifStringLen 1024

extern char gSdifString[_SdifStringLen];
extern char gSdifString2[_SdifStringLen];
extern char gSdifErrorMess[_SdifStringLen];

#define _SdifNbMaxPrintSignature 8
extern char gSdifStringSignature[_SdifNbMaxPrintSignature][5];
extern int  CurrStringPosSignature;


/*
// FUNCTION GROUP:	utility functions
*/

/*DOC:
*/
char*     SdifSignatureToString(SdifSignature Signature);

/*DOC: 
  Returns size of SDIF data type in bytes
  (which is always the low-order byte).  
*/
SdifUInt4 SdifSizeofDataType (SdifDataTypeET DataType);

/*DOC: 
  Returns true if DataType is in the list of known data types.
*/
int SdifDataTypeKnown (SdifDataTypeET DataType);

/*DOC:
*/
size_t    SdifPaddingCalculate  (size_t NbBytes);

/*DOC:
*/
size_t    SdifFPaddingCalculate (FILE *f, size_t NbBytes);


#ifndef MIN
#define MIN(a,b)	((a) < (b)  ?  (a)  :  (b))
#endif

#ifndef MAX
#define MAX(a,b)	((a) > (b)  ?  (a)  :  (b))
#endif


#endif /* _SdifGlobals_ */
