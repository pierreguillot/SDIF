/* SdifStreamID.h
 *
 *
 *
 *
 */


#ifndef _SdifStreamID_
#define _SdifStreamID_

#include "SdifGlobals.h"

#include <stdio.h>

typedef struct SdifStreamIDT
{
  SdifUInt4     NumID;
  char *Source;
  char *TreeWay; /* for the moment or to be general*/
} SdifStreamIDType;

extern SdifStreamIDType*
SdifCreateStreamID(SdifUInt4 NumID, char *Source, char *TreeWay);

extern void
SdifKillStreamID(SdifStreamIDType *StreamID);

extern int
SdifFPrintOneStreamID(FILE *fw, SdifStreamIDType *StreamID);

extern int
SdifFWriteOneStreamID(SdifStreamIDType *StreamID, FILE *fw);

extern int
SdifFScanOneStreamID(FILE *fr, int *NbBytesRead);

#endif /* _SdifStreamID_ */
