/* $Id: SdifFrame.c,v 3.2 1999-09-28 13:08:59 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * Frame Header, Frame Data structures management
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.1  1999/03/14  10:56:52  virolle
 * SdifStdErr add
 *
 */


#include <preincluded.h>
#include "SdifFrame.h"
#include "SdifFrameType.h"

#include <stdlib.h>

SdifFrameHeaderT*
SdifCreateFrameHeader(SdifSignature Signature,
		      SdifUInt4 Size,
		      SdifUInt4 NbMatrix,
		      SdifUInt4 NumID,
		      SdifFloat8 Time)
{
  SdifFrameHeaderT *NewFrameHeader = NULL;
  
  NewFrameHeader = SdifMalloc(SdifFrameHeaderT);
  if (NewFrameHeader)
    {
      NewFrameHeader->Signature = Signature;
      NewFrameHeader->Size      = Size;
      NewFrameHeader->NbMatrix  = NbMatrix;
      NewFrameHeader->NumID     = NumID;
      NewFrameHeader->Time      = Time;
     return NewFrameHeader;
    }
  else
    {
      _SdifError(eAllocFail, "FrameHeader allocation");
      return NULL;
    }
}






SdifFrameHeaderT*
SdifCreateFrameHeaderEmpty(SdifSignature Signature)
{
  return SdifCreateFrameHeader(Signature, _SdifFrameHeaderSize, 0, 0, 0.);
}







void
SdifKillFrameHeader(SdifFrameHeaderT *FrameHeader)
{
  if (FrameHeader)
    SdifFree(FrameHeader);
  else
    _SdifError(eFreeNull, "FrameHeader free");
}











SdifFrameDataT*
SdifCreateFrameData(SdifHashTableT *FrameTypesTable,
		    SdifSignature FrameSignature,
		    SdifUInt4 NumID,
		    SdifFloat8 Time)
{
  SdifFrameDataT *NewFrameData = NULL;
  SdifFrameTypeT *FrameType = NULL;
  SdifUInt4 iMtrxD;

  FrameType = SdifGetFrameType(FrameTypesTable, FrameSignature);
  if (! FrameType)
    {
      sprintf(gSdifErrorMess, "Frame Type '%s'",
	      SdifSignatureToString(FrameSignature));
      _SdifError(eNotFound, gSdifErrorMess);
      return NULL;
    }
  
  NewFrameData = SdifMalloc(SdifFrameDataT);
  if (NewFrameData)
    {
      NewFrameData->Header = SdifCreateFrameHeader(FrameSignature,
						   _SdifFrameHeaderSize,
						   0,
						   NumID,
						   Time);
      NewFrameData->Matrix_s = SdifCalloc(SdifMatrixDataT*, FrameType->NbComponent);
      for (iMtrxD=0; iMtrxD<FrameType->NbComponent; iMtrxD++)
	NewFrameData->Matrix_s[iMtrxD] = NULL;
      return NewFrameData;
    }
  else
    {
      _SdifError(eAllocFail, "FrameData allocation");
      return NULL;
    }
}






void
SdifKillFrameData(SdifHashTableT *FrameTypesTable, SdifFrameDataT *FrameData)
{
  SdifFrameTypeT *FrameType;
  unsigned int iMtrx;  
  
  FrameType = SdifGetFrameType(FrameTypesTable, FrameData->Header->Signature);
  
  if(FrameData)
    {
      if(FrameData->Matrix_s)
	{
	  for (iMtrx=0; iMtrx<FrameType->NbComponent; iMtrx++)
	    if (FrameData->Matrix_s[iMtrx] != NULL)
	      SdifKillMatrixData(FrameData->Matrix_s[iMtrx]);
	  
	  SdifFree(FrameData->Matrix_s);
	}
      else
	_SdifError(eFreeNull, "FrameData->Matrix_s free");
 
      SdifKillFrameHeader(FrameData->Header);
      SdifFree(FrameData);
    }
  else
    _SdifError(eFreeNull, "FrameData free");
}











SdifFrameDataT*
SdifFrameDataPutNthMatrixData(SdifFrameDataT *FrameData,
			      unsigned int NthMatrix,
			      SdifMatrixDataT *MatrixData)
{
  if (FrameData->Header->NbMatrix == (NthMatrix-1))
    {
      FrameData->Matrix_s[NthMatrix-1] = MatrixData;
      FrameData->Header->NbMatrix = NthMatrix;
      FrameData->Header->Size += MatrixData->Size;
    }
  else
    {
      sprintf(gSdifErrorMess,
	      "in FrameData '%s' ID:%u T:%g with Last:%d YourNth:%d",
	      SdifSignatureToString(FrameData->Header->Signature),
	      FrameData->Header->NumID,
	      FrameData->Header->Time,
	      FrameData->Header->NbMatrix,
	      NthMatrix);
      _SdifError(eAffectationOrder, gSdifErrorMess);
      return NULL;
    }
  return FrameData;
}













SdifFrameDataT*
SdifFrameDataPutComponentMatrixData(SdifHashTableT *FrameTypesTable,
				    SdifFrameDataT *FrameData,
				    char *CompoName,
				    SdifMatrixDataT *MatrixData)
{
  SdifFrameTypeT *FrameType;
  SdifComponentT *Component;

  FrameType = SdifGetFrameType(FrameTypesTable, FrameData->Header->Signature);
  if (!FrameType)
    {
      sprintf(gSdifErrorMess,
	      "Frame Type '%s'",
	      SdifSignatureToString(FrameData->Header->Signature));
      _SdifError(eNotFound, gSdifErrorMess);
      return NULL;
    }
  
  Component = SdifFrameTypeGetComponent(FrameType, CompoName);
  if ( ! Component)
    {
      sprintf(gSdifErrorMess,
	      "'%s' not a component of '%s' Frame Type",
	      CompoName, SdifSignatureToString(FrameData->Header->Signature));
      _SdifError(eNotFound, gSdifErrorMess);
      return NULL;
    }
  
  if (Component->MtrxS == MatrixData->Header->Signature)
    return SdifFrameDataPutNthMatrixData(FrameData, Component->Num,MatrixData);
  else
    {
      sprintf(gSdifErrorMess,
	      "F:%s ID:%u T:%g\n   '%s' is not matrix type of %s\n   '%s' waiting",
	      SdifSignatureToString(FrameData->Header->Signature),
	      FrameData->Header->NumID,
	      FrameData->Header->Time,
	      SdifSignatureToString(MatrixData->Header->Signature),
	      CompoName,
	      SdifSignatureToString(Component->MtrxS));
      _SdifError(eAffectationOrder, gSdifErrorMess);
      return NULL;
    }
}







SdifMatrixDataT*
SdifFrameDataGetNthMatrixData(SdifFrameDataT *FrameData,
			      unsigned int NthMatrix)
{
  if (FrameData->Header->NbMatrix >= NthMatrix)
    return FrameData->Matrix_s[NthMatrix-1];
  else
    return NULL;
}








SdifMatrixDataT*
SdifFrameDataGetComponentMatrixData(SdifHashTableT *FrameTypesTable,
				    SdifFrameDataT *FrameData,
				    char *CompoName)
{
  SdifFrameTypeT *FrameType;
  SdifComponentT *Component;

  FrameType = SdifGetFrameType(FrameTypesTable, FrameData->Header->Signature);
  if (!FrameType)
    {
      sprintf(gSdifErrorMess,
	      "'%s' Frame Type",
	      SdifSignatureToString(FrameData->Header->Signature));
      _SdifError(eNotFound, gSdifErrorMess);
      return NULL;
    }
  
  Component = SdifFrameTypeGetComponent(FrameType, CompoName);
  if ( ! Component)
    {
      sprintf(gSdifErrorMess,
	      "'%s' is not a component of '%s' Frame Type",
	      CompoName, SdifSignatureToString(FrameData->Header->Signature));
      _SdifError(eNotFound, gSdifErrorMess);
      return NULL;
    }
  
  return SdifFrameDataGetNthMatrixData(FrameData, Component->Num);
}


