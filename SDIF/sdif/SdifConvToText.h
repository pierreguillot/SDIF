/* $Id: SdifConvToText.h,v 3.2 1999-08-25 18:32:33 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifConvToText.h
 *
 * functions to convert an sdif file into a pseudo-sdif text file.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.1  1999/03/14  10:56:32  virolle
 * SdifStdErr add
 *
 * Revision 2.1  1998/12/21  18:27:00  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:22  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/11/10  15:31:39  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */

#ifndef _SdifConvToText_
#define _SdifConvToText_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"



size_t SdifFConvToTextMatrixData     (SdifFileT *SdifF);
size_t SdifFConvToTextMatrixHeader   (SdifFileT *SdifF);
size_t SdifFConvToTextMatrix         (SdifFileT *SdifF);
size_t SdifFConvToTextFrameData      (SdifFileT *SdifF);
size_t SdifFConvToTextFrameHeader    (SdifFileT *SdifF);
size_t SdifFConvToTextFrame          (SdifFileT *SdifF);
size_t SdifFConvToTextAllFrame       (SdifFileT *SdifF);
size_t SdifFConvToText               (SdifFileT *SdifF);


/*DOC: 
  Converti un fichier texte pseudo-SDIF de nom TextStreamName en un
  fichier SDIF binaire de non SdifF->Name. Le fichier doit avoir été
  ouvert en écriture (eWriteFile).  */
size_t SdifToText (SdifFileT *SdifF, char *TextStreamName);

#endif /* _SdifConvToText_ */
