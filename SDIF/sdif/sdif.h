/* $Id: sdif.h,v 3.3 2000-03-01 11:20:20 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * sdif.h
 *
 * Sound Description Interchange Format
 *
 * sdif library for management.
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.2  1999/08/31  10:02:58  schwarz
 * Added module SdifSelect which parses an access specification to a
 * chosen part of SDIF data.  Can be added to a file name.
 *
 * Revision 3.1  1999/03/14  10:57:29  virolle
 * SdifStdErr add
 *
 * Revision 2.1  1998/12/21  18:27:46  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:42:14  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/11/10  15:31:58  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */

#ifndef _sdif_
#define _sdif_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "SdifHash.h"
#include "SdifError.h"
#include "SdifGlobals.h"
#include "SdifRWLowLevel.h"

#include "SdifNameValue.h"
#include "SdifStreamID.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"

#include "SdifMatrix.h"
#include "SdifFrame.h"

#include "SdifTimePosition.h"
#include "SdifFileStruct.h"
#include "SdifTest.h"
#include "SdifFile.h"

#include "SdifPrint.h"

#include "SdifFGet.h"
#include "SdifFRead.h"
#include "SdifFScan.h"

#include "SdifFPut.h"
#include "SdifFPrint.h"
#include "SdifFWrite.h"
#include "SdifHighLevel.h"

#include "SdifTextConv.h"
#include "SdifConvToText.h"

#include "SdifPreTypes.h"
#include "SdifSelect.h"

#ifdef __cplusplus
}
#endif

#endif /* _sdif_ */
