/* SdifStreamID.c
 *
 * StreamID structures management
 *
 * author: Dominique Virolle 1997
 *
 */



#include "SdifStreamID.h"

#include <stdlib.h>



SdifStreamIDT*
SdifCreateStreamID(SdifUInt4 NumID, char *Source, char *TreeWay)
{
  SdifStreamIDT *NewID = NULL;
  
  NewID =  (SdifStreamIDT*) malloc (sizeof(SdifStreamIDT));
  if (NewID)
    {
      NewID->NumID = NumID;
      NewID->Source  = SdifCreateStrNCpy(Source, SdifStrLen(Source)+1);
      NewID->TreeWay = SdifCreateStrNCpy(TreeWay, SdifStrLen(TreeWay)+1);
      
      return NewID;
    }
  else
    {
      _SdifError(eAllocFail, "StreamID allocation");
      return NULL;
    }
}






void
SdifKillStreamID(SdifStreamIDT *StreamID)
{
  if (StreamID)
    {
      SdifKillStr(StreamID->TreeWay);
      SdifKillStr(StreamID->Source);
      free(StreamID);
    }
  else
    _SdifError(eFreeNull, "StreamID free");
}
