/* $Id: SdifTimePosition.c,v 2.3 1999-01-23 15:56:03 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 *
 *  NOT INCLUDE IN THE SDIF PROJECT AT NOV 1997
 * author: Dominique Virolle 1997
 *
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.2  1999/01/23  13:57:49  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 */


#include "SdifTimePosition.h"


#include <stdlib.h>




SdifTimePositionT*
SdifCreateTimePosition(SdifFloat8 Time, SdiffPosT Position)
{
  SdifTimePositionT* NewTimePosition = NULL;
  
  NewTimePosition = (SdifTimePositionT*) malloc (sizeof(SdifTimePositionT));
  if (NewTimePosition)
    {
      NewTimePosition->Time = Time;
      NewTimePosition->Position = Position;

      return NewTimePosition ;
    }
  else
    {
      _SdifError(eAllocFail, "Create TimePosition");
      return NULL;
    }
}











void
SdifKillTimePosition(SdifTimePositionT* TimePosition)
{
  if (TimePosition)
    free (TimePosition);
  else
    _SdifError(eFreeNull, "Kill TimePosition");
}







SdifTimePositionLT*
SdifCreateTimePositionL(void)
{
  SdifTimePositionLT *NewTimePositionL = NULL;
  
  NewTimePositionL = (SdifTimePositionLT*) malloc (sizeof(SdifTimePositionLT));
  if (NewTimePositionL)
    {
      NewTimePositionL->TimePosList = SdifCreateList(SdifKillTimePosition);
      return NewTimePositionL;
    }
  else
    {
      _SdifError(eAllocFail, "TimePositionL allocation");
      return NULL;
    }
}






void
SdifKillTimePositionL(SdifTimePositionLT *TimePositionL)
{
  if (TimePositionL)
    {
      SdifKillList(TimePositionL->TimePosList);
      free(TimePositionL);
    }
  else
    _SdifError(eFreeNull, "TimePositionL free");
}









SdifTimePositionLT*
SdifTimePositionLPutTail(SdifTimePositionLT* TimePositionL, SdifFloat8 Time, SdiffPosT Position)
{
    SdifTimePositionT* NewTP;

    NewTP = SdifCreateTimePosition(Time, Position);
    SdifListPutTail(TimePositionL->TimePosList, NewTP);
    return TimePositionL;
}


SdifTimePositionT*
SdifTimePositionLGetTail(SdifTimePositionLT* TimePositionL)
{
    return (SdifTimePositionT*) SdifListGetTail(TimePositionL->TimePosList);
}
