/* $Id: SdifError.c,v 3.8 2001-05-02 09:34:41 tisseran Exp $
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
 * sdifError.c
 *
 * Fatal or program error management
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.7  2000/11/15 14:53:25  lefevre
 * no message
 *
 * Revision 3.6  2000/10/27  20:03:25  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.5.2.2  2000/08/21  21:35:01  tisseran
 * *** empty log message ***
 *
 * Revision 3.5.2.1  2000/08/21  14:04:04  tisseran
 * *** empty log message ***
 *
 * Revision 3.4  2000/05/12  14:41:43  schwarz
 * On behalf of Adrien, synchronisation with Mac sources, with some slight
 * changes because of cross-platform issues:
 * - Mac only stuff: XpSetFileAttribute XpFileSize
 * - Cross platform wrapper: XpGetenv XpExit
 * - Dangerous: strings.h (and thus bzero, bcopy) is not ANSI and thus doesn't
 *   exist on Mac.  Use string.h and memset, memcpy.
 *
 * Revision 3.3  2000/03/01  11:17:33  schwarz
 * Configurable exit function on error.
 *
 * Revision 3.2  1999/09/28  13:08:50  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:56:35  virolle
 * SdifStdErr add
 *
 * Revision 2.5  1999/02/28  12:16:37  virolle
 * memory report
 *
 * Revision 2.4  1999/01/23  15:55:38  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.3  1999/01/23  13:57:20  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.2  1998/12/21  18:27:03  schwarz
 * Inserted copyright message.
 *
 * Revision 2.1  1998/12/09  15:05:24  virolle
 * no segmentation fault when opening failed.
 *
 * Revision 2.0  1998/11/29  11:41:25  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/04/24  12:40:31  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 *
 */


#include <preincluded.h>
#include "XpGuiCalls.h"

#include "SdifError.h"
#include <stdlib.h>

static void SdifExit (void);

SdifExitFuncT gSdifExitFunc = SdifExit;
char *SdifErrorFile;
int SdifErrorLine;
FILE* SdifStdErr = NULL;


static void 
SdifExit (void)
{
    XpExit(1);
}


void
SdifSetExitFunc (SdifExitFuncT func)
{
    gSdifExitFunc = func;
}


void
SdifErrorWarning(SdifErrorEnum Error, const void *ErrorMess)
{
  int exitit = 0;

  fprintf(SdifStdErr, "*Sdif* Error (%s, %d)\n  ", SdifErrorFile, SdifErrorLine);

  switch(Error)
    {
    case  eFalse :
      fprintf(SdifStdErr, "False : '%s'\n");
      break;
    case  eTrue :
      fprintf(SdifStdErr, "True\n");
      break;
    case  eFreeNull :
      fprintf(SdifStdErr, "Attempt to free a NULL pointer : '%s'\n", ErrorMess);
      fflush(SdifStdErr);
      exitit = 1;
      break;
    case  eAllocFail :
      fprintf(SdifStdErr, "Attempt to allocate memory : '%s'\n", ErrorMess);
      fflush(SdifStdErr);
      exitit = 1;
      break;
    case  eInvalidPreType:
      fprintf(SdifStdErr, "Invalid Predefined Type : %s\n", ErrorMess);
      break;
    case eArrayPosition :
      fprintf(SdifStdErr, "Attempt to access to a non-existing square in an array : '%s'\n", ErrorMess);
      fflush(SdifStdErr);
      exitit = 1;
      break;
    case  eEof :
      fprintf(SdifStdErr, "End of file : %s\n", ErrorMess);
      fflush(SdifStdErr);
      exitit = 1;
      break;
    case  eFileNotFound :
      fprintf(SdifStdErr, "File Not Found or no Authorizations: \"%s\"\n",ErrorMess);
      fflush(SdifStdErr);
      break;            
    case  eAffectationOrder :
      fprintf(SdifStdErr, "Affectation must be in order : '%s'\n", ErrorMess);
      break;
    case  eNoModifErr :
      fprintf(SdifStdErr, "Type has been defined yet: '%s'\n", ErrorMess);
      break;      
    case  eNotInDataTypeUnion :
      fprintf(SdifStdErr, "Type of data Not in DataTypeUnion  : '%s'\n", ErrorMess);
      fflush(SdifStdErr);
      exitit = 1;
      break;      
    case  eNotFound :
      fprintf(SdifStdErr, "Not Find : '%s'\n", ErrorMess);
      break;      
    case  eExistYet :
      fprintf(SdifStdErr, "Object Exists Yet : '%s'\n", ErrorMess);
      break;      
    case eWordCut :
      fprintf(SdifStdErr, "Word cut read : '%s'\n", ErrorMess);
      break;
    case  eTokenLength :
      fprintf(SdifStdErr, "Token too long : '%s'\n", ErrorMess);
      fflush(SdifStdErr);
      exitit = 1;
      break;
    default :
      fprintf(SdifStdErr, "Warning unknown", ErrorMess);
      break;
    }

    if (exitit)
        (*gSdifExitFunc) ();
}
