/* $Id: sdif.h,v 1.3 1998-11-10 15:31:58 schwarz Exp $
 *
 * sdif.h
 *
 * Sound Description Interchange Format
 *
 * sdif library for management.
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */

#ifndef _sdif_
#define _sdif_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "SdifHash.h"
#include "SdifError.h"
#include "SdifGlobals.h"
#include "SdifRWLowLevel.h"

#include "SdifNameValue.h"
#include "SdifStreamID.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"

#include "SdifMatrix.h"
#include "SdifFrame.h"

#include "SdifTimePosition.h"
#include "SdifFileStruct.h"
#include "SdifTest.h"
#include "SdifFile.h"

#include "SdifPrint.h"

#include "SdifFGet.h"
#include "SdifFRead.h"
#include "SdifFScan.h"

#include "SdifFPut.h"
#include "SdifFPrint.h"
#include "SdifFWrite.h"

#include "SdifTextConv.h"
#include "SdifConvToText.h"

#include "SdifPreTypes.h"

#ifdef __cplusplus
}
#endif

#endif /* _sdif_ */
