/* SdifFile.c
 *
 * Sdif File Management
 * High Level.
 *
 * author: Dominique Virolle 1997
 * 
 *
 */


#include "SdifFile.h"
#include "SdifTest.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SdifError.h"
#include "SdifRWLowLevel.h" /* for SdifInitMachineType()  into SdifFLoadPredinedTypes */
#include "SdifPreTypes.h"
#include "SdifFScan.h"

SdifFileT*
SdifOpenFile(char* Name, SdifFileModeET Mode)
{
  SdifFileT* SdifFile;



  if (SdifFile = (SdifFileT*) malloc (sizeof(SdifFileT)))
    {
      SdifFile->Name             = SdifCreateStrNCpy(Name, SdifStrLen(Name)+1);
      SdifFile->Mode             = Mode;
      
      SdifFile->NameValues       = SdifCreateNameValuesL(_SdifNameValueHashSize);
      SdifFile->MatrixTypesTable = SdifCreateHashTable(_SdifGenHashSize, eHashInt4,
						       SdifKillMatrixType);
      SdifFile->FrameTypesTable  = SdifCreateHashTable(_SdifGenHashSize, eHashInt4,
						       SdifKillFrameType);
      SdifFile->StreamIDsTable   = SdifCreateHashTable(1, eHashInt4, SdifKillStreamID);
      SdifFile->TimePositions    = SdifCreateTimePositionL();



      SdifFile->CurrSignature = eEmptySignature;
      SdifFile->CurrFramH     = NULL;
      SdifFile->CurrMtrxH     = NULL;
      SdifFile->CurrFramPos   = 0;
      SdifFile->FileSize      = 0;
      SdifFile->ChunkSize     = 0;
      SdifFile->StartChunkPos = 0;
      SdifFile->Pos           = 0;
      SdifFile->TypeDefPass   = eNotPass;
      SdifFile->StreamIDPass  = eNotPass;

      SdifFile->CurrOneRow = SdifCreateOneRow(eFloat4, 1); /* One _GsdifGranule allocated --> 256 floats */

      SdifFile->TextStreamName   = NULL;
      SdifFile->TextStream       = NULL;
      SdifFile->NbOfWarning = 0;

    
      switch (Mode)
	{

	case eReadFile :
	  if (SdifStrCmp(Name, "stdin") == 0)
	    {
	      SdifFile->Stream = stdin;
	      return SdifFile;
	    }
	  else
	    if (    (SdifStrCmp(Name, "stdout") == 0)
		 || (SdifStrCmp(Name, "stderr") == 0)   )
	      {
		_SdifFileMess(SdifFile, eBadStdFile, Name);
		SdifCloseFile(SdifFile);
		return NULL;
	      }
	    else
	      if (! (SdifFile->Stream = fopen (Name, "r")))
		{
		  _SdifError(eFileNotFound, Name);
		  SdifCloseFile(SdifFile);
		  return NULL;
		}
	      else
		return SdifFile;
	  

	case eWriteFile :
	  if (SdifStrCmp(Name, "stdout") == 0)
	    {
	      SdifFile->Stream = stdout;
	      return SdifFile;
	    }
	  else
	    if (    (SdifStrCmp(Name, "stdin") == 0)
		 || (SdifStrCmp(Name, "stderr") == 0)   )
	      {
		_SdifFileMess(SdifFile, eBadStdFile, Name);
		SdifCloseFile(SdifFile);
		return NULL;
	      }
	    else
	      if (! (SdifFile->Stream = fopen (Name, "w")))
		{
		  _SdifError(eAllocFail, Name);	  
		  SdifCloseFile(SdifFile);
		  return NULL;
		}
	      else
		return SdifFile;

	case ePredefinedTypes:
	  SdifFile->Stream = NULL;
	  return SdifFile;

	default :
	  _SdifFileMess(SdifFile, eBadMode, "this mode doesn't exist");
	  SdifCloseFile(SdifFile);
	  return NULL;
	}
    }
  else
    {
      _SdifError(eAllocFail, "SdifFile");
      return NULL;
    }
}






void
SdifCloseFile(SdifFileT* SdifF)
{
  if (SdifF)
    {
      if (SdifF->Name)               free (SdifF->Name);
        else                         _SdifError (eFreeNull, "SdifFile->Name");
      if (SdifF->NameValues)         SdifKillNameValuesL (SdifF->NameValues);
        else                         _SdifError (eFreeNull, "SdifFile->NameValues");
      if (SdifF->MatrixTypesTable)   SdifKillHashTable (SdifF->MatrixTypesTable);
        else                         _SdifError (eFreeNull, "SdifFile->MatrixTypesTable");
      if (SdifF->FrameTypesTable)    SdifKillHashTable (SdifF->FrameTypesTable);
        else                         _SdifError (eFreeNull, "SdifFile->FrameTypesTable");
      if (SdifF->StreamIDsTable)     SdifKillHashTable (SdifF->StreamIDsTable);
        else                         _SdifError (eFreeNull, "SdifFile->StreamIDsTable");
      if (SdifF->TimePositions)      SdifKillTimePositionL (SdifF->TimePositions);
        else                         _SdifError (eFreeNull, "SdifFile->TimePositions");
      if (SdifF->CurrOneRow)         SdifKillOneRow(SdifF->CurrOneRow);
        else                         _SdifError (eFreeNull, "SdifFile->CurrOneRow");


      if (SdifF->CurrFramH)
	SdifKillFrameHeader(SdifF->CurrFramH);
      if (SdifF->CurrMtrxH)
	SdifKillMatrixHeader(SdifF->CurrMtrxH);

      if (SdifF->TextStreamName)
	free(SdifF->TextStreamName);


      if (SdifF->Stream)
	fclose (SdifF->Stream);
      if (SdifF->TextStream)
	fclose(SdifF->TextStream);

      free(SdifF);
    }
    
}






SdifFrameHeaderT*
SdifFileCreateCurrFramH(SdifFileT* SdifF, SdifSignature Signature)
{
  if (! (SdifF->CurrFramH))
    SdifF->CurrFramH = SdifCreateFrameHeaderEmpty(Signature);
  else
    SdifF->CurrFramH->Signature = Signature;

  return SdifF->CurrFramH;
}





SdifMatrixHeaderT*
SdifFileCreateCurrMtrxH(SdifFileT* SdifF)
{
  if (! (SdifF->CurrMtrxH))
    SdifF->CurrMtrxH = SdifCreateMatrixHeaderEmpty();

  return SdifF->CurrMtrxH;
}






FILE*
SdifFileGetFILE_SwitchVerbose(SdifFileT* SdifF, int Verbose)
{
  switch (Verbose)
    {
    case 't' :
      return SdifF->TextStream;
    case 's' :
      return SdifF->Stream;
    default :
      fprintf(stderr, "*Sdif* %c not a verbose ('t': text; 's':SdifFile)\n", Verbose);
      return NULL;
    }
}









void
SdifTakeCodedPredefinedTypes(SdifFileT *SdifF)
{
  SdifPutMatrixType(SdifF->MatrixTypesTable,   &M_1FQ0);
  SdifPutMatrixType(SdifF->MatrixTypesTable,   &M_1FOF);
  SdifPutMatrixType(SdifF->MatrixTypesTable,   &M_1CHA);
  SdifPutMatrixType(SdifF->MatrixTypesTable,   &M_1RES);
  SdifPutMatrixType(SdifF->MatrixTypesTable,   &M_1DIS);
  SdifF->MatrixTypesTable->Killer = NULL;

  SdifPutFrameType(SdifF->FrameTypesTable,   &F_1FOB);
  SdifPutFrameType(SdifF->FrameTypesTable,   &F_1REB);
  SdifPutFrameType(SdifF->FrameTypesTable,   &F_1NOI);
  SdifF->FrameTypesTable->Killer = NULL;
}







/* TextStream becomes temporaly the file whih contains predefined types.
 */
void
SdifFLoadPredefinedTypes(SdifFileT *SdifF, char *TypesFileName)
{
  size_t SizeR =0;

  if (SdifStrCmp(TypesFileName, "") == 0)
    {
      _SdifRemark("Load Coded Predefinied Types, it can be incomplete (file name null)\n");
      SdifTakeCodedPredefinedTypes(SdifF);
    }
  else
    if (! (SdifF->TextStream = fopen(TypesFileName, "r")) )
      {
	_SdifRemark("Load Coded Predefinied Types, it can be incomplete (file not found)\n");
	SdifTakeCodedPredefinedTypes(SdifF);
      }
    else
      {
	SdifF->TextStreamName = SdifCreateStrNCpy(TypesFileName, SdifStrLen(TypesFileName)+1);
	
	SizeR += SdifFScanGeneralHeader(SdifF);
	
	SdiffGetSignature(SdifF->TextStream, &(SdifF->CurrSignature), &SizeR);
	while (    (SdifF->CurrSignature != eENDF)
		   && (!feof(SdifF->TextStream)))
	  {
	    if (SdifF->CurrSignature == e1TYP)
	      SizeR += SdifFScanAllType(SdifF);
	    else
	      _SdifFileMess(SdifF, eBadTypesFile, TypesFileName);
	    
	    SizeR += SdiffGetSignature(SdifF->TextStream, &(SdifF->CurrSignature), &SizeR);
	  }  
	
	fclose(SdifF->TextStream);
	SdifF->TextStream = NULL;
      }
}







SdifFileT *gSdifPredefinedTypes;

void
SdifGenInit(char *PredefinedTypesFile)
{
  SdifInitMachineType();
  gSdifPredefinedTypes = SdifOpenFile("Predefined", ePredefinedTypes);
  SdifFLoadPredefinedTypes(gSdifPredefinedTypes, PredefinedTypesFile);
}





void
SdifGenKill(void)
{
  SdifCloseFile(gSdifPredefinedTypes);
}








SdifFrameHeaderT*
SdifSetCurrFrameHeader(SdifFileT     *SdifF,
		       SdifSignature Signature,
		       SdifUInt4     Size,
		       SdifUInt4     NbMatrix,
		       SdifUInt4     NumID,
		       SdifFloat8    Time)
{
  SdifFileCreateCurrFramH(SdifF, Signature);
  SdifF->CurrFramH->Size      = Size;
  SdifF->CurrFramH->NbMatrix  = NbMatrix;
  SdifF->CurrFramH->NumID     = NumID;
  SdifF->CurrFramH->Time      = Time;
  return SdifF->CurrFramH;
}





SdifMatrixHeaderT*
SdifSetCurrMatrixHeader(SdifFileT     *SdifF,
			SdifSignature  Signature,
			SdifDataTypeET DataType,
			SdifUInt4      NbRow,
			SdifUInt4      NbCol)
{
  SdifFileCreateCurrMtrxH(SdifF);
  SdifF->CurrMtrxH->Signature = Signature;
  SdifF->CurrMtrxH->DataType  = DataType;
  SdifF->CurrMtrxH->NbRow     = NbRow;
  SdifF->CurrMtrxH->NbCol     = NbCol;

  SdifReInitOneRow(SdifF->CurrOneRow, DataType, NbCol);

  return SdifF->CurrMtrxH;
}







SdifOneRowT*
SdifSetCurrOneRow(SdifFileT *SdifF, void *Values)
{
  switch (SdifF->CurrOneRow->DataType)
    {
    case eFloat4:
      memcpy(SdifF->CurrOneRow->Data.F4, (char*) Values, sizeof(SdifFloat4)* SdifF->CurrOneRow->NbData);
      break;
    case eFloat8:
      memcpy(SdifF->CurrOneRow->Data.F8, (char*) Values, sizeof(SdifFloat8)* SdifF->CurrOneRow->NbData);
      break;
    default:
      return NULL;
    }

  return SdifF->CurrOneRow;
}


SdifOneRowT*
SdifSetCurrOneRowCol(SdifFileT *SdifF, SdifUInt4 numCol, SdifFloat8 Value)
{
  return SdifOneRowPutValue(SdifF->CurrOneRow, numCol, Value);
}



SdifFloat8
SdifCurrOneRowCol(SdifFileT *SdifF, SdifUInt4 numCol)
{
  return SdifOneRowGetValue(SdifF->CurrOneRow, numCol);
}

SdifFloat8
SdifCurrOneRowColName(SdifFileT *SdifF, SdifMatrixTypeT *MatrixType, char *NameCD)
{
  return SdifOneRowGetValue(SdifF->CurrOneRow,
			    SdifMatrixTypeGetNumColumnDef(MatrixType, NameCD));
}

SdifSignature
SdifCurrSignature(SdifFileT *SdifF)
{
  return SdifF->CurrSignature;
}

SdifSignature
SdifCleanCurrSignature(SdifFileT *SdifF)
{
  SdifF->CurrSignature = eEmptySignature;
  return SdifF->CurrSignature;
}

SdifSignature
SdifCurrFrameSignature(SdifFileT *SdifF)
{
  return SdifF->CurrMtrxH->Signature;
}

SdifSignature
SdifCurrMatrixSignature(SdifFileT *SdifF)
{
  return SdifF->CurrMtrxH->Signature;
}

SdifOneRowT*
SdifCurrOneRow(SdifFileT *SdifF)
{
  return SdifF->CurrOneRow;
}

SdifUInt4
SdifCurrNbCol(SdifFileT *SdifF)
{
  return SdifF->CurrMtrxH->NbCol;
}

SdifUInt4
SdifCurrNbRow(SdifFileT *SdifF)
{
  return SdifF->CurrMtrxH->NbRow;
}

SdifUInt4
SdifCurrNbMatrix(SdifFileT *SdifF)
{
  return SdifF->CurrFramH->NbMatrix;
}

SdifUInt4
SdifCurrID(SdifFileT *SdifF)
{
  return SdifF->CurrFramH->NumID;
}

SdifFloat8
SdifCurrTime(SdifFileT *SdifF)
{
  return SdifF->CurrFramH->Time;
}

