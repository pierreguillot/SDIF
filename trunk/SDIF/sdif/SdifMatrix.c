/* SdifMatrix.c
 *
 *
 *
 */


#include "SdifMatrix.h"

#include "SdifRWLowLevel.h"

#include <stdlib.h>
#include <string.h>








SdifMatrixHeaderType*
SdifCreateMatrixHeader(char *MatrixName,
		       SdifUInt4 DataType,
		       SdifUInt4 NbRow,
		       SdifUInt4 NbCol)
{
  SdifMatrixHeaderType *NewMatrixHeader;
  
  if (NewMatrixHeader = (SdifMatrixHeaderType*) malloc (sizeof(SdifMatrixHeaderType)))
    {
      SdifStrNCpy(NewMatrixHeader->MatrixName, MatrixName, _SdifNameLen);
      NewMatrixHeader->DataType = (SdifDataTypeEnum) (int) DataType;
      NewMatrixHeader->NbRow = NbRow;
      NewMatrixHeader->NbCol = NbCol;
      
      return NewMatrixHeader;
    }
  else
    {
      _SdifError(eAllocFail, "MatrixHeader allocation");
      return NULL;
    }
}








SdifMatrixHeaderType*
SdifCreateMatrixHeaderEmpty(void)
{
  return SdifCreateMatrixHeader((char*)"\0\0\0\0", eFloat4, 0, 0);
}








void
SdifKillMatrixHeader(SdifMatrixHeaderType *MatrixHeader)
{
  if (MatrixHeader)
    free(MatrixHeader);
  else
    _SdifError(eFreeNull, "MatrixHeader free");
}












/*
SdifFWriteMatrixHeader(SdifMatrixHeaderType *MatrixHeader, FILE *fr)
{
  int NbBytesWrite = 0;
  
  NbBytesWrite += SdifFWrite4Char( MatrixHeader->MatrixName, fw);
  NbBytesWrite += sizeof(SdifUInt4) * SdifFWriteUInt4( &(MatrixHeader->DataType), 1, fr);
  NbBytesWrite += sizeof(SdifUInt4) * SdifFWriteUInt4( &(MatrixHeader->NbRow), 1, fr);
  NbBytesWrite += sizeof(SdifUInt4) * SdifFWriteUInt4( &(MatrixHeader->NbCol), 1, fr);
   
  return NbBytesWrite;
}
*/







int
SdifFWriteMatrixHeader(SdifMatrixHeaderType* MatrixHeader, FILE *fw)
{
  int NbBytesWrite = 0;
  SdifFloat4 FloatTab[3];

  FloatTab[0] = (SdifFloat4) MatrixHeader->DataType;
  FloatTab[1] = (SdifFloat4) MatrixHeader->NbRow;
  FloatTab[2] = (SdifFloat4) MatrixHeader->NbCol;

  NbBytesWrite += SdifFWrite4Char( MatrixHeader->MatrixName, fw);
  NbBytesWrite += sizeof(SdifFloat4) * SdifFWriteFloat4(FloatTab, 3, fw);

  return NbBytesWrite;
}








void
SdifFScanMatrixHeader(FILE *fr, char *MatrixName, SdifMatrixHeaderType* MatrixHeader)
{
  int NbBytesRead = 0;
  SdifUInt4 DataType;

  SdifFGetName((MatrixHeader->MatrixName), fr, &NbBytesRead);
  fscanf(fr, "%u", &(DataType));
  MatrixHeader->DataType = (SdifDataTypeEnum) (int) DataType;
  fscanf(fr, "%u", &(MatrixHeader->NbRow));
  fscanf(fr, "%u", &(MatrixHeader->NbCol));
}








int
SdifFPrintMatrixHeader(FILE *f, SdifMatrixHeaderType *MatrixHeader)
{
  int
    NbCharsWrite = 0;

  NbCharsWrite += fprintf(f, "  ");
  NbCharsWrite += SdifPrintName(f, MatrixHeader->MatrixName);
  NbCharsWrite += fprintf(f,
			  "\t%u\t%u\t%u\n",
			  MatrixHeader->DataType,
			  MatrixHeader->NbRow,
			  MatrixHeader->NbCol);
  return NbCharsWrite;
}







/*
int
SdifFReadMatrixHeader(SdifMatrixHeaderType *MatrixHeader, FILE *fr)
{
  int NbBytesRead = 0;
  
  SdifFGetName((MatrixHeader->MatrixName), fr, &NbBytesRead);
  NbBytesRead += sizeof(SdifUInt4) * SdifFReadUInt4( &((SdifUInt4) MatrixHeader->DataType), 1, fr);
  NbBytesRead += sizeof(SdifUInt4) * SdifFReadUInt4( &(MatrixHeader->NbRow), 1, fr);
  NbBytesRead += sizeof(SdifUInt4) * SdifFReadUInt4( &(MatrixHeader->NbCol), 1, fr);
   
  return NbBytesRead;
}
*/


int
SdifFReadMatrixHeader(SdifMatrixHeaderType *MatrixHeader, FILE *fr)
{
  int NbBytesRead = 0;
  SdifFloat4 FloatTab[3];

  SdifFGetName((MatrixHeader->MatrixName), fr, &NbBytesRead);
  NbBytesRead += sizeof(SdifFloat4) * SdifFReadFloat4( FloatTab, 3, fr);
  
  MatrixHeader->DataType  = (SdifDataTypeEnum) (int) (float) FloatTab[0];
  MatrixHeader->NbRow     = (SdifUInt4) FloatTab[1];
  MatrixHeader->NbCol     = (SdifUInt4) FloatTab[2];

  return NbBytesRead;
}






SdifOneRowType*
SdifCreateOneRow(SdifDataTypeEnum DataType, SdifUInt4  NbData)
{
  SdifOneRowType * NewOneRow;
  SdifUInt4 i;

  if (NbData <= 0)
    {
      _SdifError(eAllocFail, "NbData of OneRow <= 0. You must protect it at upper level.");
      return NULL;
    }

  if (NewOneRow = (SdifOneRowType*) malloc (sizeof(SdifOneRowType)))
    {
      NewOneRow->DataType = DataType;
      NewOneRow->NbData = NbData;
      switch (NewOneRow->DataType)
	{
	case eFloat4 :
	  if (NewOneRow->Data.F4 = (SdifFloat4*) malloc (sizeof(SdifFloat4) * NewOneRow->NbData))
	    {	
	      for(i=0; i<NewOneRow->NbData; i++)
		NewOneRow->Data.F4[i] = 0.0;
	      return NewOneRow;
	    }
	  else
	    {
	      _SdifError(eAllocFail, "OneRow->Data.F4 allocation");
	      free(NewOneRow);
	      return NULL;
	    }
	case eFloat8 :
	  if (NewOneRow->Data.F8 = (SdifFloat8*) malloc (sizeof(SdifFloat8) * NewOneRow->NbData))
	    {	
	      for(i=0; i<NewOneRow->NbData; i++)
		NewOneRow->Data.F8[i] = 0.0;
	      return NewOneRow;
	    }
	  else
	    {
	      _SdifError(eAllocFail, "OneRow->Data.F8 allocation");
	      free(NewOneRow);
	      return NULL;
	    }
	default :
	  sprintf(gSdifErrorMess, "Data of a OneRow : 0x%x", NewOneRow->DataType);
	  _SdifError(eTypeDataNotSupported, gSdifErrorMess);
	  free(NewOneRow);
	  return NULL;
	}
    }
  else
    {
      _SdifError(eAllocFail, "OneRow allocation");
      return NULL;
    }
}






void
SdifKillOneRow(SdifOneRowType *OneRow)
{
  if (OneRow)
    {
      switch (OneRow->DataType)
	{
	case eFloat4 :
	  if (OneRow->Data.F4)
	    {
	      free(OneRow->Data.F4);
	    }
	  else
	    _SdifError(eFreeNull, "OneRow->Data.F4 free");
	  break;
	case eFloat8 :
	  if (OneRow->Data.F8)
	    {
	      free(OneRow->Data.F8);
	    }
	  else
	    _SdifError(eFreeNull, "OneRow->Data.F8 free");
	  break;
	default :
	  sprintf(gSdifErrorMess, "Data of a OneRow : 0x%x", OneRow->DataType);
	  _SdifError(eTypeDataNotSupported, gSdifErrorMess);
	  break;
	}
     free(OneRow);
    }
  else
    _SdifError(eFreeNull, "OneRow free");
}









int
SdifFPrintOneRow(FILE *f, SdifOneRowType *OneRow)
{
  unsigned int
    iCol;
  int
    NbCharWrite = 0;
  
  switch (OneRow->DataType)
    {
    case eFloat4 :
      for(iCol=0; iCol<OneRow->NbData; iCol++)
	NbCharWrite += fprintf(f, "\t%g", OneRow->Data.F4[iCol]);
      break;
    case eFloat8 :
      for(iCol=0; iCol<OneRow->NbData; iCol++)
	NbCharWrite += fprintf(f, "\t%g", OneRow->Data.F8[iCol]);
      break;
    default :
      sprintf(gSdifErrorMess, "Data of a OneRow : 0x%x", OneRow->DataType);
      _SdifError(eTypeDataNotSupported, gSdifErrorMess);
      break;
    }
  NbCharWrite += fprintf(f, "\n");

  return NbCharWrite;
}











SdifOneRowType*
SdifOneRowPutValue(SdifOneRowType *OneRow, SdifUInt4 numCol, SdifFloat8 Value)
{
  /* numCol is in [1, NbData] */
  if ((numCol <= OneRow->NbData) && (numCol > 0))
    switch (OneRow->DataType)
      {
      case eFloat4 :
	OneRow->Data.F4[numCol-1] = (SdifFloat4) Value;
	break;
      case eFloat8 :
	OneRow->Data.F8[numCol-1] = Value;
	break;
      default :
	OneRow->Data.F4[numCol-1] = (SdifFloat4) Value;      
	break;
      }
  else
    {
      sprintf(gSdifErrorMess, "OneRow Put Value Col : %d ", numCol);
      _SdifError(eArrayPosition, gSdifErrorMess);
    }
  return OneRow;
}







SdifFloat8
SdifOneRowGetValue(SdifOneRowType *OneRow, SdifUInt4 numCol)
{
  /* numCol is in [1, NbData] */
  if ((numCol <= OneRow->NbData) && (numCol > 0))
    switch (OneRow->DataType)
      {
      case eFloat4 :
	return (SdifFloat8) OneRow->Data.F4[numCol-1];
      case eFloat8 :
	return OneRow->Data.F8[numCol-1];
      default :
	return (SdifFloat8) OneRow->Data.F4[numCol-1];
      }
  else
    {
      sprintf(gSdifErrorMess, "OneRow Get Value Col : %d ", numCol);
      _SdifError(eArrayPosition, gSdifErrorMess);
      return _SdifFloat8Error;
    }
}







SdifFloat8
SdifOneRowGetValueColName(SdifOneRowType *OneRow,
			  SdifMatrixTypeType *MatrixType,
			  char * NameCD)
{
  return SdifOneRowGetValue(OneRow,
			    SdifMatrixTypeGetNumColumnDef(MatrixType, NameCD));
}







int
SdifFWriteOneRow(SdifOneRowType *OneRow, FILE* fw)
{
  switch (OneRow->DataType)
    {
    case eFloat4 :
      return sizeof(SdifFloat4) * SdifFWriteFloat4(OneRow->Data.F4, OneRow->NbData, fw);
    case eFloat8 :
      return sizeof(SdifFloat8) * SdifFWriteFloat8(OneRow->Data.F8, OneRow->NbData, fw);
    default :
      return -1;
    }
}






int
SdifFReadOneRow(SdifOneRowType *OneRow, FILE* fr)
{
  switch (OneRow->DataType)
    {
    case eFloat4 :
      return sizeof(SdifFloat4) * SdifFReadFloat4(OneRow->Data.F4, OneRow->NbData, fr);
    case eFloat8 :
      return sizeof(SdifFloat8) * SdifFReadFloat8(OneRow->Data.F8, OneRow->NbData, fr);
    default :
      return -1;
    }
}







SdifMatrixDataType*
SdifCreateMatrixData(char *MatrixName,
		     SdifDataTypeEnum DataType,
		     SdifUInt4 NbRow,
		     SdifUInt4 NbCol)
{
  SdifMatrixDataType *NewMatrixData;
  SdifUInt4 iRow;
  SdifUInt4 MatrixSize;
  
  if (NewMatrixData = (SdifMatrixDataType*) malloc (sizeof(SdifMatrixDataType)))
    {
      NewMatrixData->Header = SdifCreateMatrixHeader(MatrixName,
						     DataType,
						     NbRow,
						     NbCol);
      MatrixSize = 
	_SdifNameLen	
	+ 3*sizeof(SdifUInt4)    /* DataType,  NbRow, NbCol */
	+ NbRow*NbCol* SdifSizeofDataType(DataType);

      /* Padding size added */
      NewMatrixData->Size = (MatrixSize + SdifPaddingCalculate(MatrixSize));

      if (NewMatrixData->Rows = (SdifOneRowType**) malloc (sizeof(SdifOneRowType*) * NbRow))
	{
	  for (iRow=0; iRow<NbRow; iRow++)
	    NewMatrixData->Rows[iRow] = SdifCreateOneRow(DataType, NbCol);
	  return NewMatrixData;
	}
      else
	{
	  _SdifError(eAllocFail, "MatrixData->Rows allocation");
	  return NULL;
	}
    }
  else
    {
      _SdifError(eAllocFail, "MatrixData allocation");
      return NULL;
    }
}









void
SdifKillMatrixData(SdifMatrixDataType *MatrixData)
{
  SdifUInt4 iRow;

  if (MatrixData)
    {
      if (MatrixData->Rows)
	{
	  for(iRow = 0; iRow<MatrixData->Header->NbRow; iRow++)
	    SdifKillOneRow(MatrixData->Rows[iRow]);
	  
	  SdifKillMatrixHeader(MatrixData->Header);
	  free(MatrixData->Rows);
	}
      else
	_SdifError(eFreeNull, "MatrixData->Rows free");
      
      free(MatrixData);
    }
  else
    _SdifError(eFreeNull, "MatrixData free");
}






int
SdifFPrintMatrixRows(FILE* f, SdifMatrixDataType *MatrixData)
{
  SdifUInt4
    iRow;
  int
    NbCharWrite = 0;

  for(iRow=0; iRow<MatrixData->Header->NbRow; iRow++)
      NbCharWrite += SdifFPrintOneRow(f, MatrixData->Rows[iRow]);

  return NbCharWrite;
}










int
SdifFWriteMatrixRows(SdifMatrixDataType *MatrixData, FILE* fw)
{
  int NbBytesWrite = 0;
  SdifUInt4 iRow;

  for(iRow=0; iRow<MatrixData->Header->NbRow; iRow++)
    NbBytesWrite += SdifFWriteOneRow(MatrixData->Rows[iRow], fw);
  
  return  NbBytesWrite;
}








int
SdifFWriteMatrixData(SdifMatrixDataType *MatrixData, FILE* fw)
{
  int NbBytesWrite = 0;

  NbBytesWrite += SdifFWriteMatrixHeader(MatrixData->Header, fw);
  NbBytesWrite += SdifFWriteMatrixRows(MatrixData, fw);
  
  NbBytesWrite += SdifFWritePadding(SdifFPaddingCalculate(fw, NbBytesWrite), fw);
  
  return  NbBytesWrite;
}








int
SdifFReadMatrixRows(SdifMatrixDataType *MatrixData, FILE* fw)
{
  int NbBytesRead = 0;
  SdifUInt4 iRow;
  
  for(iRow=0; iRow<MatrixData->Header->NbRow; iRow++)
    NbBytesRead += SdifFReadOneRow(MatrixData->Rows[iRow], fw);
  
  return NbBytesRead;
}









SdifMatrixDataType *
SdifMatrixDataPutValue(SdifMatrixDataType *MatrixData,
		       SdifUInt4  numRow,
		       SdifUInt4  numCol,
		       SdifFloat8 Value)
{
  /* numRow is in [1, MatrixData->Header->NbRow] */
  if ((numRow <=  MatrixData->Header->NbRow) && (numRow > 0))
    SdifOneRowPutValue(MatrixData->Rows[numRow-1], numCol, Value);
  else
    {
      sprintf(gSdifErrorMess, "MatrixData Put Value Row : %d ", numRow);
      _SdifError(eArrayPosition, gSdifErrorMess);
    }

  return MatrixData;
}









SdifFloat8
SdifMatrixDataGetValue(SdifMatrixDataType *MatrixData,
		       SdifUInt4  numRow,
		       SdifUInt4  numCol)
{
  /* numRow is in [1, MatrixData->Header->NbRow] */
  if ((numRow <=  MatrixData->Header->NbRow) && (numRow > 0))
    return SdifOneRowGetValue(MatrixData->Rows[numRow-1], numCol);
  else
    {
      sprintf(gSdifErrorMess, "MatrixData Get Value Row : %d ", numRow);
      _SdifError(eArrayPosition, gSdifErrorMess);
      return _SdifFloat8Error;
    }
}    








SdifMatrixDataType *
SdifMatrixDataColNamePutValue(SdifMatrixDataType *MatrixData,
			      SdifUInt4  numRow,
			      char *ColName,
			      SdifFloat8 Value)
{
  SdifMatrixTypeType* MtrxT;
  SdifUInt4 numCol;
  
  MtrxT = SdifGetMatrixType(MatrixData->Header->MatrixName);

  if (MtrxT)
    {
      numCol = (SdifUInt4) SdifMatrixTypeGetNumColumnDef(MtrxT, ColName);
      if (numCol)
	SdifMatrixDataPutValue(MatrixData, numRow, numCol, Value);
      else
	{
	  sprintf(gSdifErrorMess,
		  "'%s' of '%c%c%c%c' matrix type",
		  ColName,
		  MatrixData->Header->MatrixName[0],
		  MatrixData->Header->MatrixName[1],
		  MatrixData->Header->MatrixName[2],
		  MatrixData->Header->MatrixName[3]);
	  _SdifError(eUnDefined, gSdifErrorMess);
	}
    }
  else
    {
      sprintf(gSdifErrorMess,
	      "'%c%c%c%c' matrix type",
	      MatrixData->Header->MatrixName[0],
	      MatrixData->Header->MatrixName[1],
	      MatrixData->Header->MatrixName[2],
	      MatrixData->Header->MatrixName[3]);
      _SdifError(eUnDefined, gSdifErrorMess);
    }
  return MatrixData;
}








SdifFloat8
SdifMatrixDataColNameGetValue(SdifMatrixDataType *MatrixData,
			      SdifUInt4  numRow,
			      char *ColName)
{
  SdifMatrixTypeType* MtrxT;
  SdifUInt4 numCol;
  
  MtrxT = SdifGetMatrixType(MatrixData->Header->MatrixName);

  if (MtrxT)
    {
      numCol = (SdifUInt4) SdifMatrixTypeGetNumColumnDef(MtrxT, ColName);
      if (numCol)
	return SdifMatrixDataGetValue(MatrixData, numRow, numCol);
      else
	{
	  sprintf(gSdifErrorMess,
		  "'%s' of '%c%c%c%c' matrix type",
		  ColName,
		  MatrixData->Header->MatrixName[0],
		  MatrixData->Header->MatrixName[1],
		  MatrixData->Header->MatrixName[2],
		  MatrixData->Header->MatrixName[3]);
	  _SdifError(eUnDefined, gSdifErrorMess);
	  return _SdifFloat8Error;
	}
    }
  else
    {
      sprintf(gSdifErrorMess,
	      "'%c%c%c%c' matrix type",
	      MatrixData->Header->MatrixName[0],
	      MatrixData->Header->MatrixName[1],
	      MatrixData->Header->MatrixName[2],
	      MatrixData->Header->MatrixName[3]);
      _SdifError(eUnDefined, gSdifErrorMess);
      return _SdifFloat8Error;
    }
}






/* SdifSkipMatrixData don't read matrix header. */
int
SdifSkipMatrixData(SdifMatrixHeaderType *MtrxH, FILE *fr)
{
  int
    NbBytesRead = 0,
    NbBytesToPass;
  
  NbBytesToPass = MtrxH->NbCol * MtrxH->NbRow * SdifSizeofDataType(MtrxH->DataType);
  NbBytesToPass += SdifPaddingCalculate(NbBytesToPass);
  if (fseek(fr, NbBytesToPass, SEEK_CUR) != 0)
    return -1;
  else
    {
      NbBytesRead += NbBytesToPass;
      return NbBytesRead;
    }
}







/* SdifSkipMatrix read entire matrix header. */
int
SdifSkipMatrix(FILE *fr)
{
  int
    NbBytesRead = 0,
    NbBytesToPass;
  SdifMatrixHeaderType MtrxH;
  
  NbBytesRead = SdifFReadMatrixHeader(&MtrxH, fr);
  NbBytesRead += SdifSkipMatrixData(&MtrxH, fr);
  return NbBytesRead;
}

