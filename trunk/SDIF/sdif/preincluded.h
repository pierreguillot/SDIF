/******************************************************************************

  $Id: preincluded.h,v 3.1 2003-11-06 16:19:23 tisseran Exp $

  preincluded.h should be #included at the beginning of every .c file.
  It contains information, macro definitions and conditionals that are
  used throughout the whole project.
  
  You should not add project-specific declarations to preincluded.h.
  Instead, these should be in project_preinclude.h, which is to be
  provided by each project. 

  Currently, preincluded.h is used both in UDI and SVP.
  
  Alberto Ricci, 19990212 <- I'm Y2K!

  $Log: not supported by cvs2svn $
  Revision 1.8  2003/07/21 16:46:59  roebel
  Reuse FOUR_CHAR_CODE for file markers to keep files portable.
  FOUR_CHAR_CODE defined for linux.

  Revision 1.7  2001/12/05 13:21:49  tisseran
  Fix problem with UniversalEnvVar.h which define for all arch an enum only
  used on Macintosh.
  Prevent including host_architecture when using autoconf stuff (preincluded.h)

  Revision 1.6  2000/12/06 13:42:34  lefevre
  Mix HostArchiteture and AutoConfigure mechanisms

 * Revision 1.5  2000/11/15  18:10:08  lefevre
 * no message
 *
 * Revision 1.4  2000/08/21  14:27:24  tisseran
 * Use host_configure.h with autoconf and automake.
 * With this file, you don't need anymore 	#ifdef ARCH ....
 * 					#endif
 *
 * Revision 1.3  1999/02/17  17:55:10  schwarz
 * Some more comments.
 *

******************************************************************************/

#ifndef __H_PREINCLUDED__
#define __H_PREINCLUDED__


#ifndef HAVE_CONFIG_H
/* Host Architecture Mechanism
   ---------------------------
*/
#include <host_architecture.h>


/* Macro HOST_FOUR_CHAR_CODE()
   ---------------------------
*/
#if defined(WORDS_BIGENDIAN)
#   define HOST_FOUR_CHAR_CODE(x)		(x)
#   define FOUR_CHAR_CODE(x)		(x)
#else
#   define HOST_FOUR_CHAR_CODE(x)	(((UInt32) ((x) & 0x000000FF)) << 24) \
				      | (((UInt32) ((x) & 0x0000FF00)) << 8)  \
				      | (((UInt32) ((x) & 0x00FF0000)) >> 8)  \
				      | (((UInt32) ((x) & 0xFF000000)) >> 24)
#   define FOUR_CHAR_CODE(x)		(x)
#endif /* defined(WORDS_BIGENDIAN) */



#else /* HAVE_CONFIG_H */

/* Auto Configure Mechanism
   ------------------------
*/
/* Yet defined in host_architecture.h, then undef those common macros...
*/
#undef STDC_HEADERS
#undef WORDS_BIGENDIAN
#undef SIZEOF_LONG
#undef HAVE_ERRNO_H

/* ... and redefined them in "config.h"
*/
#include "config.h"

/* HOST_ENDIAN_LITTLE/BIG defined from WORDS_BIGENDIAN
*/
#undef  HOST_ENDIAN_BIG
#undef  HOST_ENDIAN_LITTLE

#if defined(WORDS_BIGENDIAN)
#define HOST_ENDIAN_BIG		1
#else
#define HOST_ENDIAN_LITTLE	1
#endif /* defined(WORDS_BIGENDIAN) */

#define HOST_DIRECTORY_DIVIDER '/'
#define HOST_DIRECTORY_DIVIDER_STR "/"

#endif /* HAVE_CONFIG_H */


/* project_preinclude.h must be present in YOUR project
   ----------------------------------------------------
*/
#include "project_preinclude.h"

#endif /* __H_PREINCLUDED__ */
