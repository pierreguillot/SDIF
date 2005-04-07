/* $Id: SdifCheck.h,v 3.5 2005-04-07 14:10:32 schwarz Exp $
  
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
  
    For any information regarding this and other IRCAM software, please
    send email to:
                              sdif@ircam.fr

LIBRARY
    SdifCheck.h		2. May 2000		Diemo Schwarz

    File test functions.

LOG
    $Log: not supported by cvs2svn $
    Revision 3.4  2001/05/02 09:34:40  tisseran
    Change License from GNU Public License to GNU Lesser Public License.

    Revision 3.3  2000/11/21 14:51:47  schwarz
    - sdif.h is now included by all sdif/Sdif*.c files.
    - Removed all public typedefs, enums, structs, and defines from the
      individual sdif/Sdif*.h files, because they were duplicated in sdif.h.
    - Todo: Do the same for the function prototypes, decide which types and
      prototypes really need to be exported.
    - Removed SdifFileStruct.h.
    - Preliminary new version of SdiffGetPos, SdiffSetPos.  They used the
      type fpos_t, which is no longer a long on RedHat 7 Linux.

    Revision 3.2  2000/10/27 20:03:24  roebel
    autoconf merged back to main trunk

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



#endif /* _SdifCheck_ */
