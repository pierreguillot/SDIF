/* $Id: SdifString.h,v 3.3 2000-11-21 14:51:51 schwarz Exp $
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
 * String management
 *
 * author : Patrice Tisserand 2000
 *
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.2  2000/10/27 20:03:45  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.1.2.1  2000/08/21  21:35:53  tisseran
 * *** empty log message ***
 *
 * Revision 3.1  2000/07/18  15:08:41  tisseran
 * This release implements the New SDIF Specification (june 1999):
 * - Name Values Table are written in a 1NVT frame which contains a 1NVT matrix
 * - Frame and matrix type declaration are written in a 1TYP frame which contains a 1TYP matrix.
 * - Stream ID are written in a 1IDS frame which contains a 1IDS matrix.
 *
 * Read function accept the previous version of the specification (read a text frame without matrix) to be compatible with older SDIF files.
 *
 * SdifString.h and SdifString.c implements some string mangement (creation, destruction, append, test of end of string, getc, ungetc).
 *
 * WATCH OUT:
 *      We don't care about the old SDIF Specification (_SdifFormatVersion < 3)
 * To use _SdifFormatVersion < 3, get the previous release.
 *
 */

#ifndef _SdifString_
#define _SdifString_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <sdif.h>


#define _SdifStringGranule 128 /* Default memory size allocated for string */

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

