/* $Id: SdifCheck.h,v 3.2 2000-10-27 20:03:24 roebel Exp $
  
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
  
    For any information regarding this and other IRCAM software, please
    send email to:
                              sdif@ircam.fr

LIBRARY
    SdifCheck.h		2. May 2000		Diemo Schwarz

    File test functions.

LOG
    $Log: not supported by cvs2svn $
 * Revision 3.1.2.2  2000/08/21  21:34:55  tisseran
 * *** empty log message ***
 *
 * Revision 3.1.2.1  2000/08/21  14:04:01  tisseran
 * *** empty log message ***
 *
 * Revision 3.1  2000/05/04  14:59:34  schwarz
 * Added modules SdifCheck and SdifSignatureTab, containing functions
 * formerly in SdifFile.
 *
*/


#ifndef _SDIFCHECK_H
#define _SDIFCHECK_H 1

#include "SdifFileStruct.h"


/*DOC: 
  Test if file is an SDIF file.

  [] Returns:	0 if not an SDIF file (the first 4 chars are not "SDIF"),
		or file can not be opened, else 1.  

  Warning: This function doesn't work with stdio. */
int SdifCheckFileFormat (const char *name);


/*DOC: 
  Test if file contains frames of certain types.

  [in]  name	Filename + selection
	frames  Table of frame signatures to look for
  []	return	The first signature from frames found, or eEmptySignature if 
		no frames could be found (or if file is not SDIF).

  Warning: This function doesn't work with stdio. */
SdifSignature SdifCheckFileFramesTab   (const char		*name, 
					const SdifSignatureTabT *frames);

/*DOC: 
  Test if file contains frames of certain types.

  [in]  name	Filename + selection
	frames  Array of frame signatures to look for, terminated with 
		eEmptySignature.
  []	return	The index in frames of the first signature found, or -1
		if no frames could be found (or if file is not SDIF).

  Warning: This function doesn't work with stdio. */
int	      SdifCheckFileFramesIndex (const char	        *name, 
					const SdifSignature     *frames);

/*DOC: 
  Test if file contains frames of certain types.

  [in]  in	open SDIF file
	frames  Table of frame signatures to look for
  [out] index   If the int pointer index is not NULL, it will receive
		the index in frames of the first signature found, or -1
		if no frames could be found (or if file is not SDIF).
  []	return	The first signature from frames found, or eEmptySignature if 
		no frames could be found (or if file is not SDIF).

  Warning: This function doesn't work with stdio. */
SdifSignature SdifCheckNextFrame       (SdifFileT		*in, 
					const SdifSignatureTabT *frames,
					int			*index);

/*DOC: 
  TODO: Test if file is an SDIF file (only when opening for read or
  append) and open it.

  [Return] NULL if not an SDIF file (the first 4 chars are not "SDIF"),
  or file can not be opened.  */
SdifFileT*	   SdifFTryOpen			(const char *Name, SdifFileModeET Mode);

#endif /* _SdifCheck_ */
