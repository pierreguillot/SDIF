/* $Id: SdifFPut.c,v 3.7 2000-07-18 15:08:29 tisseran Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * F : SdifFileT* SdifF, Put : write ascii frames into pseudo-sdif text file,
 * SdifF->TextStream or sdif file SdifF->Stream.
 * switch 'verbose'
 *
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.6  2000/07/06  19:01:46  tisseran
 * Add function for frame and matrix type declaration
 * Remove string size limitation for NameValueTable
 * TODO: 1TYP and 1IDS frame must contain an 1NVT (text) matrix
 *       Actually, data are written with fprintf.
 *
 * Revision 3.5  2000/05/12  14:41:45  schwarz
 * On behalf of Adrien, synchronisation with Mac sources, with some slight
 * changes because of cross-platform issues:
 * - Mac only stuff: XpSetFileAttribute XpFileSize
 * - Cross platform wrapper: XpGetenv XpExit
 * - Dangerous: strings.h (and thus bzero, bcopy) is not ANSI and thus doesn't
 *   exist on Mac.  Use string.h and memset, memcpy.
 *
 * Revision 3.4  2000/05/04  15:03:58  schwarz
 * Avoid strlen warning.
 *
 * Revision 3.3  2000/04/11  14:31:21  schwarz
 * Read/write NVT as frame with 1 text matrix, conforming to SDIF spec.
 *
 * Revision 3.2  1999/09/28  13:08:53  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:56:41  virolle
 * SdifStdErr add
 *
 *
 */


#include <preincluded.h>
#include <string.h>
#include "SdifFPut.h"

#include "SdifTest.h"
#include "SdifFile.h"

#include "SdifRWLowLevel.h"

#include "SdifList.h"
#include "SdifHash.h"

#include "SdifString.h"





size_t
SdifFPutOneNameValue(SdifFileT* SdifF, int Verbose, SdifNameValueT *NameValue)
{
  size_t  SizeW = 0;
  FILE   *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
  
  SizeW += fprintf(file, "%s\t", NameValue->Name);
  SizeW += fprintf(file, "%s;\n", NameValue->Value);

  return  SizeW;
}




size_t
SdifFPutNameValueLCurrNVT (SdifFileT *SdifF, int Verbose)
/* SdifFPutNameValueLCurrNVT doesn't write frame header and padding */
{
  size_t          SizeW = 0;
  SdifUInt4       iNV;
  SdifHashNT     *pNV;
  SdifHashTableT *HTable;
  FILE           *file;


  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  HTable = SdifF->NameValues->CurrNVT->NVHT;
  
  SizeW += fprintf(file, "{\n");
  for(iNV=0; iNV<HTable->HashSize; iNV++)
    for (pNV = HTable->Table[iNV]; pNV; pNV = pNV->Next)
      SizeW += SdifFPutOneNameValue(SdifF, Verbose, pNV->Data);
  
  SizeW += fprintf(file, "}");  

  return SizeW;
}



char *
SdifFNameValueLCurrNVTtoString (SdifFileT *SdifF)
{
  SdifUInt4       iNV;
  SdifHashNT     *pNV;
  SdifHashTableT *HTable;

  char           *tmpStr; /* To conserve value if the reallocation
			     is impossible */
  SdifStringT *SdifString; /* Structure used for memory reallocation */
  int result;
  
  /* Memory allocation */
  SdifString = SdifStringNew();

  HTable = SdifF->NameValues->CurrNVT->NVHT;
  
  for(iNV=0; iNV<HTable->HashSize; iNV++)
    for (pNV = HTable->Table[iNV]; pNV; pNV = pNV->Next)
      {
	SdifNameValueT *NameValue = pNV->Data;
	result = SdifStringAppend(SdifString, NameValue->Name);
	result *= SdifStringAppend(SdifString, "\t");
	result *= SdifStringAppend(SdifString, NameValue->Value);
	result *= SdifStringAppend(SdifString, "\n");	
      }

  tmpStr = strdup(SdifString->str);
  
  /* ERROR TRAITEMENT MUST BE VERIFIED !!!!! */
  if (!tmpStr)
      fprintf(stderr,"No more memory avaluable!!\n");

  SdifStringFree(SdifString);
  
  return tmpStr;
}


/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
  This function put NameValue to SdifString
*/
int
SdifFNameValueLCurrNVTtoSdifString (SdifFileT *SdifF, SdifStringT *SdifString)
{
  SdifUInt4       iNV;
  SdifHashNT     *pNV;
  SdifHashTableT *HTable;
  int result;
  
  HTable = SdifF->NameValues->CurrNVT->NVHT;
  
  for(iNV=0; iNV<HTable->HashSize; iNV++)
    for (pNV = HTable->Table[iNV]; pNV; pNV = pNV->Next)
      {
	SdifNameValueT *NameValue = pNV->Data;
	result = SdifStringAppend(SdifString, NameValue->Name);
	result *= SdifStringAppend(SdifString, "\t");
	result *= SdifStringAppend(SdifString, NameValue->Value);
	result *= SdifStringAppend(SdifString, "\n");	
      }

  return result;
}



size_t
SdifFPutOneMatrixType(SdifFileT* SdifF, int Verbose, SdifMatrixTypeT *MatrixType)
{
  SdifColumnDefT    *ColumnDef;
  size_t            SizeW = 0;
  FILE              *file;
  
  
  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  if (! SdifListIsEmpty(MatrixType->ColumnUserList))
    {
      SizeW += fprintf(file, "  %s\t", SdifSignatureToString(e1MTD));
      SizeW += sizeof(SdifSignature) * SdiffWriteSignature(&(MatrixType->Signature), file);
      SizeW += fprintf(file, "\t{");

      ColumnDef = SdifListGetHead(MatrixType->ColumnUserList); /* Reinit GetNext */
	  SizeW += fprintf(file, "%s",ColumnDef->Name);

      while (SdifListIsNext(MatrixType->ColumnUserList))
      {
          ColumnDef = SdifListGetNext(MatrixType->ColumnUserList);
	      SizeW += fprintf(file, ", %s",ColumnDef->Name);
      }
      SizeW += fprintf(file, "}\n");  
    }

  return SizeW;
}


/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
  Append matrix type declaration in SdifString
*/
int SdifFOneMatrixTypeToSdifString(SdifMatrixTypeT *MatrixType, SdifStringT *SdifString)
{
  int success = 1;
  SdifColumnDefT    *ColumnDef;
  
  if (! SdifListIsEmpty(MatrixType->ColumnUserList))
    {
      success *= SdifStringAppend(SdifString, "  ");
      success *= SdifStringAppend(SdifString,SdifSignatureToString(e1MTD));
      success *= SdifStringAppend(SdifString, "  ");
      success *= SdifStringAppend(SdifString,SdifSignatureToString(MatrixType->Signature));
      success *= SdifStringAppend(SdifString,"\t{");
      
      ColumnDef = SdifListGetHead(MatrixType->ColumnUserList); /* Reinit GetNext */

      success *= SdifStringAppend(SdifString,ColumnDef->Name);
      
      while (SdifListIsNext(MatrixType->ColumnUserList))
	{
	  ColumnDef = SdifListGetNext(MatrixType->ColumnUserList);
	  success *= SdifStringAppend(SdifString,", ");
	  success *= SdifStringAppend(SdifString,ColumnDef->Name);
	}
      success *= SdifStringAppend(SdifString,"}\n");
    }  
  return(success);
}



size_t
SdifFPutAllMatrixType(SdifFileT* SdifF, int Verbose)
{
  unsigned int   iName;
  SdifHashNT    *pName;
  size_t         SizeW = 0;

  for(iName=0; iName<SdifF->MatrixTypesTable->HashSize; iName++)
    for (pName = SdifF->MatrixTypesTable->Table[iName]; pName;  pName=pName->Next)
      SizeW += SdifFPutOneMatrixType(SdifF, Verbose, pName->Data);
  
  return SizeW;
}



/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
  Write all Matrix type in SdifString
*/
int SdifFAllMatrixTypeToSdifString(SdifFileT *SdifF, SdifStringT *SdifString)
{
  unsigned int iName;
  SdifHashNT *pName;
  int success = 1;
  
  for(iName=0;iName<SdifF->MatrixTypesTable->HashSize; iName++)
    for(pName=SdifF->MatrixTypesTable->Table[iName]; pName; 
	pName = pName->Next)
      {
	success *= SdifFOneMatrixTypeToSdifString(pName->Data, SdifString);
      }
  return success;
}



size_t
SdifFPutOneComponent(SdifFileT *SdifF, int Verbose, SdifComponentT *Component)
{
  size_t   SizeW = 0;
  FILE    *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  SizeW += fprintf(file, "\t  ");
  SizeW += sizeof(SdifSignature) * SdiffWriteSignature(&(Component->MtrxS), file);
  SizeW += fprintf(file, "\t%s;\n", Component->Name);
  
  return SizeW;
}


/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
  Append one Component to SdifString
*/
int
SdifFOneComponentToSdifString(SdifComponentT *Component, SdifStringT *SdifString)
{
  int success = 1;

  success *= SdifStringAppend(SdifString, "\t  ");
  success *= SdifStringAppend(SdifString, SdifSignatureToString(Component->MtrxS));
  success *= SdifStringAppend(SdifString, "\t");
  success *= SdifStringAppend(SdifString, Component->Name);
  success *= SdifStringAppend(SdifString, ";\n");
  
  return success;
}



size_t
SdifFPutOneFrameType(SdifFileT *SdifF, int Verbose, SdifFrameTypeT *FrameType)
{
  SdifUInt4 iC;
  size_t    SizeW = 0;
  FILE      *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
  
  if (FrameType->NbComponentUse > 0)
    {
      SizeW += fprintf(file, "  %s\t", SdifSignatureToString(e1FTD));
      SizeW += sizeof(SdifSignature) * SdiffWriteSignature( &(FrameType->Signature), file);
      SizeW += fprintf(file, "\n\t{\n");
      for(iC = FrameType->NbComponent - FrameType->NbComponentUse + 1;
          iC<= FrameType->NbComponent;
          iC++)
	    {
	      SizeW += SdifFPutOneComponent(SdifF, Verbose,
                   SdifFrameTypeGetNthComponent(FrameType, iC));
	    }
      SizeW += fprintf(file, "\t}\n");
    }

  return SizeW;
}


/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
  Append one frame type to SdifString
*/
int
SdifFOneFrameTypeToSdifString(SdifFrameTypeT *FrameType, SdifStringT *SdifString)
{
  SdifUInt4 iC;
  int success = 1; /* success of SdifStringAppend */

  if (FrameType->NbComponentUse > 0)
    {
      success *= SdifStringAppend(SdifString, "  ");
      success *= SdifStringAppend(SdifString, SdifSignatureToString(e1FTD));
      success *= SdifStringAppend(SdifString, "  ");
      success *= SdifStringAppend(SdifString, SdifSignatureToString(FrameType->Signature));
      success *= SdifStringAppend(SdifString, "\n\t{\n");
      
      for(iC = FrameType->NbComponent - FrameType->NbComponentUse + 1;
          iC<= FrameType->NbComponent;
          iC++)
	    {
	      success *= SdifFOneComponentToSdifString(
			      SdifFrameTypeGetNthComponent(FrameType, iC),
			      SdifString);
	    }
      success *= SdifStringAppend(SdifString,"\t}\n");
    }

  return success;      
}



size_t
SdifFPutAllFrameType(SdifFileT *SdifF, int Verbose)
{
  unsigned int  iName;
  SdifHashNT   *pName;
  size_t        SizeW = 0;

  for(iName=0; iName<SdifF->FrameTypesTable->HashSize; iName++)
    for (pName = SdifF->FrameTypesTable->Table[iName]; pName;  pName=pName->Next)
      SizeW += SdifFPutOneFrameType(SdifF, Verbose, pName->Data);

  return SizeW;
}


/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
  Write all frame type in SdifString
*/
int
SdifFAllFrameTypeToSdifString(SdifFileT *SdifF, SdifStringT *SdifString)
{
  unsigned int  iName;
  SdifHashNT   *pName;
  int           success = 0;

  for(iName=0; iName<SdifF->FrameTypesTable->HashSize; iName++)
    for (pName = SdifF->FrameTypesTable->Table[iName]; pName;  pName=pName->Next)
      success *= SdifFOneFrameTypeToSdifString(pName->Data, SdifString);

  return success;
}  



/* SdifFPutAllType writes no '1TYP', no chunk size and no padding too */
size_t
SdifFPutAllType(SdifFileT *SdifF, int Verbose)
{
  size_t  SizeW = 0;
  FILE   *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
 
  SizeW += fprintf(file, "{\n");  
  SizeW += SdifFPutAllMatrixType(SdifF, Verbose);
  SizeW += SdifFPutAllFrameType(SdifF, Verbose);  
  SizeW += fprintf(file, "}");

  return SizeW;
}




size_t
SdifFPutOneStreamID(SdifFileT *SdifF, int Verbose, SdifStreamIDT *StreamID)
{
  size_t   SizeW = 0;
  FILE    *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  SizeW += fprintf(file, "  %u ", StreamID->NumID);
  SizeW += fprintf(file, "%s:", StreamID->Source);
  SizeW += fprintf(file, "%s;\n", StreamID->TreeWay);

  return  SizeW;
}

/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
  Append one StreamID to SdifString
*/
int
SdifFOneStreamIDToSdifString(SdifStringT *SdifString,
				  SdifStreamIDT *StreamID)
{
  int success = 1;
  char *tmpStr;
  int SizeW = 0;
  
  success *= SdifStringAppend(SdifString," ");
  
  /* Add StreamID->NumID  (unsigned int) */
  /* We use a temporary string for conversion in unsigned int */
  /* Memory allocation */
  tmpStr = (char *) malloc(_SdifStringGranule*sizeof(char));
  SizeW = sprintf(tmpStr,"%u",StreamID->NumID);
  success *=  SdifStringAppend(SdifString,tmpStr);
  free(tmpStr);
  
  success *= SdifStringAppend(SdifString," ");
  success *= SdifStringAppend(SdifString,StreamID->Source);
  success *= SdifStringAppend(SdifString,":");
  success *= SdifStringAppend(SdifString,StreamID->TreeWay);
  success *= SdifStringAppend(SdifString,";");
  success *= SdifStringAppend(SdifString,"\n");

  return success;
}

/* SdifFPutAllStreamID doesn't write '1IDS', chunk size and padding */
size_t
SdifFPutAllStreamID(SdifFileT *SdifF, int Verbose)
{
  unsigned int     iID;
  SdifHashNT *pID;
  size_t           SizeW = 0;
  FILE             *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  SizeW += fprintf(file, "{\n");
  
  for(iID=0; iID<SdifF->StreamIDsTable->SIDHT->HashSize; iID++)
    for (pID = SdifF->StreamIDsTable->SIDHT->Table[iID]; pID; pID = pID->Next)
      SizeW += SdifFPutOneStreamID(SdifF, Verbose, pID->Data);
  
  SizeW += fprintf(file, "}");

  return SizeW;
}

/*DOC:
  Remark:
         This function implements the new SDIF Specification (June 1999):
	 Name Value Table, Matrix and Frame Type declaration, Stream ID declaration are
	 defined in text matrix:
	 1NVT 1NVT
	 1TYP 1TYP
	 1IDS 1IDS
  Write all StreamID in SdifString
*/
int
SdifFAllStreamIDToSdifString(SdifFileT *SdifF, SdifStringT *SdifString)
{
  unsigned int iID;
  SdifHashNT   *pID;
  int          success;
  
  success = 1;
  
  for (iID = 0; iID<SdifF->StreamIDsTable->SIDHT->HashSize; iID++)
    for (pID = SdifF->StreamIDsTable->SIDHT->Table[iID]; pID; pID = pID->Next)
      success *= SdifFOneStreamIDToSdifString(SdifString, pID->Data);

  return success;
}


/*
 * obsolete
 */

size_t
SdifFPutNameValueCurrHT (SdifFileT *SdifF, int Verbose)
{
    /* obsolete */
    _Debug("SdifFPutNameValueCurrHT is obsolete, use SdifFPutNameValueLCurrNVT");
    return SdifFPutNameValueLCurrNVT(SdifF, Verbose);
}







