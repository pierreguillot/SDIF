/* $Id: SdifFile.c,v 3.13.2.1 2000-08-21 14:04:15 tisseran Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifFile.c
 *
 * Sdif File Management
 * High Level.
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.11  2000/05/12  14:41:47  schwarz
 * On behalf of Adrien, synchronisation with Mac sources, with some slight
 * changes because of cross-platform issues:
 * - Mac only stuff: XpSetFileAttribute XpFileSize
 * - Cross platform wrapper: XpGetenv XpExit
 * - Dangerous: strings.h (and thus bzero, bcopy) is not ANSI and thus doesn't
 *   exist on Mac.  Use string.h and memset, memcpy.
 *
 * Revision 3.10  2000/05/04  15:05:47  schwarz
 * SDIF Selection is now parsed automatically on opening a file,
 * and placed in file->Selection.
 * Moved SdifCheckFileFormat and SdifSignatureTab functions into
 * separate files SdifCheck and SdifSignatureTab.
 *
 * Revision 3.9  2000/04/26  15:31:23  schwarz
 * Added SdifGenInitCond for conditional initialisation.
 *
 * Revision 3.8  2000/03/01  11:19:36  schwarz
 * Tough check for pipe on open.
 * Added SdifFCurrDataType.
 *
 * Revision 3.7  1999/11/17  16:22:37  schwarz
 * SdifCheckFileFormat now only reads the first 4 bytes.  This avoids a
 * subtle bug if the file was shorter than 16 bytes, as e.g. with f0
 * test files:  The library would bail out on EOF (not a good idea to do
 * that anyway).
 *
 * Revision 3.6  1999/10/13  16:05:45  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
 * Revision 3.5  1999/10/07  15:12:22  schwarz
 * Added isSeekable flag in SdifFileT struct.  This allows to simplify the
 * many tests for stdio on opening the stream.
 * Added SdifStrEq utility function.
 *
 * Revision 3.4  1999/09/28  13:08:57  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.3  1999/09/28  10:36:59  schwarz
 * Added SdifCheckFileFormat to test if a file is in SDIF.
 *
 * Revision 3.2  1999/09/20  13:21:57  schwarz
 * Introduced user data and access functions SdifFAddUserData/GetUserData.
 *
 * Revision 3.1  1999/03/14  10:56:49  virolle
 * SdifStdErr add
 *
 * Revision 2.5  1999/02/28  12:16:39  virolle
 * memory report
 *
 * Revision 2.4  1999/01/23  15:55:47  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.3  1999/01/23  13:57:32  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.2  1998/12/21  18:27:15  schwarz
 * Inserted copyright message.
 *
 * Revision 2.1  1998/12/09  15:05:25  virolle
 * no segmentation fault when opening failed.
 *
 * Revision 2.0  1998/11/29  11:41:40  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.4  1998/05/14  09:50:34  schwarz
 * Added SdifCurrOneRowData to return a pointer to the raw data.
 * This can subsequently be used for SdifSetCurrOneRow.
 *
 */





#include <preincluded.h>
#include "XpGuiCalls.h"
#include "SdifFile.h"
#include "SdifTest.h"
#include "SdifSelect.h"
#include "SdifErrMess.h"
#include "SdifFRead.h"		/* for SdifFReadGeneralHeader */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "SdifError.h"
#include "SdifRWLowLevel.h" /* for SdifInitMachineType()  into SdifFLoadPredinedTypes */
#include "SdifPreTypes.h"
#include "SdifFScan.h"


/* #include <sys/types.h> */
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif


SdifFileT*
SdifFOpen(const char* Name, SdifFileModeET Mode)
{
  SdifFileT* SdifF;


  assert (gSdifInitialised  &&  "SDIF library not initialised!");

  SdifF = SdifMalloc(SdifFileT);
  if (SdifF)
    {
      /* Check if doing standard i/o or file i/o.  
	 stdio holds the standard i/o type as eBinaryModeStd(Input|Output),
	 eBinaryModeUnknown means file i/o */
      SdifBinaryModeET    stdio;   

      /* Split name and selection */
      SdifF->Selection        = SdifCreateSelection ();
      SdifF->Name             = SdifGetFilenameAndSelection (Name, 
							     SdifF->Selection);

      if (SdifF->Name == NULL)
	  SdifF->Name = "";
      if (SdifF->Name [0] == 0  ||  SdifStrEq (SdifF->Name, "-"))
	  stdio = Mode == eReadFile		    ?  eBinaryModeStdInput
						    :  eBinaryModeStdOutput;
      else
	  stdio = SdifStrEq(SdifF->Name, "stdin")   ?  eBinaryModeStdInput   :
		  SdifStrEq(SdifF->Name, "stdout")  ?  eBinaryModeStdOutput  :
		  SdifStrEq(SdifF->Name, "stderr")  ?  eBinaryModeStdError   : 
						       eBinaryModeUnknown;

      SdifF->Mode             = Mode;
      SdifF->FormatVersion    = _SdifFormatVersion; /* default */
      SdifF->TypesVersion     = _SdifTypesVersion;  /* default */
      
      SdifF->NameValues       = SdifCreateNameValuesL (_SdifNameValueHashSize);
      SdifF->MatrixTypesTable = SdifCreateHashTable   (_SdifGenHashSize, 
						       eHashInt4, 
						       SdifKillMatrixType);
      SdifF->FrameTypesTable  = SdifCreateHashTable   (_SdifGenHashSize, 
						       eHashInt4,
						       SdifKillFrameType);
/*      SdifF->StreamIDsTable   = SdifCreateHashTable(1, eHashInt4, SdifKillStreamID);*/
      SdifF->StreamIDsTable   = SdifCreateStreamIDTable(1);
      SdifF->TimePositions    = SdifCreateTimePositionL();

      SdifF->CurrSignature = eEmptySignature;
      SdifF->CurrFramH     = NULL;
      SdifF->CurrMtrxH     = NULL;
      SdifF->CurrFramT     = NULL;
      SdifF->CurrMtrxT     = NULL;
      SdifF->PrevTime      = _Sdif_MIN_DOUBLE_;
      SdifF->MtrxUsed      = SdifCreateSignatureTab (_SdifSignatureTabGranule);
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

      SdifF->NbUserData    = 0;
      

      switch (Mode)
      {
          case eReadFile:
	      switch (stdio)
	      {
	          case eBinaryModeStdInput:
		      SdifF->Stream = SdiffBinOpen (SdifF->Name, 
						    eBinaryModeStdInput);
		  break;

		  case eBinaryModeUnknown:
		      SdifF->Stream = SdiffBinOpen (SdifF->Name, 
						    eBinaryModeRead);
		  break;

	          default:
		      _SdifFError(SdifF, eBadStdFile, SdifF->Name);
		  break;
	      }
	  break;

          case eWriteFile :
	      switch (stdio)
	      {
	          case eBinaryModeStdOutput:
		      SdifF->Stream = SdiffBinOpen (SdifF->Name, 
						    eBinaryModeStdOutput);
		  break;

		  case eBinaryModeUnknown:
		      SdifF->Stream = SdiffBinOpen (SdifF->Name, 
						    eBinaryModeWrite);
		  break;

	          default:
		      _SdifFError(SdifF, eBadStdFile, SdifF->Name);
		  break;
              }
	  break;

          case ePredefinedTypes: /* special case:		  */
	      return SdifF;	 /* stream is NULL, but that's ok */

          default:
	      _SdifFError(SdifF, eBadMode, "this mode doesn't exist");
      }

      if (!SdifF->Stream)
      {
	  _SdifError(eFileNotFound, Name);
	  SdifFClose (SdifF);
	  SdifF = NULL;
      }
      else
      {   /* check if we can perform seeks (with Sdiffsetpos) on this file */
#if HAVE_SYS_STAT_H
	  SdifF->isSeekable  =  stdio == eBinaryModeUnknown
			        && !S_ISFIFO (fileno (SdifF->Stream));
#else
	  SdifF->isSeekable  =  stdio == eBinaryModeUnknown;
#endif
      }
  }
  else
  {
      _SdifError(eAllocFail, "SdifFile");
  }
      
  return SdifF;
}









SdifFileT*
SdifOpenFile(const char* Name, SdifFileModeET Mode)
{
    /* obsolete */
    _Debug("SdifOpenFile is obsolete, use SdifFOpen(SdifFileT* SdifF)");
	return SdifFOpen(Name, Mode);
}









SdifFileT*
SdifFOpenText(SdifFileT *SdifF, const char* Name, SdifFileModeET Mode)
{
  SdifF->TextStreamName = SdifCreateStrNCpy(Name, SdifStrLen(Name)+1);

  switch (Mode)
  {
	case eReadFile :
      if (SdifStrEq(Name, "stdin"))
	  {
	    SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeStdInput);
	    return SdifF;
	  }
	  else
	  {
	    if (    (SdifStrEq(Name, "stdout"))
			 || (SdifStrEq(Name, "stderr"))   )
		{
		  _SdifFError(SdifF, eBadStdFile, Name);
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

	case eWriteFile :
	  if (SdifStrEq(Name, "stdout"))
	  {
	    SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeStdOutput);
	    return SdifF;
	  }
	  else
	  {
        if (SdifStrEq(Name, "stderr"))
		{
	      SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeStdError);
	      return SdifF;
		}
		else
		{
	      if (SdifStrEq(Name, "stdin"))
		  {
		    _SdifFError(SdifF, eBadStdFile, Name);
		    return NULL;
		  }
	      else
		  {
	        SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeWrite);
	        if (! SdifF->TextStream)
			{
		      _SdifError(eAllocFail, Name);	  
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
      if (SdifStrEq(Name, "stdin"))
	  {
	    SdifF->TextStream = SdiffBinOpen (Name, eBinaryModeStdInput);
	    return SdifF;
	  }
	  else
	  {
	    if (    (SdifStrEq(Name, "stdout"))
			 || (SdifStrEq(Name, "stderr"))   )
		{
		  _SdifFError(SdifF, eBadStdFile, Name);
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
	  return NULL;
	}
}








void
SdifFClose(SdifFileT* SdifF)
{
  if (SdifF)
    {
	/* name is now part of the selection and freed with it.
	   if (SdifF->Name)          SdifFree (SdifF->Name);
	   else                     _SdifError (eFreeNull, "SdifFile->Name");
	*/
      if (SdifF->NameValues)         SdifKillNameValuesL (SdifF->NameValues);
        else                         _SdifError (eFreeNull, "SdifFile->NameValues");
      if (SdifF->MatrixTypesTable)   SdifKillHashTable (SdifF->MatrixTypesTable);
        else                         _SdifError (eFreeNull, "SdifFile->MatrixTypesTable");
      if (SdifF->FrameTypesTable)    SdifKillHashTable (SdifF->FrameTypesTable);
        else                         _SdifError (eFreeNull, "SdifFile->FrameTypesTable");
/*      if (SdifF->StreamIDsTable)     SdifKillHashTable (SdifF->StreamIDsTable);
        else                         _SdifError (eFreeNull, "SdifFile->StreamIDsTable");*/
      if (SdifF->StreamIDsTable)     SdifKillStreamIDTable (SdifF->StreamIDsTable);
        else                         _SdifError (eFreeNull, "SdifFile->StreamIDsTable");
      if (SdifF->TimePositions)      SdifKillTimePositionL (SdifF->TimePositions);
        else                         _SdifError (eFreeNull, "SdifFile->TimePositions");
      if (SdifF->Selection)          SdifFreeSelection (SdifF->Selection);
        else                         _SdifError (eFreeNull, "SdifFile->Selection");
      if (SdifF->CurrOneRow)         SdifKillOneRow(SdifF->CurrOneRow);
        else                         _SdifError (eFreeNull, "SdifFile->CurrOneRow");
      if (SdifF->Errors)             SdifKillErrorL(SdifF->Errors);
        else                         _SdifError (eFreeNull, "SdifFile->Errors");


      if (SdifF->CurrFramH)
        SdifKillFrameHeader(SdifF->CurrFramH);
      if (SdifF->CurrMtrxH)
        SdifKillMatrixHeader(SdifF->CurrMtrxH);
      if (SdifF->MtrxUsed)
	SdifKillSignatureTab(SdifF->MtrxUsed);

      if (SdifF->TextStreamName)
        SdifFree(SdifF->TextStreamName);


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



      SdifFree(SdifF);
    }
    
}







void
SdifCloseFile(SdifFileT* SdifF)
{
    /* obsolete */
    _Debug("SdifCloseFile is obsolete, use SdifFClose(SdifFileT* SdifF)");
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
     sprintf(gSdifErrorMess, "*Sdif* %c not a verbose ('t': text; 's':SdifFile)\n", Verbose);
      _Debug(gSdifErrorMess);
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
  if (SdifStrEq(TypesFileName, ""))
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






int        gSdifInitialised = 0;
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

    assert (!gSdifInitialised  &&  "SDIF library already initialised");
    gSdifInitialised = 1;

    if (SdifStdErr == NULL)
	SdifStdErr = stderr;

    SdifInitMachineType();
    SdifSetStdIOBinary (); /* only WIN32 */
    SdifInitListNodeStock(_SdifListNodeStockSize);
    SdifInitSelect ();

    gSdifPredefinedTypes = SdifFOpen("Predefined", ePredefinedTypes);

    if ( (!PredefinedTypesFile) || (strlen(PredefinedTypesFile)== 0) )
    {
	PreTypesEnvVar = XpGetenv(_SdifEnvVar);
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
SdifGenInitCond (char *pfile)
{
    if (!gSdifInitialised)
        SdifGenInit (pfile);
}




void
SdifGenKill(void)
{
  SdifFClose(gSdifPredefinedTypes);
  SdifDrainListNodeStock();

#ifdef _SdifMemoryReport
  SdifMrDrainBlockList(&SdifMrReport);
#endif

  gSdifInitialised = 0;
}



void SdifPrintVersion(void)
{
#ifndef lint
    static char rcsid[]= "$Revision: 3.13.2.1 $ IRCAM $Date: 2000-08-21 14:04:15 $";
#endif

    if (SdifStdErr == NULL)
	SdifStdErr = stderr;

    fprintf(SdifStdErr, "SDIF Library\n");
    fprintf(SdifStdErr, "Format version : %d\n", _SdifFormatVersion);

#ifndef lint
    fprintf(SdifStdErr, "CVS: %s\n", rcsid);
#endif

    fprintf(SdifStdErr, "Release: %s, %s\n", VERSION, __DATE__);
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
#if (_SdifFormatVersion > 2)

    memcpy (SdifF->CurrOneRow->Data.Void, (void *) Values, 
	    SdifSizeofDataType (SdifF->CurrOneRow->DataType) * SdifF->CurrOneRow->NbData);

#else

  switch (SdifF->CurrOneRow->DataType)
    {
    case eFloat4:
      memcpy(SdifF->CurrOneRow->Data.Float4, (char*) Values, sizeof(SdifFloat4)* SdifF->CurrOneRow->NbData);
      break;
    case eFloat8:
      memcpy(SdifF->CurrOneRow->Data.Float8, (char*) Values, sizeof(SdifFloat8)* SdifF->CurrOneRow->NbData);
      break;
    default:
      return NULL;
    }
#endif

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
#if (_SdifFormatVersion >= 3)
    return SdifF->CurrOneRow->Data.Void;
#else
  switch (SdifF->CurrOneRow->DataType)
    {
    case eFloat4:
      return SdifF->CurrOneRow->Data.Float4;
    case eFloat8:
      return SdifF->CurrOneRow->Data.Float8;
    default:
      return NULL;
    }
#endif
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

SdifDataTypeET
SdifFCurrDataType (SdifFileT *SdifF)
{
  return SdifF->CurrMtrxH->DataType;
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



/* Return list of NVTs for querying. */
SdifNameValuesLT *
SdifFNameValueList (SdifFileT *file)
{
    return (file->NameValues);
}

/* Return number of NVTs present. */
int SdifFNameValueNum (SdifFileT *file)
{
    return (SdifListGetNbData (file->NameValues->NVTList));
}


/* Add user data, return index added */
int
SdifFAddUserData (SdifFileT *file, void *data)
{
    assert (file->NbUserData < MaxUserData);
    file->UserData [file->NbUserData] = data;
    return (file->NbUserData++);
}

/* Get user data by index */
void *
SdifFGetUserData (SdifFileT *file, int index)
{
    assert (index >= 0  &&  index < file->NbUserData);
    return (file->UserData [index]);
}
