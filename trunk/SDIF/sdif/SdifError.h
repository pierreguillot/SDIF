/* $Id: SdifError.h,v 3.4 2000-10-27 20:03:26 roebel Exp $
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
 * SdifError.h
 *
 * Fatal or program error management
 *
 * author: Dominique Virolle 1997
 * 
 * $Log: not supported by cvs2svn $
 * Revision 3.3.2.2  2000/08/21  21:35:02  tisseran
 * *** empty log message ***
 *
 * Revision 3.3.2.1  2000/08/21  14:04:05  tisseran
 * *** empty log message ***
 *
 * Revision 3.2  2000/03/01  11:17:34  schwarz
 * Configurable exit function on error.
 *
 * Revision 3.1  1999/03/14  10:56:36  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/02/28  12:16:38  virolle
 * memory report
 *
 * Revision 2.2  1999/01/23  13:57:21  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:03  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:26  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.4  1998/11/10  15:31:39  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 * Revision 1.3  1998/04/24  12:40:32  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 *
 */


#ifndef _SDIFERROR_H
#define _SDIFERROR_H

#include <stdio.h>

typedef enum SdifErrorE
{
  eFalse = 0,
  eTrue = 1,
  eFreeNull = 256,
  eAllocFail,
  eArrayPosition,
  eEof,
  eFileNotFound,
  eInvalidPreType,
  eAffectationOrder,
  eNoModifErr,
  eNotInDataTypeUnion,
  eNotFound,
  eExistYet,
  eWordCut,
  eTokenLength
} SdifErrorEnum;

typedef void (*SdifExitFuncT) (void);

extern SdifExitFuncT gSdifExitFunc;
extern char *SdifErrorFile;
extern int SdifErrorLine;
extern FILE* SdifStdErr;

void SdifSetExitFunc (SdifExitFuncT func);
void SdifErrorWarning(SdifErrorEnum Error, const void *ErrorMess);

#define _SdifError(error, mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, SdifErrorWarning((error), (mess)))


#define _Debug(mess) \
(SdifErrorFile = __FILE__, SdifErrorLine = __LINE__, \
fprintf(SdifStdErr, "*Sdif Debug* %s, %d:\n", SdifErrorFile, SdifErrorLine), \
fprintf(SdifStdErr, "%s\n",(mess)))

#define _SdifRemark(mess) \
fprintf(SdifStdErr, "*Sdif* %s\n", mess)

#endif /* _SdifError_ */
