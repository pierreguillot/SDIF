/* $Id: SdifPreTypes.h,v 3.3 2000-11-21 14:51:50 schwarz Exp $
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
 * SdifPreTypes.h
 *
 * Compiled Predefined Types. If the predefined types file isn't find or understood,
 * then this types are used. But in this case, some types can miss.
 *
 * author: Dominique Virolle 1997
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.2  2000/10/27 20:03:41  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.1.2.1  2000/08/21  21:35:41  tisseran
 * *** empty log message ***
 *
 * Revision 3.1  1999/03/14  10:57:15  virolle
 * SdifStdErr add
 *
 * Revision 2.2  1999/01/23  13:57:44  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:34  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:42:01  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.2  1998/11/10  15:31:53  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifPreTypes_
#define _SdifPreTypes_

#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifGlobals.h"


#define   M_1FQ0_Frequency  "Frequency"
#define   M_1FQ0_Mode       "Mode"
#define   M_1FQ0_Hit        "Hit"

#define   M_1FOF_Frequency  "Frequency"
#define   M_1FOF_Amplitude  "Amplitude"
#define   M_1FOF_BandWidth  "BandWidth"
#define   M_1FOF_Tex        "Tex"
#define   M_1FOF_DebAtt     "DebAtt"
#define   M_1FOF_Atten      "Atten"
#define   M_1FOF_Phase      "Phase"

#define   M_1CHA_Channel1   "Channel1"
#define   M_1CHA_Channel2   "Channel2"
#define   M_1CHA_Channel3   "Channel3"
#define   M_1CHA_Channel4   "Channel4"

#define   M_1RES_Frequency  "Frequency"
#define   M_1RES_Amplitude  "Amplitude"
#define   M_1RES_BandWidth  "BandWidth"
#define   M_1RES_Saliance   "Saliance"
#define   M_1RES_Correction "Correction"

#define   M_1DIS_Distribution    "Distribution"
#define   M_1DIS_Amplitude  "Amplitude"

SdifFrameTypeT* CreateF_1FOB(void);
SdifFrameTypeT* CreateF_1REB(void);
SdifFrameTypeT* CreateF_1NOI(void);
void SdifCreatePredefinedTypes(SdifHashTableT *MatrixTypesHT,
                                      SdifHashTableT *FrameTypesHT);

#endif /* _SdifPreTypes_ */
