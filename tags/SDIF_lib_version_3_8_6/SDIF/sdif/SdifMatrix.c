/* $Id: SdifMatrix.c,v 3.9 2004-07-22 14:47:56 bogaards Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
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
 * Revision 3.8  2003/11/07 21:47:18  roebel
 * removed XpGuiCalls.h and replaced preinclude.h  by local files
 *
 * Revision 3.7  2002/05/24 19:37:07  ftissera
 * Add include "sdif.h" to be compatible with C++
 *
 * Revision 3.6  2001/05/02 09:34:45  tisseran
 * Change License from GNU Public License to GNU Lesser Public License.
 *
 * Revision 3.5  2000/11/15 14:53:32  lefevre
 * no message
 *
 * Revision 3.4  2000/10/27  20:03:37  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.3.2.1  2000/08/21  21:35:33  tisseran
 * *** empty log message ***
 *
 * Revision 3.3  1999/10/13  16:05:51  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
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


#include "sdif_portability.h"

#include "SdifMatrix.h"
#include <stdlib.h>
#include <assert.h>
#include "sdif.h"



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
	char errorMess[_SdifStringLen];

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
      sprintf(errorMess, "Data of a OneRow : 0x%x", NewOneRow->DataType);
      _SdifError(eNotInDataTypeUnion, errorMess);
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
	  sprintf(errorMess, "Data of a OneRow : 0x%x", NewOneRow->DataType);
	  _SdifError(eNotInDataTypeUnion, errorMess);
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
      sprintf(errorMess, "Data of a OneRow : 0x%x", OneRow->DataType);
      _SdifError(eNotInDataTypeUnion, errorMess);
      return NULL;
    }

#endif
}








void
SdifKillOneRow(SdifOneRowT *OneRow)
{
	char errorMess[_SdifStringLen];
  if (OneRow)
    {
#if (_SdifFormatVersion >= 3)

#ifndef NDEBUG	
	if (!SdifDataTypeKnown (OneRow->DataType))
	{
	    sprintf (errorMess, "Data of a OneRow : 0x%x", 
		     OneRow->DataType);
	    _SdifError (eNotInDataTypeUnion, errorMess);
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
	  sprintf(errorMess, "Data of a OneRow : 0x%x", OneRow->DataType);
	  _SdifError(eNotInDataTypeUnion, errorMess);
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
	char errorMess[_SdifStringLen];
    /* case template for type from SdifDataTypeET */
#   define setcase(type) 						   \
    case e##type:   OneRow->Data.type [numCol-1] = (Sdif##type) Value;  break;
   
  /* numCol is in [1, NbData] */
  if ((numCol <= OneRow->NbData) && (numCol > 0))
    switch (OneRow->DataType)
      {
	  /* generate cases for all types */
	  sdif_foralltypes (setcase);
  
      default :
	OneRow->Data.Float4[numCol-1] = (SdifFloat4) Value;      
	break;
      }
  else
    {
      sprintf(errorMess, "OneRow Put Value Col : %d ", numCol);
      _SdifError(eArrayPosition, errorMess);
    }
  return OneRow;
}







SdifFloat8
SdifOneRowGetValue(SdifOneRowT *OneRow, SdifUInt4 numCol)
{
	char errorMess[_SdifStringLen];
    /* case template for type from SdifDataTypeET */
#   define getcase(type) 						   \
    case e##type:   return (Sdif##type) OneRow->Data.type [numCol-1];
   
  /* numCol is in [1, NbData] */
  if ((numCol <= OneRow->NbData) && (numCol > 0))
    switch (OneRow->DataType)
      {
	  /* generate cases for all types */
	  sdif_foralltypes (getcase);
  
      default :
	return (SdifFloat8) OneRow->Data.Float4[numCol-1];
      }
  else
    {
      sprintf(errorMess, "OneRow Get Value Col : %d ", numCol);
      _SdifError(eArrayPosition, errorMess);
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
	char errorMess[_SdifStringLen];
  /* numRow is in [1, MatrixData->Header->NbRow] */
  if ((numRow <=  MatrixData->Header->NbRow) && (numRow > 0))
    SdifOneRowPutValue(MatrixData->Rows[numRow-1], numCol, Value);
  else
    {
      sprintf(errorMess, "MatrixData Put Value Row : %d ", numRow);
      _SdifError(eArrayPosition, errorMess);
    }

  return MatrixData;
}









SdifFloat8
SdifMatrixDataGetValue(SdifMatrixDataT *MatrixData,
		       SdifUInt4  numRow,
		       SdifUInt4  numCol)
{
	char errorMess[_SdifStringLen];
  /* numRow is in [1, MatrixData->Header->NbRow] */
  if ((numRow <=  MatrixData->Header->NbRow) && (numRow > 0))
    return SdifOneRowGetValue(MatrixData->Rows[numRow-1], numCol);
  else
    {
      sprintf(errorMess, "MatrixData Get Value Row : %d ", numRow);
      _SdifError(eArrayPosition, errorMess);
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
	char errorMess[_SdifStringLen];
  
  MtrxT = SdifGetMatrixType(MatrixTypesTable, MatrixData->Header->Signature);

  if (MtrxT)
    {
      numCol = (SdifUInt4) SdifMatrixTypeGetNumColumnDef(MtrxT, ColName);
      if (numCol)
	SdifMatrixDataPutValue(MatrixData, numRow, numCol, Value);
      else
	{
	  sprintf(errorMess,
		  "'%s' of '%s' matrix type",
		  ColName, SdifSignatureToString(MatrixData->Header->Signature));
	  _SdifError(eNotFound, errorMess);
	}
    }
  else
    {
      sprintf(errorMess,
	      "'%s' Matrix type",SdifSignatureToString(MatrixData->Header->Signature));
      _SdifError(eNotFound, errorMess);
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
	char errorMess[_SdifStringLen];
  
  MtrxT = SdifGetMatrixType(MatrixTypesTable, MatrixData->Header->Signature);

  if (MtrxT)
    {
      numCol = (SdifUInt4) SdifMatrixTypeGetNumColumnDef(MtrxT, ColName);
      if (numCol)
	return SdifMatrixDataGetValue(MatrixData, numRow, numCol);
      else
	{
	  sprintf(errorMess,
		  "'%s' of '%s' matrix type",
		  ColName, SdifSignatureToString(MatrixData->Header->Signature));
	  _SdifError(eNotFound, errorMess);
	  return _SdifFloat8Error;
	}
    }
  else
    {
      sprintf(errorMess,
	      "'%s' Matrix type", SdifSignatureToString(MatrixData->Header->Signature));
      _SdifError(eNotFound, errorMess);
      return _SdifFloat8Error;
    }
}
