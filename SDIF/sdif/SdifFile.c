/* $Id: SdifFile.c,v 2.0 1998-11-29 11:41:40 virolle Exp $
 *
 * SdifFile.c
 *
 * Sdif File Management
 * High Level.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.4  1998/05/14  09:50:34  schwarz
 * Added SdifCurrOneRowData to return a pointer to the raw data.
 * This can subsequently be used for SdifSetCurrOneRow.
 *
 */





#include "SdifFile.h"
#include "SdifTest.h"
#include "SdifErrMess.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SdifError.h"
#include "SdifRWLowLevel.h" /* for SdifInitMachineType()  into SdifFLoadPredinedTypes */
#include "SdifPreTypes.h"
#include "SdifFScan.h"






SdifFileT*
SdifFOpen(const char* Name, SdifFileModeET Mode)
{
  SdifFileT* SdifF;



  SdifF = (SdifFileT*) malloc (sizeof(SdifFileT));
  if (SdifF)
    {
      SdifF->Name             = SdifCreateStrNCpy(Name, SdifStrLen(Name)+1);
      SdifF->Mode             = Mode;
	  SdifF->FormatVersion    = _SdifFormatVersion; /* default */
      
      SdifF->NameValues       = SdifCreateNameValuesL(_SdifNameValueHashSize);
      SdifF->MatrixTypesTable = SdifCreateHashTable(_SdifGenHashSize, eHashInt4,
						                            SdifKillMatrixType);
      SdifF->FrameTypesTable  = SdifCreateHashTable(_SdifGenHashSize, eHashInt4,
						                            SdifKillFrameType);
      SdifF->StreamIDsTable   = SdifCreateHashTable(1, eHashInt4, SdifKillStreamID);
      SdifF->TimePositions    = SdifCreateTimePositionL();


      SdifF->CurrSignature = eEmptySignature;
      SdifF->CurrFramH     = NULL;
      SdifF->CurrMtrxH     = NULL;
      SdifF->CurrFramT     = NULL;
      SdifF->CurrMtrxT     = NULL;
      SdifF->PrevTime      = _Sdif_MIN_DOUBLE_;
      SdifF->MtrxUsed      = SdifCreateSignatureTab(_SdifGranule);
      /*SdifF->MtrxUsed      = SdifCreateSignatureTab(1);*/
      SdifF->CurrFramPos   = 0;
      SdifF->FileSize      = 0;
      SdifF->ChunkSize     = 0;
      SdifF->StartChunkPos = 0;
      SdifF->Pos           = 0;
      SdifF->TypeDefPass   = eNotPass;
      SdifF->StreamIDPass  = eNotPass;
 
      /* One _SdifGranule allocated --> 256 floats */
      SdifF->CurrOneRow = SdifCreateOneRow(eFloat4, 1);

      SdifF->TextStreamName   = NULL;
      SdifF->TextStream       = NULL;
      SdifF->NbOfWarning = 0;

      SdifF->Stream        = NULL;
      SdifF->Errors        = SdifCreateErrorL(SdifF);
      

      switch (Mode)
        {
      case eReadFile :
        if (SdifStrCmp(Name, "stdin") == 0)
          {
            SdifF->Stream = SdiffBinOpen (Name, eBinaryModeStdInput);
            return SdifF;
		  }
        else
          {
            if (    (SdifStrCmp(Name, "stdout") == 0)
			     || (SdifStrCmp(Name, "stderr") == 0)   )
              {
                _SdifFError(SdifF, eBadStdFile, Name);
                SdifFClose(SdifF);
				return NULL;
			  }
            else
              {
                SdifF->Stream = SdiffBinOpen (Name, eBinaryModeRead);
                if (! SdifF->Stream)
                  {
                    _SdifError(eFileNotFound, Name);
                    SdifFClose(SdifF);
                    return NULL;
                  }
                else
                  {
                    return SdifF;
                  }
              }
          }

      case eWriteFile :
          if (SdifStrCmp(Name, "stdout") == 0)
            {
              SdifF->Stream = SdiffBinOpen (Name, eBinaryModeStdOutput);
              return SdifF;
            }
		  else
            if (    (SdifStrCmp(Name, "stdin") == 0)
			     || (SdifStrCmp(Name, "stderr") == 0)   )
              {
                _SdifFError(SdifF, eBadStdFile, Name);
                SdifFClose(SdifF);
                return NULL;
              }
            else
              {
                SdifF->Stream = SdiffBinOpen (Name, eBinaryModeWrite);
                if (! SdifF->Stream)
                 {
                    _SdifError(eFileNotFound, Name);
                    SdifFClose(SdifF);
                    return NULL;
                  }
                else
                  return SdifF;
              }

      case ePredefinedTypes:
        SdifF->Stream = NULL;
        return SdifF;

      default :
        _SdifFError(SdifF, eBadMode, "this mode doesn't exist");
        SdifFClose(SdifF);
        return NULL;
      }
  }
  else
  {
    _SdifError(eAllocFail, "SdifFile");
    return NULL;
  }
}









/* Obsolete version */
SdifFileT*
SdifOpenFile(const char* Name, SdifFileModeET Mode)
{
	return SdifFOpen(Name, Mode);
}









SdifFileT*
SdifFOpenText(SdifFileT *SdifF, const char* Name, SdifFileModeET Mode)
{
  SdifF->TextStreamName = SdifCreateStrNCpy(Name, SdifStrLen(Name)+1);

  switch (Mode)
  {
	case eReadFile :
      if (SdifStrCmp(Name, "stdin") == 0)
	  {
	    SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeStdInput);
	    return SdifF;
	  }
	  else
	  {
	    if (    (SdifStrCmp(Name, "stdout") == 0)
			 || (SdifStrCmp(Name, "stderr") == 0)   )
		{
		  _SdifFError(SdifF, eBadStdFile, Name);
		  SdifFClose(SdifF);
		  return NULL;
	    }
	    else
		{
	      SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeRead);
	      if (! SdifF->TextStream)
		  {
		    _SdifError(eFileNotFound, Name);
		    SdifFClose(SdifF);
		    return NULL;
		  }
	      else
		  {
		    return SdifF;
		  }
		}
	  }

	case eWriteFile :
	  if (SdifStrCmp(Name, "stdout") == 0)
	  {
	    SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeStdOutput);
	    return SdifF;
	  }
	  else
	  {
        if (SdifStrCmp(Name, "stderr") == 0)
		{
	      SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeStdError);
	      return SdifF;
		}
		else
		{
	      if (SdifStrCmp(Name, "stdin") == 0)
		  {
		    _SdifFError(SdifF, eBadStdFile, Name);
		    SdifFClose(SdifF);
		    return NULL;
		  }
	      else
		  {
	        SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeWrite);
	        if (! SdifF->TextStream)
			{
		      _SdifError(eAllocFail, Name);	  
		      SdifFClose(SdifF);
		      return NULL;
			}
	        else
			{
		      return SdifF;
			}
		  }
		}
	  }
	case ePredefinedTypes:
      if (SdifStrCmp(Name, "stdin") == 0)
	  {
	    SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeStdInput);
	    return SdifF;
	  }
	  else
	  {
	    if (    (SdifStrCmp(Name, "stdout") == 0)
			 || (SdifStrCmp(Name, "stderr") == 0)   )
		{
		  _SdifFError(SdifF, eBadStdFile, Name);
		  SdifFClose(SdifF);
		  return NULL;
	    }
	    else
		{
	      SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeRead);
	      if (! SdifF->TextStream)
		  {
		    _SdifError(eFileNotFound, Name);
		    return NULL;
		  }
	      else
		  {
		    return SdifF;
		  }
		}
	  }
	default :
	  _SdifFError(SdifF, eBadMode, "this mode doesn't exist or isn't appropriated");
	  SdifFClose(SdifF);
	  return NULL;
	}
}








void
SdifFClose(SdifFileT* SdifF)
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
      if (SdifF->Errors)             SdifKillErrorL(SdifF->Errors);
        else                         _SdifError (eFreeNull, "SdifFile->Errors");


      if (SdifF->CurrFramH)
        SdifKillFrameHeader(SdifF->CurrFramH);
      if (SdifF->CurrMtrxH)
        SdifKillMatrixHeader(SdifF->CurrMtrxH);

      if (SdifF->TextStreamName)
        free(SdifF->TextStreamName);


      if (SdifF->Stream)
		if (    (SdifF->Stream != stdout)
			 && (SdifF->Stream != stdin)
			 && (SdifF->Stream != stderr)  )
          SdiffBinClose (SdifF->Stream);


      if (SdifF->TextStream)
		if (    (SdifF->TextStream != stdout)
			 && (SdifF->TextStream != stdin)
			 && (SdifF->TextStream != stderr)  )
          SdiffBinClose(SdifF->TextStream);



      free(SdifF);
    }
    
}







/* Obsolete version */
void
SdifCloseFile(SdifFileT* SdifF)
{
	SdifFClose(SdifF);
}









SdifFrameHeaderT*
SdifFCreateCurrFramH(SdifFileT* SdifF, SdifSignature Signature)
{
  if (! (SdifF->CurrFramH))
    SdifF->CurrFramH = SdifCreateFrameHeaderEmpty(Signature);
  else
    SdifF->CurrFramH->Signature = Signature;

  return SdifF->CurrFramH;
}





SdifMatrixHeaderT*
SdifFCreateCurrMtrxH(SdifFileT* SdifF)
{
  if (! (SdifF->CurrMtrxH))
    SdifF->CurrMtrxH = SdifCreateMatrixHeaderEmpty();

  return SdifF->CurrMtrxH;
}










FILE*
SdifFGetFILE_SwitchVerbose(SdifFileT* SdifF, int Verbose)
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
  SdifCreatePredefinedTypes(SdifF->MatrixTypesTable, SdifF->FrameTypesTable);
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
    {
      SdifFOpenText(SdifF, TypesFileName, ePredefinedTypes);
      if (! SdifF->TextStream)
        {
          _SdifRemark("Load Coded Predefinied Types, it can be incomplete (file not found)\n");
          SdifTakeCodedPredefinedTypes(SdifF);
        }
      else
        {
            SdifFScanGeneralHeader   (SdifF);
            SdifFScanAllASCIIChunks  (SdifF);
        }
    }
}







SdifFileT *gSdifPredefinedTypes;

/* _SdifTypesFileName is normaly defined
 * in the Makefile with -D_SdifTypesFileName="<FileNameWithPath>"
 * then default _SdifTypesFileName is not used.
 */
#ifndef _SdifTypesFileName
#define _SdifTypesFileName  "SdifTypes.STYP"
#endif

void
SdifGenInit(char *PredefinedTypesFile)
{
  char *PreTypesEnvVar=NULL;
  
  SdifInitMachineType();
  SdifSetStdIOBinary (); /* only WIN32 */

  gSdifPredefinedTypes = SdifFOpen("Predefined", ePredefinedTypes);

  if ( (!PredefinedTypesFile) || (strlen(PredefinedTypesFile)== 0) )
    {
      PreTypesEnvVar = getenv(_SdifEnvVar);
      if (PreTypesEnvVar)
        SdifFLoadPredefinedTypes(gSdifPredefinedTypes, PreTypesEnvVar);
      else
        SdifFLoadPredefinedTypes(gSdifPredefinedTypes,  _SdifTypesFileName);
    }
  else
    {
      SdifFLoadPredefinedTypes(gSdifPredefinedTypes, PredefinedTypesFile);
    }

}





void
SdifGenKill(void)
{
  SdifFClose(gSdifPredefinedTypes);
}



void SdifPrintVersion(void)
{
#ifndef lint
  static char rcsid[]= "$Revision: 2.0 $ IRCAM $Date: 1998-11-29 11:41:40 $";
#endif


  fprintf(stderr, "SDIF Library\n");
  fprintf(stderr, "Format version : %d\n", _SdifFormatVersion);

#ifndef lint
  fprintf(stderr, "CVS: %s\n", rcsid);
#endif

  fprintf(stderr, "Release: %s, %s\n", _SDIF_VERSION, __DATE__);
}




SdifFrameHeaderT*
SdifFSetCurrFrameHeader(SdifFileT     *SdifF,
		       SdifSignature Signature,
		       SdifUInt4     Size,
		       SdifUInt4     NbMatrix,
		       SdifUInt4     NumID,
		       SdifFloat8    Time)
{
  SdifFCreateCurrFramH(SdifF, Signature);
  SdifF->CurrFramH->Size      = Size;
  SdifF->CurrFramH->NbMatrix  = NbMatrix;
  SdifF->CurrFramH->NumID     = NumID;
  SdifF->CurrFramH->Time      = Time;
  return SdifF->CurrFramH;
}





SdifMatrixHeaderT*
SdifFSetCurrMatrixHeader(SdifFileT     *SdifF,
			SdifSignature  Signature,
			SdifDataTypeET DataType,
			SdifUInt4      NbRow,
			SdifUInt4      NbCol)
{
  SdifFCreateCurrMtrxH(SdifF);
  SdifF->CurrMtrxH->Signature = Signature;
  SdifF->CurrMtrxH->DataType  = DataType;
  SdifF->CurrMtrxH->NbRow     = NbRow;
  SdifF->CurrMtrxH->NbCol     = NbCol;

  SdifReInitOneRow(SdifF->CurrOneRow, DataType, NbCol);

  return SdifF->CurrMtrxH;
}







SdifOneRowT*
SdifFSetCurrOneRow(SdifFileT *SdifF, void *Values)
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
SdifFSetCurrOneRowCol(SdifFileT *SdifF, SdifUInt4 numCol, SdifFloat8 Value)
{
  return SdifOneRowPutValue(SdifF->CurrOneRow, numCol, Value);
}



SdifFloat8
SdifFCurrOneRowCol(SdifFileT *SdifF, SdifUInt4 numCol)
{
  return SdifOneRowGetValue(SdifF->CurrOneRow, numCol);
}

SdifFloat8
SdifFCurrOneRowColName(SdifFileT *SdifF, SdifMatrixTypeT *MatrixType, char *NameCD)
{
  return SdifOneRowGetValue(SdifF->CurrOneRow,
			    SdifMatrixTypeGetNumColumnDef(MatrixType, NameCD));
}

SdifSignature
SdifFCurrSignature(SdifFileT *SdifF)
{
  return SdifF->CurrSignature;
}

SdifSignature
SdifFCleanCurrSignature(SdifFileT *SdifF)
{
  SdifF->CurrSignature = eEmptySignature;
  return SdifF->CurrSignature;
}

SdifSignature
SdifFCurrFrameSignature(SdifFileT *SdifF)
{
  return SdifF->CurrFramH->Signature;
}

SdifSignature
SdifFCurrMatrixSignature(SdifFileT *SdifF)
{
  return SdifF->CurrMtrxH->Signature;
}

SdifOneRowT*
SdifFCurrOneRow(SdifFileT *SdifF)
{
  return SdifF->CurrOneRow;
}

/*  DS: Added SdifCurrOneRowData to return a pointer to the raw data.
    This can subsequently be used for SdifSetCurrOneRow.
 */
void*
SdifFCurrOneRowData(SdifFileT *SdifF)
{
  switch (SdifF->CurrOneRow->DataType)
    {
    case eFloat4:
      return SdifF->CurrOneRow->Data.F4;
    case eFloat8:
      return SdifF->CurrOneRow->Data.F8;
    default:
      return NULL;
    }
}

SdifUInt4
SdifFCurrNbCol(SdifFileT *SdifF)
{
  return SdifF->CurrMtrxH->NbCol;
}

SdifUInt4
SdifFCurrNbRow(SdifFileT *SdifF)
{
  return SdifF->CurrMtrxH->NbRow;
}

SdifUInt4
SdifFCurrNbMatrix(SdifFileT *SdifF)
{
  return SdifF->CurrFramH->NbMatrix;
}

SdifUInt4
SdifFCurrID(SdifFileT *SdifF)
{
  return SdifF->CurrFramH->NumID;
}

SdifFloat8
SdifFCurrTime(SdifFileT *SdifF)
{
  return SdifF->CurrFramH->Time;
}












/***** SdifSignatureTabT *****/


SdifSignatureTabT*
SdifCreateSignatureTab (SdifUInt4 NbSignMax)
{
  SdifSignatureTabT* NewSignTab = NULL;
  SdifUInt4 iSign;

  NewSignTab = (SdifSignatureTabT*) malloc (sizeof(SdifSignatureTabT));
  if (NewSignTab)
    {
      NewSignTab->Tab = (SdifSignature*) malloc (NbSignMax * sizeof(SdifSignature));
      if (NewSignTab->Tab)
        {
          NewSignTab->NbSignMax = NbSignMax;
          for (iSign=0; iSign<NewSignTab->NbSignMax; iSign++)
            NewSignTab->Tab[iSign] = 0;
          NewSignTab->NbSign    = 0;
        }
      else
        {
          _SdifError(eAllocFail, "NewSignTab->Tab");
          return NULL;
        }
    }
  else
    {
      _SdifError(eAllocFail, "NewSignTab");
      return NULL;
    }

  return NewSignTab;
}




SdifSignatureTabT*
SdifReInitSignatureTab (SdifSignatureTabT* SignTab, SdifUInt4 NewNbSignMax)
{
  SdifUInt4 iSign;

  if (SignTab->NbSignMax  < NewNbSignMax)
	{
	  SignTab->Tab = (SdifSignature*) realloc (SignTab->Tab, NewNbSignMax);
	  if (SignTab->Tab )
	    {
	      SignTab->NbSignMax = NewNbSignMax;
	    }
	  else
	    {
	      _SdifError(eAllocFail, "SignTab->Tab RE-allocation");
	      return NULL;
	    }
  }

  for (iSign=0; iSign<NewNbSignMax; iSign++)
    SignTab->Tab[iSign] = 0;
  SignTab->NbSign  = 0;

  return SignTab;
}




SdifSignature
SdifIsInSignatureTab (SdifSignatureTabT* SignTab, SdifSignature Sign)
{
  SdifUInt4 iSign;

  for (iSign=0; iSign<SignTab->NbSign; iSign++)
    if (SignTab->Tab[iSign] == Sign)
      return Sign;

  return 0;
}


SdifSignatureTabT*
SdifPutInSignatureTab (SdifSignatureTabT* SignTab, SdifSignature Sign)
{
  SignTab->Tab[SignTab->NbSign] = Sign;
  SignTab->NbSign++;
  return SignTab;
}


SdifFileT*
SdifFReInitMtrxUsed (SdifFileT *SdifF)
{
  SdifReInitSignatureTab(SdifF->MtrxUsed, SdifF->CurrFramH->NbMatrix);
  return SdifF; 
}

SdifSignature
SdifFIsInMtrxUsed (SdifFileT *SdifF, SdifSignature Sign)
{
  return SdifIsInSignatureTab (SdifF->MtrxUsed, Sign);
}

SdifFileT*
SdifFPutInMtrxUsed (SdifFileT *SdifF, SdifSignature Sign)
{
  SdifPutInSignatureTab (SdifF->MtrxUsed,  Sign);
  return SdifF; 
}


/* Error management */

SdifErrorT*
SdifFLastError (SdifFileT *SdifF)
{
  return SdifLastError(SdifF->Errors);
}



SdifErrorTagET
SdifFLastErrorTag (SdifFileT *SdifF)
{
  return SdifLastErrorTag(SdifF->Errors);
}
