/* SdifFGet.h
 *
 * F : SdifFileT*, Get : ascii frames reading,
 * common to read a sdif file and a pseudo-sdif text file
 * 'verbose' allows to choise if the file is a sdif file or a pseudo-sdif text file
 *
 *
 * author: Dominique Virolle 1997
 *
 *
 */


#ifndef _SdifFGet_
#define _SdifFGet_

#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include <stdio.h>

extern int SdifFGetSignature   (SdifFileT *SdifF, size_t *NbCharRead);

int    SdifFGetOneNameValue    (SdifFileT *SdifF, int Verbose, size_t *NbCharRead);
size_t SdifFGetNameValueCurrHT (SdifFileT *SdifF, int Verbose);
size_t SdifFGetOneMatrixType   (SdifFileT *SdifF, int Verbose);
int    SdifFGetOneComponent    (SdifFileT *SdifF, int Verbose,
				SdifSignature *MatrixSignature,
				char *ComponentName,           size_t *NbCharRead);
size_t SdifFGetOneFrameType    (SdifFileT *SdifF, int Verbose);
size_t SdifFGetAllType         (SdifFileT *SdifF, int Verbose);
int    SdifFGetOneStreamID     (SdifFileT *SdifF, int Verbose, size_t *NbBytesRead);
size_t SdifFGetAllStreamID     (SdifFileT *SdifF, int Verbose);

#endif /* _SdifFGet_ */
