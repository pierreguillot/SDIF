/* $Id: SdifStreamID.h,v 2.0 1998-11-29 11:42:07 virolle Exp $
 *
 * SdifStreamID.h
 *
 * StreamID structures management
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.4  1998/11/10  15:31:55  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
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

SdifStreamIDT* SdifCreateStreamID(SdifUInt4 NumID, char *Source, char *TreeWay);
void           SdifKillStreamID(SdifStreamIDT *StreamID);

#endif /* _SdifStreamID_ */
