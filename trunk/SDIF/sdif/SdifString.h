/* $Id: SdifString.h,v 3.1 2000-07-18 15:08:41 tisseran Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * String management
 *
 * author : Patrice Tisserand 2000
 *
 *
 *
 * $Log: not supported by cvs2svn $
 */

#ifndef _SdifString_
#define _SdifString_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _SdifStringGranule 128 /* Default memory size allocated for string */

/* Structure declaration */
typedef struct SdifStringS SdifStringT;
struct SdifStringS
{
  char   *str; 
  size_t TotalSize; /* Memory size allocated for str */
  size_t SizeW; /* Memory size actually used */
  int    NbCharRead; /* Number of char read */
};


/* Function declaration */

/*DOC:
  Make a memory allocation for a SdifStringT structure.
  The size for the string is defined in SdifString.h; define _SdifStringGranule 128.
*/
SdifStringT * SdifStringNew(void);


/*DOC:
  Free memory allocated for SdifString.
*/
void SdifStringFree(SdifStringT * SdifString);


/*DOC:
  Append a string to another one.
  Manage memory reallocation.
  Return a boolean for the succes of the function's call.
*/
int SdifStringAppend(SdifStringT * SdifString ,char *strToAppend);


/*DOC:
  Read the current char (= fgetc).
*/
int SdifStringGetC(SdifStringT * SdifString);


/*DOC:
  Equivalent of ungetc
*/
int SdifStringUngetC(SdifStringT * SdifString);


/*DOC:
  Test the end of the string (= feof)
*/
int SdifStringIsEOS(SdifStringT *SdifString);

#endif /* _SdifString_ */

