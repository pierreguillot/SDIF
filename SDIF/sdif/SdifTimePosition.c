/* SdifTimePosition.c
 *
 *
 *  NOT INCLUDE IN THE SDIF PROJECT AT NOV 1997
 * author: Dominique Virolle 1997
 *
 */


#include "SdifTimePosition.h"


#include <stdlib.h>




SdifTimePositionT*
SdifCreateTimePosition(SdifFloat8 Time, SdiffPosT Position)
{
  SdifTimePositionT* NewTimePosition;
  
  if (NewTimePosition = (SdifTimePositionT*) malloc (sizeof(SdifTimePositionT)))
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








SdifTimePositionNT*
SdifCreateTimePositionN(SdifTimePositionNT* Next, SdifTimePositionT* TimePosition)
{
  SdifTimePositionNT* NewTimePositionNode;
  
  if (NewTimePositionNode = (SdifTimePositionNT*) malloc(sizeof(SdifTimePositionNT)))
    {
      NewTimePositionNode->Next = Next;
      NewTimePositionNode->TP = TimePosition;
      
      return NewTimePositionNode;
    }
  else
    {
      _SdifError(eAllocFail, "TimePositionNode allocation");
      return NULL;
    }
}







SdifTimePositionNT*
SdifKillTimePositionN(SdifTimePositionNT* TPNode)
{
  SdifTimePositionNT *Next;
  
  if (TPNode)
    {
      Next = TPNode->Next;
      SdifKillTimePosition(TPNode->TP);
      free(TPNode);
      
      return Next;
    }
  else
    {
      _SdifError(eFreeNull, "TimePositionNode free");
      return NULL;
    }
}








SdifTimePositionLT*
SdifCreateTimePositionL(void)
{
  SdifTimePositionLT *NewTimePositionL;
  
  if (NewTimePositionL = (SdifTimePositionLT*) malloc (sizeof(SdifTimePositionLT)))
    {
      NewTimePositionL->Head = NULL;
      NewTimePositionL->Tail = NULL;
      NewTimePositionL->NbTimePosition = 0;
      NewTimePositionL->Curr = NULL;
      
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
      while (TimePositionL->Head)
	TimePositionL->Head= SdifKillTimePositionN(TimePositionL->Head);
      
      free(TimePositionL);
    }
  else
    _SdifError(eFreeNull, "TimePositionL free");
}









SdifTimePositionLT*
SdifTimePositionLPut(SdifTimePositionLT* TimePositionL, SdifFloat8 Time, SdiffPosT Position)
{
  SdifTimePositionNT
    *iNode,
    *StartNode,
    *StopNode;
  
  if (! TimePositionL->Head)                       /* case empty List */
    {
      TimePositionL->Head = SdifCreateTimePositionN (TimePositionL->Head,
						     SdifCreateTimePosition (Time,
									     Position));
      TimePositionL->Tail = TimePositionL->Head;
      TimePositionL->Curr = TimePositionL->Head;
      TimePositionL->NbTimePosition++;
      return TimePositionL;
    }
  else
    
  if (SdifFloat8Equ(TimePositionL->Head->TP->Time, Time))         /* case Head Replace */
    {
      TimePositionL->Head->TP->Position = Position;
      TimePositionL->Curr = TimePositionL->Head;
      return TimePositionL;
    }
  else
      
  if (TimePositionL->Head->TP->Time > Time)                       /* case Head */
    {
      TimePositionL->Head = SdifCreateTimePositionN (TimePositionL->Head,
						     SdifCreateTimePosition (Time,
									     Position));
      TimePositionL->Curr =  TimePositionL->Head;
      TimePositionL->NbTimePosition++;
      return TimePositionL;
    }
  else

  if (SdifFloat8Equ(TimePositionL->Tail->TP->Time, Time))         /* case Tail Replace */
    {
      TimePositionL->Tail->TP->Position = Position;
      TimePositionL->Curr = TimePositionL->Tail;
      return TimePositionL;
    }
  else

  if (TimePositionL->Tail->TP->Time < Time)                       /* case Tail */
    {
      TimePositionL->Tail->Next = SdifCreateTimePositionN (TimePositionL->Tail->Next,
							   SdifCreateTimePosition (Time,
										   Position));
      TimePositionL->Tail = TimePositionL->Tail->Next;
      TimePositionL->Curr = TimePositionL->Tail;
      TimePositionL->NbTimePosition++;
      return TimePositionL;
    }
  else

  if (SdifFloat8Equ(TimePositionL->Curr->TP->Time, Time))        /* case Curr Replace */
    {
      TimePositionL->Curr->TP->Position = Position;
      return TimePositionL;
    }

  else
    {
      if (TimePositionL->Curr->TP->Time > Time)
	{
	  StartNode = TimePositionL->Head;
	  StopNode = TimePositionL->Curr;
	}
      else
	{
	  StartNode = TimePositionL->Curr;
	  StopNode = TimePositionL->Tail;
	}
      
      for (iNode = StartNode; (iNode->Next)&&(iNode->Next!=StopNode); iNode=iNode->Next)
	{
	  if (iNode->Next->TP->Time > Time)
	    {
	      iNode->Next = SdifCreateTimePositionN (iNode->Next->Next,
						     SdifCreateTimePosition (Time,
									     Position));
	      TimePositionL->Curr = iNode->Next;
	      TimePositionL->NbTimePosition++;
	      break;
	    }
	  else
	    if (SdifFloat8Equ(iNode->Next->TP->Time, Time))
	      {
		iNode->Next->TP->Position = Position;
		break;
	      }
	}
      return TimePositionL;
    }
}









SdifTimePositionT*
SdifTimePositionLGet(SdifTimePositionLT* TimePositionL, SdifFloat8 Time)
{
  SdifTimePositionT
    *ResTP = NULL;
  SdifTimePositionNT
    *iNode,
    *StartNode,
    *StopNode;



  if (! TimePositionL->Head)                                /* case empty List */
    ResTP = NULL;
  else
  if (SdifFloat8Equ(TimePositionL->Head->TP->Time, Time))   /* case Head */
    {
      ResTP = TimePositionL->Head->TP;
      TimePositionL->Curr = TimePositionL->Head;
    }
  else
  if (SdifFloat8Equ(TimePositionL->Tail->TP->Time, Time))   /* case Tail */
    {
      ResTP = TimePositionL->Tail->TP;
      TimePositionL->Curr = TimePositionL->Tail;
    }
  else
  if (SdifFloat8Equ(TimePositionL->Tail->TP->Time, Time))   /* case Curr */
    {
      ResTP = TimePositionL->Tail->TP;
    }
  else
    {
      if (TimePositionL->Curr->TP->Time > Time)
	{
	  StartNode = TimePositionL->Head->Next;
	  StopNode = TimePositionL->Curr;
	}
      else
	{
	  StartNode = TimePositionL->Curr;
	  StopNode = TimePositionL->Tail;
	}
      
      for (iNode = StartNode; (iNode)&&(iNode!=StopNode); iNode=iNode->Next)
	{
	  if (SdifFloat8Equ(iNode->TP->Time, Time))
	    {
	      ResTP = iNode->TP;
	      TimePositionL->Curr = iNode;
	    }
	}
    }
  return ResTP;
}
