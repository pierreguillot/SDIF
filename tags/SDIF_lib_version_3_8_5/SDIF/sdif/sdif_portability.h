/******************************************************************************
 *
 * $Id: sdif_portability.h,v 3.1 2003-11-07 22:00:48 roebel Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
 *
 * Desctiption of header
 *
 * sdif_portability ensures proper  compilation on systems that do not
 * provide the autoconf mechanism.
 *
 * LOG
 * $Log: not supported by cvs2svn $
 *
 *****************************************************************************/

#ifndef __H_SDIF_PORTABILITY__
#define __H_SDIF_PORTABILITY__

#include <sdif_version.h>


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

#endif /* __H_SDIF_PORTABILITY__ */
