/* $Id: SdifMatrix.c,v 3.3 1999-10-13 16:05:51 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * Matrix Header, Data, Rows structure management.
 *
 * Memory allocation of SdifOneRowT* depend on size of one granule.
 * if nb rows or data type (float4 or float8),
 * then SdifReInitOneRow makes a realloc. Then think to use it before write.
 *
 * author: Dominique Virolle 1997
 * $Log: not supported by cvs2svn $
 * Revision 3.2  1999/09/28  13:09:03  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:57:06  virolle
 * SdifStdErr add
 *
 * Revision 2.4  1999/02/28  12:16:49  virolle
 * memory report
 *
 * Revision 2.3  1999/01/23  15:55:54  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:39  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 */


#include <preincluded.h>
#include "SdifMatrix.h"

#include <stdlib.h>
#include <assert.h>




SdifMatrixHeaderT*
SdifCreateMatrixHeader(SdifSignature  Signature,
		       SdifDataTypeET DataType,
		       SdifUInt4 NbRow,
		       SdifUInt4 NbCol)
{
  SdifMatrixHeaderT *NewMatrixHeader = NULL;
  
  NewMatrixHeader = SdifMalloc(SdifMatrixHeaderT);
  if (NewMatrixHeader)
    {
      NewMatrixHeader->Signature = Signature;
      NewMatrixHeader->DataType  = DataType;
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








SdifMatrixHeaderT*
SdifCreateMatrixHeaderEmpty(void)
{
  return SdifCreateMatrixHeader(eEmptySignature, eFloat4, 0, 0);
}








void
SdifKillMatrixHeader(SdifMatrixHeaderT *MatrixHeader)
{
  if (MatrixHeader)
    SdifFree(MatrixHeader);
  else
    _SdifError(eFreeNull, "MatrixHeader free");
}







SdifOneRowT*
SdifCreateOneRow(SdifDataTypeET DataType, SdifUInt4  NbGranuleAlloc)
{
  SdifOneRowT * NewOneRow = NULL;

  if (NbGranuleAlloc <= 0)
    {
      _SdifError(eAllocFail, "NbGranuleAlloc of OneRow <= 0. You must protect it at upper level.");
      return NULL;
    }

  NewOneRow = SdifMalloc(SdifOneRowT);
  if (!NewOneRow)
  {
      _SdifError(eAllocFail, "OneRow allocation");
      return NULL;
  }

  NewOneRow->DataType = DataType;
  NewOneRow->NbGranuleAlloc = NbGranuleAlloc;
  NewOneRow->NbData = 0;

#if (_SdifFormatVersion >= 3)

  if (!SdifDataTypeKnown (DataType))
  {
      sprintf(gSdifErrorMess, "Data of a OneRow : 0x%x", NewOneRow->DataType);
      _SdifError(eNotInDataTypeUnion, gSdifErrorMess);
      SdifFree(NewOneRow);
      return NULL;
  }

  NewOneRow->Data.Void = 
      (void *) SdifCalloc (char, NewOneRow->NbGranuleAlloc * _SdifGranule);
  if (!NewOneRow->Data.Void)
  {
      _SdifError(eAllocFail, "OneRow->Data allocation");
      SdifFree(NewOneRow);
      return NULL;
  }
  else
      return NewOneRow;
      
#else
      switch (NewOneRow->DataType)
	{
	case eFloat4 :
	  NewOneRow->Data.Float4 = (SdifFloat4*) SdifCalloc(char, NewOneRow->NbGranuleAlloc * _SdifGranule);
	  if (! NewOneRow->Data.Float4)
	    {
	      _SdifError(eAllocFail, "OneRow->Data.Float4 allocation");
	      SdifFree(NewOneRow);
	      return NULL;
	    }
	  else
	    return NewOneRow;

	case eFloat8 :
	  NewOneRow->Data.Float8 = (SdifFloat8*) SdifCalloc(char, NewOneRow->NbGranuleAlloc * _SdifGranule);
	  if (! NewOneRow->Data.Float8)
	    {
	      _SdifError(eAllocFail, "OneRow->Data.Float8 allocation");
	      SdifFree(NewOneRow);
	      return NULL;
	    }
	  else
	    return NewOneRow;
	default :
	  sprintf(gSdifErrorMess, "Data of a OneRow : 0x%x", NewOneRow->DataType);
	  _SdifError(eNotInDataTypeUnion, gSdifErrorMess);
	  SdifFree(NewOneRow);
	  return NULL;
	}
#endif
}




SdifOneRowT*
SdifReInitOneRow (SdifOneRowT *OneRow, SdifDataTypeET DataType, SdifUInt4 NbData)
{
  SdifUInt4 NewNbGranule;

  OneRow->NbData   = NbData;
  OneRow->DataType = DataType;


#if (_SdifFormatVersion >= 3)

  if (OneRow->NbGranuleAlloc * _SdifGranule < NbData * SdifSizeofDataType (DataType))
  {
      NewNbGranule = (NbData * SdifSizeofDataType (DataType)) / _SdifGranule;
   /* NewNbGranule = NewNbGranule ? NewNbGranule : 1; This case cannot appear*/
      OneRow->Data.Void = (void *) SdifRealloc (OneRow->Data.Void, char, 
						NewNbGranule * _SdifGranule);
      if (!OneRow->Data.Void)
      {
	  _SdifError(eAllocFail, "OneRow->Data RE-allocation");
	  return NULL;
      }
      else
      {
	  OneRow->NbGranuleAlloc = NewNbGranule;
	  return OneRow;
      }
  }
  else
      return OneRow;

#else

  switch (OneRow->DataType)
    {
      
    case eFloat4 :
      if ( (OneRow->NbGranuleAlloc * _SdifGranule) < (OneRow->NbData * sizeof(SdifFloat4)) )
	{
	  NewNbGranule = (SdifUInt4)(OneRow->NbData * sizeof(SdifFloat4)) / _SdifGranule;
	  /* NewNbGranule = (NewNbGranule) ? NewNbGranule : 1; This case cannot appear */
	  OneRow->Data.Float4 = (SdifFloat4*) SdifRealloc(OneRow->Data.Float4, char, NewNbGranule * _SdifGranule);
	  if (! OneRow->Data.Float4)
	    {
	      _SdifError(eAllocFail, "OneRow->Data.Float4 RE-allocation");
	      return NULL;
	    }
	  else
	    {
	      OneRow->NbGranuleAlloc = NewNbGranule;
	      return OneRow;
	    }
	}
      else
	return OneRow;
      
      
    case eFloat8 :
      if ( (OneRow->NbGranuleAlloc * _SdifGranule) < (OneRow->NbData * sizeof(SdifFloat8)) )
	{
	  NewNbGranule = (SdifUInt4)(OneRow->NbData * sizeof(SdifFloat8)) / _SdifGranule;
	  /* NewNbGranule = (NewNbGranule) ? NewNbGranule : 1; This case cannot appear */
	  OneRow->Data.Float8 = (SdifFloat8*) SdifRealloc(OneRow->Data.Float8, char, NewNbGranule * _SdifGranule);
	  if (! OneRow->Data.Float8)
	    {
	      _SdifError(eAllocFail, "OneRow->Data.Float8 RE-allocation");
	      return NULL;
	    }
	  else
	    {
	      OneRow->NbGranuleAlloc = NewNbGranule;
	      return OneRow;
	    }
	}
      else
	return OneRow;
    
    default :
      sprintf(gSdifErrorMess, "Data of a OneRow : 0x%x", OneRow->DataType);
      _SdifError(eNotInDataTypeUnion, gSdifErrorMess);
      return NULL;
    }

#endif
}








void
SdifKillOneRow(SdifOneRowT *OneRow)
{
  if (OneRow)
    {
#if (_SdifFormatVersion >= 3)

#ifndef NDEBUG	
	if (!SdifDataTypeKnown (OneRow->DataType))
	{
	    sprintf (gSdifErrorMess, "Data of a OneRow : 0x%x", 
		     OneRow->DataType);
	    _SdifError (eNotInDataTypeUnion, gSdifErrorMess);
	}
#endif

	if (OneRow->Data.Void)
	{
	    SdifFree (OneRow->Data.Void);
	}
	else
	    _SdifError(eFreeNull, "OneRow->Data free");

#else

      switch (OneRow->DataType)
	{
	case eFloat4 :
	  if (OneRow->Data.Float4)
	    {
	      SdifFree(OneRow->Data.Float4);
	    }
	  else
	    _SdifError(eFreeNull, "OneRow->Data.Float4 free");
	  break;
	case eFloat8 :
	  if (OneRow->Data.Float8)
	    {
	      SdifFree(OneRow->Data.Float8);
	    }
	  else
	    _SdifError(eFreeNull, "OneRow->Data.Float8 free");
	  break;
	default :
	  sprintf(gSdifErrorMess, "Data of a OneRow : 0x%x", OneRow->DataType);
	  _SdifError(eNotInDataTypeUnion, gSdifErrorMess);
	  break;
	}
     SdifFree(OneRow);

#endif

    }
  else
    _SdifError(eFreeNull, "OneRow free");
}











SdifOneRowT*
SdifOneRowPutValue(SdifOneRowT *OneRow, SdifUInt4 numCol, SdifFloat8 Value)
{
    /* case template for type from SdifDataTypeET */
#   define setcase(type) 						   \
    case e##type:   OneRow->Data.type [numCol-1] = (Sdif##type) Value;  break;
   
  /* numCol is in [1, NbData] */
  if ((numCol <= OneRow->NbData) && (numCol > 0))
    switch (OneRow->DataType)
      {
	  /* generate cases for all types */
	  sdif_foralltypes (setcase)
  
      default :
	OneRow->Data.Float4[numCol-1] = (SdifFloat4) Value;      
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
SdifOneRowGetValue(SdifOneRowT *OneRow, SdifUInt4 numCol)
{
    /* case template for type from SdifDataTypeET */
#   define getcase(type) 						   \
    case e##type:   return (Sdif##type) OneRow->Data.type [numCol-1];
   
  /* numCol is in [1, NbData] */
  if ((numCol <= OneRow->NbData) && (numCol > 0))
    switch (OneRow->DataType)
      {
	  /* generate cases for all types */
	  sdif_foralltypes (getcase)
  
      default :
	return (SdifFloat8) OneRow->Data.Float4[numCol-1];
      }
  else
    {
      sprintf(gSdifErrorMess, "OneRow Get Value Col : %d ", numCol);
      _SdifError(eArrayPosition, gSdifErrorMess);
      return _SdifFloat8Error;
    }
}







SdifFloat8
SdifOneRowGetValueColName(SdifOneRowT *OneRow,
			  SdifMatrixTypeT *MatrixType,
			  char *NameCD)
{
  return SdifOneRowGetValue(OneRow,
			    SdifMatrixTypeGetNumColumnDef(MatrixType, NameCD));
}








SdifMatrixDataT*
SdifCreateMatrixData(SdifSignature Signature,
		     SdifDataTypeET DataType,
		     SdifUInt4 NbRow,
		     SdifUInt4 NbCol)
{
  SdifMatrixDataT *NewMatrixData = NULL;
  SdifUInt4 iRow;
  SdifUInt4 NbGranule;
  SdifUInt4 MatrixSize;
  
  NewMatrixData = SdifMalloc(SdifMatrixDataT);
  if (NewMatrixData)
    {
      NewMatrixData->Header = SdifCreateMatrixHeader(Signature,
						     DataType,
						     NbRow,
						     NbCol);
      MatrixSize = 
	(int) sizeof(SdifSignature)
	+ 3*sizeof(SdifUInt4)    /* DataType,  NbRow, NbCol */
	+ NbRow*NbCol* SdifSizeofDataType(DataType);

      /* Padding size added */
      NewMatrixData->Size = (MatrixSize + SdifPaddingCalculate(MatrixSize));

      NewMatrixData->Rows = SdifCalloc(SdifOneRowT*, NbRow);
      if (NewMatrixData->Rows)
	{
	  NbGranule = (NbCol*SdifSizeofDataType(DataType))/_SdifGranule;
	  NbGranule = NbGranule ? NbGranule : 1;
	  for (iRow=0; iRow<NbRow; iRow++)
	    NewMatrixData->Rows[iRow] = SdifCreateOneRow(DataType, NbGranule);
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
SdifKillMatrixData(SdifMatrixDataT *MatrixData)
{
  SdifUInt4 iRow;

  if (MatrixData)
    {
      if (MatrixData->Rows)
	{
	  for(iRow = 0; iRow<MatrixData->Header->NbRow; iRow++)
	    SdifKillOneRow(MatrixData->Rows[iRow]);
	  
	  SdifKillMatrixHeader(MatrixData->Header);
	  SdifFree(MatrixData->Rows);
	}
      else
	_SdifError(eFreeNull, "MatrixData->Rows free");
      
      SdifFree(MatrixData);
    }
  else
    _SdifError(eFreeNull, "MatrixData free");
}








SdifMatrixDataT *
SdifMatrixDataPutValue(SdifMatrixDataT *MatrixData,
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
SdifMatrixDataGetValue(SdifMatrixDataT *MatrixData,
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





SdifMatrixDataT *
SdifMatrixDataColNamePutValue(SdifHashTableT *MatrixTypesTable,
			      SdifMatrixDataT *MatrixData,
			      SdifUInt4  numRow,
			      char *ColName,
			      SdifFloat8 Value)
{
  SdifMatrixTypeT* MtrxT;
  SdifUInt4 numCol;
  
  MtrxT = SdifGetMatrixType(MatrixTypesTable, MatrixData->Header->Signature);

  if (MtrxT)
    {
      numCol = (SdifUInt4) SdifMatrixTypeGetNumColumnDef(MtrxT, ColName);
      if (numCol)
	SdifMatrixDataPutValue(MatrixData, numRow, numCol, Value);
      else
	{
	  sprintf(gSdifErrorMess,
		  "'%s' of '%s' matrix type",
		  ColName, SdifSignatureToString(MatrixData->Header->Signature));
	  _SdifError(eNotFound, gSdifErrorMess);
	}
    }
  else
    {
      sprintf(gSdifErrorMess,
	      "'%s' Matrix type",SdifSignatureToString(MatrixData->Header->Signature));
      _SdifError(eNotFound, gSdifErrorMess);
    }
  return MatrixData;
}








SdifFloat8
SdifMatrixDataColNameGetValue(SdifHashTableT *MatrixTypesTable,
			      SdifMatrixDataT *MatrixData,
			      SdifUInt4  numRow,
			      char *ColName)
{
  SdifMatrixTypeT* MtrxT;
  SdifUInt4 numCol;
  
  MtrxT = SdifGetMatrixType(MatrixTypesTable, MatrixData->Header->Signature);

  if (MtrxT)
    {
      numCol = (SdifUInt4) SdifMatrixTypeGetNumColumnDef(MtrxT, ColName);
      if (numCol)
	return SdifMatrixDataGetValue(MatrixData, numRow, numCol);
      else
	{
	  sprintf(gSdifErrorMess,
		  "'%s' of '%s' matrix type",
		  ColName, SdifSignatureToString(MatrixData->Header->Signature));
	  _SdifError(eNotFound, gSdifErrorMess);
	  return _SdifFloat8Error;
	}
    }
  else
    {
      sprintf(gSdifErrorMess,
	      "'%s' Matrix type", SdifSignatureToString(MatrixData->Header->Signature));
      _SdifError(eNotFound, gSdifErrorMess);
      return _SdifFloat8Error;
    }
}
