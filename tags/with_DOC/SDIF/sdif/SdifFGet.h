/* $Id: SdifFGet.h,v 2.1 1998-12-21 18:27:06 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifFGet.h
 *
 * F : SdifFileT*, Get : ascii frames reading,
 * common to read a sdif file and a pseudo-sdif text file
 * 'verbose' allows to choise if the file is a sdif file or a pseudo-sdif text file
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.0  1998/11/29  11:41:29  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.2  1998/11/10  15:31:40  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifFGet_
#define _SdifFGet_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include <stdio.h>

int SdifFGetSignature   (SdifFileT *SdifF, size_t *NbCharRead);

int    SdifFGetOneNameValue    (SdifFileT *SdifF, int Verbose, size_t *NbCharRead);
size_t SdifFGetNameValueCurrHT (SdifFileT *SdifF, int Verbose);
size_t SdifFGetOneMatrixType   (SdifFileT *SdifF, int Verbose);
int    SdifFGetOneComponent    (SdifFileT *SdifF, int Verbose,
				SdifSignature *MatrixSignature,
				char *ComponentName,           size_t *NbCharRead);
size_t SdifFGetOneFrameType    (SdifFileT *SdifF, int Verbose);
size_t SdifFGetAllType         (SdifFileT *SdifF, int Verbose);
int    SdifFGetOneStreamID     (SdifFileT *SdifF, int Verbose, size_t *NbBytesRead);
size_t SdifFGetAllStreamID     (SdifFileT *SdifF, int Verbose);

#endif /* _SdifFGet_ */