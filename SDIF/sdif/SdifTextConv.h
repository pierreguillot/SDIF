/* SdifTextConv.h
 *
 *
 *
 *
 *
 *
 */


#ifndef _SdifTextConv_
#define _SdifTextConv_

#include <stdio.h>
#include "SdifGlobals.h"
#include "SdifMatrix.h"

extern int
SdifFTextRowsConvFloat4(SdifMatrixHeaderType *MtrxH, FILE *ftext, FILE *fsdif);

extern int
SdifFTextRowsConvFloat8(SdifMatrixHeaderType *MtrxH, FILE *ftext, FILE *fsdif);

extern int
SdifFTextFrameConv(char *Name, FILE *ftext, FILE *fsdif);

extern int
SdifFTextFramesChunkConv(FILE *ftext, FILE *fsdif);

extern SdifUInt8
SdifFTextConv(FILE *ftext, FILE *fsdif);

extern SdifUInt8
SdifTextConv(char *NameFText, char *NameFSdif);

#endif   /* _SdifTextConv_ */
