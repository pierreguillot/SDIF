/* $Id: SdifStreamID.h,v 1.4 1998-11-10 15:31:55 schwarz Exp $
 *
 * SdifStreamID.h
 *
 * StreamID structures management
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */


#ifndef _SdifStreamID_
#define _SdifStreamID_

#include "SdifGlobals.h"
#include "SdifHash.h"

typedef struct SdifStreamIDS SdifStreamIDT;
struct SdifStreamIDS
{
  SdifUInt4     NumID;
  char *Source;
  char *TreeWay; /* for the moment or to be general*/
} ;

SdifStreamIDT* SdifCreateStreamID(SdifUInt4 NumID, char *Source, char *TreeWay);
void           SdifKillStreamID(SdifStreamIDT *StreamID);

#endif /* _SdifStreamID_ */
