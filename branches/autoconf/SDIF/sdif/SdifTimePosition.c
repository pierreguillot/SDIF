/* $Id: SdifTimePosition.c,v 3.3 2000-05-15 16:22:36 schwarz Exp $
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
 * Revision 3.2  1999/09/28  13:09:17  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:57:27  virolle
 * SdifStdErr add
 *
 * Revision 2.4  1999/02/28  12:16:57  virolle
 * memory report
 *
 * Revision 2.3  1999/01/23  15:56:03  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:49  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 */


#include <preincluded.h>
#include "SdifTimePosition.h"


#include <stdlib.h>




SdifTimePositionT*
SdifCreateTimePosition(SdifFloat8 Time, SdiffPosT Position)
{
  SdifTimePositionT* NewTimePosition = NULL;
  
  NewTimePosition = SdifMalloc(SdifTimePositionT);
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
SdifKillTimePosition(void* TimePosition)
{
  if (TimePosition)
    SdifFree (TimePosition);
  else
    _SdifError(eFreeNull, "Kill TimePosition");
}







SdifTimePositionLT*
SdifCreateTimePositionL(void)
{
  SdifTimePositionLT *NewTimePositionL = NULL;
  
  NewTimePositionL = SdifMalloc(SdifTimePositionLT);
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
      SdifFree(TimePositionL);
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
