/* $Id: host_configure.h,v 3.1 2002-09-20 15:18:25 schwarz Exp $
    
   host_configure.h uses autoconf to know system's capbilities
  
   $Log: not supported by cvs2svn $
*/
#ifndef _HOST_CONFIGURE_H
#define _HOST_CONFIGURE_H 1

#include "config.h"



/*===========================================================================*/
/* CPU */
/* We don't need anymore test on system definition */

/*===========================================================================*/
/* UNIX */

#if   defined(unix) || defined(__unix) || defined(__unix__)

#define HOST_OS_UNIX					1

/* sub-case Linux */
#if   defined(__linux__)
#define HOST_OS_LINUX					1
#endif

typedef unsigned char 					UInt8;
typedef signed char 					SInt8;
typedef unsigned short 					UInt16;
typedef signed short 					SInt16;
typedef unsigned int 					UInt32;
typedef signed int	 				SInt32;



#define	HOST_CALLBACK_API_C(_type, _name)	_type (*_name)

#endif /* defined(__unix) */

#if defined(WIN32) || defined(OS2) || defined(__DOS__)
#define HOST_DIRECTORY_DIVIDER '\\'
#define HOST_DIRECTORY_DIVIDER_STR "\\"
#else
#define	HOST_DIRECTORY_DIVIDER			'/'
#define	HOST_DIRECTORY_DIVIDER_STR		"/"
#endif /* WIN32 */


/*===========================================================================*/
#if defined(WORDS_BIGENDIAN) /* Define in config.h (./configure => config.h) */
#   define HOST_FOUR_CHAR_CODE(x)		(x)
#else
#   define HOST_FOUR_CHAR_CODE(x)	(((UInt32) ((x) & 0x000000FF)) << 24) \
				      | (((UInt32) ((x) & 0x0000FF00)) << 8)  \
				      | (((UInt32) ((x) & 0x00FF0000)) >> 8)  \
				      | (((UInt32) ((x) & 0xFF000000)) >> 24)
#endif

/*===========================================================================*/


#endif /* _HOST_CONFIGURE_H */

