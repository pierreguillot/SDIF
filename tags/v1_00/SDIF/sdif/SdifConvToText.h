/* SdifConvToText.h
 *
 *
 *
 *
 */

#ifndef _SdifConvToText_
#define _SdifConvToText_

#include "SdifGlobals.h"

#include "SdifMatrix.h"

#include <stdio.h>

extern int
SdifFConvRowsFloat4ToText(SdifMatrixHeaderType *MtrxH, FILE *fsdif, FILE *ftext);

extern int
SdifFConvRowsFloat8ToText(SdifMatrixHeaderType *MtrxH, FILE *fsdif, FILE *ftext);

extern int
SdifFConvFrameToText(char *Name, FILE *fsdif, FILE *ftext);

extern int
SdifFConvFramesChunkToText(FILE *fsdif, FILE *ftext);

extern int
SdifFConvChunkToText(char *Name, FILE *fsdif, FILE *ftext);

extern SdifUInt8
SdifFConvToText(FILE *fsdif, FILE *ftext);

extern SdifUInt8
SdifConvToText(char *NameFSdif, char *NameFText);

#endif /* _SdifConvToText_ */
