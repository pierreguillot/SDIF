/* $Id: SdifTimePosition.h,v 2.1 1998-12-21 18:27:45 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifTimePosition.h
 *
 *  NOT INCLUDE IN THE SDIF PROJECT AT NOV 1997
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.0  1998/11/29  11:42:12  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/11/10  15:31:57  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifTimePosition_
#define _SdifTimePosition_



#include "SdifGlobals.h"

#include <stdio.h>

typedef struct SdifTimePositionS SdifTimePositionT;

struct SdifTimePositionS
{
  SdifFloat8 Time;
  SdiffPosT     Position;
} ;

SdifTimePositionT* SdifCreateTimePosition(SdifFloat8 Time, SdiffPosT Position);
void               SdifKillTimePosition(SdifTimePositionT* TimePosition);





typedef struct SdifTimePositionNS SdifTimePositionNT;

struct SdifTimePositionNS
{
  SdifTimePositionNT *Next;
  SdifTimePositionT  *TP;
};

SdifTimePositionNT* SdifCreateTimePositionN(SdifTimePositionNT* Next, SdifTimePositionT* TimePosition);
SdifTimePositionNT* SdifKillTimePositionN  (SdifTimePositionNT* TPNode);



typedef struct SdifTimePositionLS SdifTimePositionLT;

struct SdifTimePositionLS
{
  SdifTimePositionNT* Head;
  SdifTimePositionNT* Tail;
  SdifTimePositionNT* Curr;
  SdifUInt4 NbTimePosition;
} ;

SdifTimePositionLT* SdifCreateTimePositionL(void);
void                SdifKillTimePositionL  (SdifTimePositionLT *TimePositionL);
SdifTimePositionLT* SdifTimePositionLPut   (SdifTimePositionLT* TimePositionL,
						   SdifFloat8 Time, SdiffPosT Position);
SdifTimePositionT*  SdifTimePositionLGet   (SdifTimePositionLT* TimePositionL, SdifFloat8 Time);

#endif /* _SdifTimePosition_ */
