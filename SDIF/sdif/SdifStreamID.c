/* $Id: SdifStreamID.c,v 2.4 1999-02-28 12:16:55 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * StreamID structures management
 *
 * author: Dominique Virolle 1997
 *
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.3  1999/01/23  15:56:00  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:47  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 *
 */



#include "SdifStreamID.h"

#include <stdlib.h>



SdifStreamIDT*
SdifCreateStreamID(SdifUInt4 NumID, char *Source, char *TreeWay)
{
  SdifStreamIDT *NewID = NULL;
  
  NewID =  SdifMalloc(SdifStreamIDT);
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
      SdifFree(StreamID);
    }
  else
    _SdifError(eFreeNull, "StreamID free");
}








SdifStreamIDTableT*
SdifCreateStreamIDTable(SdifUInt4 HashSize)
{
    SdifStreamIDTableT* NewSIDTable;

    NewSIDTable = SdifMalloc(SdifStreamIDTableT);
    if (NewSIDTable)
    {
        NewSIDTable->NumIDLink  = _SdifNoStreamID;
        NewSIDTable->Time       = _Sdif_MIN_DOUBLE_;
        NewSIDTable->SIDHT      = SdifCreateHashTable(HashSize, eHashInt4, SdifKillStreamID);;
        return NewSIDTable;
    }
    else
    {
      _SdifError(eAllocFail, "StreamIDTable allocation");
      return NULL;
    }
}





void
SdifKillStreamIDTable (SdifStreamIDTableT* SIDTable)
{
    if (SIDTable)
    {
      SdifKillHashTable(SIDTable->SIDHT);
      SdifFree(SIDTable);
    }
    else
        _SdifError(eFreeNull, "StreamIDTable free");
}



SdifStreamIDT*
SdifStreamIDTablePutSID(SdifStreamIDTableT* SIDTable, SdifUInt4 NumID, char *Source, char *TreeWay)
{
    SdifStreamIDT* StreamID;

    StreamID = SdifCreateStreamID(NumID, Source, TreeWay);
    if (StreamID)
        SdifHashTablePut(SIDTable->SIDHT, &(StreamID->NumID), 1, StreamID);

    return StreamID;
}




SdifStreamIDT*
SdifStreamIDTableGetSID(SdifStreamIDTableT* SIDTable, SdifUInt4 NumID)
{
    return SdifHashTableSearch(SIDTable->SIDHT, &(NumID), 1);
}



SdifUInt4
SdifStreamIDTableGetNbData(SdifStreamIDTableT* SIDTable)
{
    return SIDTable->SIDHT->NbOfData;
}



