/* SdifStreamID.h
 *
 * StreamID structures management
 *
 *
 * author: Dominique Virolle 1997
 *
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

extern SdifStreamIDT* SdifCreateStreamID(SdifUInt4 NumID, char *Source, char *TreeWay);
extern void           SdifKillStreamID(SdifStreamIDT *StreamID);

#endif /* _SdifStreamID_ */
