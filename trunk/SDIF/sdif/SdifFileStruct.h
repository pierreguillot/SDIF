/* $Id: SdifFileStruct.h,v 1.3 1998-11-10 15:31:45 schwarz Exp $
 *
 * SdifFileStruct.h
 *
 * Upper level structure
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */

#ifndef _SdifFileStruct_
#define _SdifFileStruct_

#include "SdifGlobals.h"

#include "SdifNameValue.h"
#include "SdifHash.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifTimePosition.h"
#include "SdifMatrix.h"
#include "SdifFrame.h"
#include <stdio.h>


typedef enum SdifFileModeE
{
  eUnknownFileMode,
  eWriteFile,
  eReadFile,
  ePredefinedTypes
} SdifFileModeET ;



enum SdifPassE
{
  eNotPass,
  eReadPass,
  eWritePass
};
  

typedef struct SdifFileS SdifFileT;

struct SdifFileS
{
  char *Name;                           /* Name of the file, can be "stdin, stdout, stderr */
  SdifFileModeET Mode;                  /* eWriteFile or eReadFile or ePredefinedTypes */

  SdifNameValuesLT   *NameValues;       /* DataBase of Names Values */
  SdifHashTableT     *MatrixTypesTable; /* DataBase of Matrix Types */
  SdifHashTableT     *FrameTypesTable;  /* DataBase of Frame Types */
  SdifHashTableT     *StreamIDsTable;   /* DataBase of Stream IDs */
  SdifTimePositionLT *TimePositions;    /* List of (Time, Position in the file) */

  FILE *Stream;                         /* Stream to read or to write */

  
  SdifSignature      CurrSignature;
  SdifFrameHeaderT   *CurrFramH;        /* Current Frame Header can be NULL */
  SdifMatrixHeaderT  *CurrMtrxH;        /* Current Matrix Header can be NULL */

  SdifOneRowT        *CurrOneRow;
  /* Currenet OneRow allocated memory in function
   * of _SdifGranule, use SdifReInitOneRow(SdifOneRowT *OneRow, SdifDataTypeET DataType, SdifUInt4 NbData)
   * to assure NbData (=NbColumns) objects memory allocated
   */

  size_t  FileSize;
  size_t  ChunkSize;

  SdiffPosT  CurrFramPos;
  SdiffPosT  StartChunkPos;
  SdiffPosT  Pos;
  
  SdifUInt2  TypeDefPass;
  SdifUInt2  StreamIDPass;

  char *TextStreamName;                 /* Name of the text file corresponding to the sdif file */
  FILE *TextStream;                     /* Stream text */


  unsigned int NbOfWarning;

};


#endif /* _SdifFileStruct_ */
