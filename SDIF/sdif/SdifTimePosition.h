/* SdifTimePosition.h
 *
 *  NOT INCLUDE IN THE SDIF PROJECT AT NOV 1997
 * author: Dominique Virolle 1997
 *
 *
 */


#ifndef _SdifTimePosition_
#define _SdifTimePosition_



#include "SdifGlobals.h"

#include <stdio.h>

typedef struct SdifTimePositionS
{
  SdifFloat8 Time;
  SdifFPosT     Position;
} SdifTimePositionT;

extern SdifTimePositionT* SdifCreateTimePosition(SdifFloat8 Time, SdifFPosT Position);
extern void               SdifKillTimePosition(SdifTimePositionT* TimePosition);


typedef struct SdifTimePositionNS SdifTimePositionNT;

struct SdifTimePositionNS
{
  SdifTimePositionNT *Next;
  SdifTimePositionT  *TP;
};

extern SdifTimePositionNT* SdifCreateTimePositionN(SdifTimePositionNT* Next, SdifTimePositionT* TimePosition);
extern SdifTimePositionNT* SdifKillTimePositionN  (SdifTimePositionNT* TPNode);



typedef struct SdifTimePositionLS
{
  SdifTimePositionNT* Head;
  SdifTimePositionNT* Tail;
  SdifTimePositionNT* Curr;
  SdifUInt4 NbTimePosition;
} SdifTimePositionLT;

extern SdifTimePositionLT* SdifCreateTimePositionL(void);
extern void                SdifKillTimePositionL  (SdifTimePositionLT *TimePositionL);
extern SdifTimePositionLT* SdifTimePositionLPut   (SdifTimePositionLT* TimePositionL,
						   SdifFloat8 Time, SdifFPosT Position);
extern SdifTimePositionT*  SdifTimePositionLGet   (SdifTimePositionLT* TimePositionL, SdifFloat8 Time);

#endif /* _SdifTimePosition_ */
