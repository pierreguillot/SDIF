/* $Id: SdifTimePosition.h,v 1.3 1998-11-10 15:31:57 schwarz Exp $
 *
 * SdifTimePosition.h
 *
 *  NOT INCLUDE IN THE SDIF PROJECT AT NOV 1997
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
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
