/* SdifRWHighLevel.h
 *
 *
 *
 */

#ifndef _SdifRWHighLevel_
#define _SdifRWHighLevel_

#include "SdifGlobals.h"

#include "SdifMatrixType.h"
#include "SdifFrameType.h"

#include <stdio.h>

extern void
SdifUpdateFileSize(SdifUInt8 FileSize, FILE *fw);

extern void
SdifUpdateChunkSize(SdifInt4 Size, FILE *fw);

extern int
SdifLoadPredefinedTypes(char* TypesFileName);


/* FWrite */
extern int
SdifFWriteGeneralHeader(SdifUInt8 FileSize, FILE *fw);

extern int
SdifFWriteNameValueCurrHT(FILE *fw);

extern int
SdifFWriteAllType(SdifPredefinedEnum Predefined, FILE *fw);

extern int
SdifFWriteAllStreamID(FILE *fw);

extern int
SdifFWriteAllASCIIChunks(FILE *fw);

extern int
SdifFWriteFramesChunkHeader(SdifInt4 Size, FILE *fw);



/* FRead */
extern int 
SdifFReadGeneralHeader(SdifUInt8 *oFileSize, FILE *fr);

extern int
SdifFReadNameValueCurrHT(FILE *fr);

extern int
SdifFReadAllType(SdifPredefinedEnum Predefined, FILE *fr);

extern int
SdifFReadAllStreamID(FILE *fr);

extern int
SdifFReadAllASCIIChunks(FILE *fr);

extern int
SdifFReadFramesChunkSize(SdifInt4 *pSize, FILE *fr);


/* FPrint */
extern int
SdifFPrintNameValueCurrHT(FILE *fw);

extern int
SdifFPrintAllType(FILE *fw, SdifPredefinedEnum Predefined);

extern int
SdifFPrintAllStreamID(FILE *fw);


/* FScan */
extern int
SdifFScanAllType(SdifPredefinedEnum Predefined, FILE *fr);

extern int
SdifFScanAllStreamID(FILE *fr);

extern int
SdifFScanNameValueCurrHT(FILE *fr);

#endif /* _SdifRWType_ */
