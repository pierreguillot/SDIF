/* SdifFrame.c
 *
 *
 *
 */


#include "SdifFrame.h"

#include "SdifRWLowLevel.h"
#include "SdifFrameType.h"

#include <stdlib.h>
#include <string.h>







SdifFrameHeaderType*
SdifCreateFrameHeader(char *FrameName,
		      SdifUInt4 Size,
		      SdifUInt4 NbMatrix,
		      SdifUInt4 NumID,
		      SdifFloat8 Time)
{
  SdifFrameHeaderType *NewFrameHeader;
  
  if (NewFrameHeader = (SdifFrameHeaderType*) malloc (sizeof(SdifFrameHeaderType)))
    {
      SdifStrNCpy(NewFrameHeader->FrameName, FrameName, _SdifNameLen);
      NewFrameHeader->Size         = Size;
      NewFrameHeader->NbMatrix     = NbMatrix;
      NewFrameHeader->NumID        = NumID;
      NewFrameHeader->Time         = Time;
     return NewFrameHeader;
    }
  else
    {
      _SdifError(eAllocFail, "FrameHeader allocation");
      return NULL;
    }
}






SdifFrameHeaderType*
SdifCreateFrameHeaderEmpty(char *FrameName)
{
  return SdifCreateFrameHeader(FrameName, _SdifFrameHeaderSize, 0, 0, 0.);
}







void
SdifKillFrameHeader(SdifFrameHeaderType *FrameHeader)
{
  if (FrameHeader)
    free(FrameHeader);
  else
    _SdifError(eFreeNull, "FrameHeader free");
}









int
SdifFPrintFrameHeader(FILE *f, SdifFrameHeaderType* FrameHeader)
{
  int
    NbCharWrite = 0;

  NbCharWrite += SdifPrintName(f, FrameHeader->FrameName);
  NbCharWrite += fprintf(f,
			 "\t%u\t%u\t%g\n",
			 FrameHeader->NbMatrix,
			 FrameHeader->NumID,
			 FrameHeader->Time);
  
  return NbCharWrite;
}







/* FrameName read before */
void
SdifFScanFrameHeader(FILE *fr, char *FrameName, SdifFrameHeaderType* FrameHeader)
{

  SdifStrNCpy(FrameHeader->FrameName, FrameName, _SdifNameLen);
  FrameHeader->Size = _SdifUnknownSize ;
  fscanf(fr, "%u", &(FrameHeader->NbMatrix));
  fscanf(fr, "%u", &(FrameHeader->NumID));
  fscanf(fr, "%lg", &(FrameHeader->Time));
}







/* FrameName read before */
/*
int
SdifFReadFrameHeader(char *FrameName, SdifFrameHeaderType* FrameHeader, FILE *fr)
{
  int NbBytesRead = 0;

  SdifStrNCpy(FrameHeader->FrameName, FrameName, _SdifNameLen);
  NbBytesRead += sizeof(SdifUInt4)  * SdifFReadUInt4 ( &(FrameHeader->Size),     1, fr);
  NbBytesRead += sizeof(SdifUInt4)  * SdifFReadUInt4 ( &(FrameHeader->NbMatrix), 1, fr);
  NbBytesRead += sizeof(SdifUInt4)  * SdifFReadUInt4 ( &(FrameHeader->NumID),    1, fr);
  NbBytesRead += sizeof(SdifFloat8) * SdifFReadFloat8( &(FrameHeader->Time),     1, fr);

  return NbBytesRead;
}
*/	      

int
SdifFReadFrameHeader(char *FrameName, SdifFrameHeaderType* FrameHeader, FILE *fr)
{
  int NbBytesRead = 0;
  SdifFloat4 FloatTab[2];

  SdifStrNCpy(FrameHeader->FrameName, FrameName, _SdifNameLen);
  NbBytesRead += sizeof(SdifUInt4)  * SdifFReadUInt4 ( &(FrameHeader->Size), 1, fr);
  NbBytesRead += sizeof(SdifFloat4) * SdifFReadFloat4( FloatTab, 2, fr);
  NbBytesRead += sizeof(SdifFloat8) * SdifFReadFloat8( &(FrameHeader->Time), 1, fr);

  FrameHeader->NbMatrix = (SdifUInt4) FloatTab[0];
  FrameHeader->NumID    = (SdifUInt4) FloatTab[1];

  return NbBytesRead;
}


/*
int
SdifFWriteFrameHeader(SdifFrameHeaderType* FrameHeader, FILE *fw)
{
  int NbBytesWrite = 0;

  NbBytesWrite += SdifFWrite4Char(FrameHeader->FrameName, fw);
  NbBytesWrite += sizeof(SdifUInt4)  * SdifFWriteUInt4( &(FrameHeader->Size),     1, fw);
  NbBytesWrite += sizeof(SdifUInt4)  * SdifFWriteUInt4( &(FrameHeader->NbMatrix), 1, fw);
  NbBytesWrite += sizeof(SdifUInt4)  * SdifFWriteUInt4( &(FrameHeader->NumID),    1, fw);
  NbBytesWrite += sizeof(SdifFloat8) * SdifFWriteFloat8( &(FrameHeader->Time),    1, fw);

  return NbBytesWrite;
}
*/




int
SdifFWriteFrameHeader(SdifFrameHeaderType* FrameHeader, FILE *fw)
{
  int NbBytesWrite = 0;
  SdifFloat4 FloatTab[2];

  FloatTab[0] = (SdifFloat4) FrameHeader->NbMatrix;
  FloatTab[1] = (SdifFloat4) FrameHeader->NumID;

  NbBytesWrite += SdifFWrite4Char(FrameHeader->FrameName, fw);
  NbBytesWrite += sizeof(SdifUInt4)  * SdifFWriteUInt4( &(FrameHeader->Size), 1, fw);
  NbBytesWrite += sizeof(SdifFloat4) * SdifFWriteFloat4( FloatTab, 2, fw);
  NbBytesWrite += sizeof(SdifFloat8) * SdifFWriteFloat8( &(FrameHeader->Time), 1, fw);

  return NbBytesWrite;
}








SdifFrameDataType*
SdifCreateFrameData(char *FrameName,
		    SdifUInt4 NumID,
		    SdifFloat8 Time)
{
  SdifFrameDataType *NewFrameData;
  SdifFrameTypeType *FrameType;
  SdifUInt4 iMtrxD;
  
  
  FrameType = SdifGetFrameType(FrameName);
  if (! FrameType)
    {
      sprintf(gSdifErrorMess,
	      "Frame Type '%c%c%c%c' %d, %g",
	      FrameName[0],
	      FrameName[1],
	      FrameName[2],
	      FrameName[3],
	      NumID,
	      Time);
      _SdifError(eUnDefined, gSdifErrorMess);
      return NULL;
    }

  
  
  if (NewFrameData = (SdifFrameDataType*) malloc (sizeof(SdifFrameDataType)))
    {
      NewFrameData->Header = SdifCreateFrameHeader(FrameName,
						   _SdifFrameHeaderSize,
						   0,
						   NumID,
						   Time);
      NewFrameData->Matrix_s = (SdifMatrixDataType**) malloc (sizeof(SdifMatrixDataType*) 
							       * FrameType->NbComponent);
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
SdifKillFrameData(SdifFrameDataType *FrameData)
{
  SdifFrameTypeType *FrameType;
  unsigned int iMtrx;  
  
  FrameType = SdifGetFrameType(FrameData->Header->FrameName);
  
  if(FrameData)
    {
      if(FrameData->Matrix_s)
	{
	  for (iMtrx=0; iMtrx<FrameType->NbComponent; iMtrx++)
	    if (FrameData->Matrix_s[iMtrx] != NULL)
	      SdifKillMatrixData(FrameData->Matrix_s[iMtrx]);
	  
	  free(FrameData->Matrix_s);
	}
      else
	_SdifError(eFreeNull, "FrameData->Matrix_s free");
 
      SdifKillFrameHeader(FrameData->Header);
      free(FrameData);
    }
  else
    _SdifError(eFreeNull, "FrameData free");
}











SdifFrameDataType*
SdifFrameDataPutNthMatrixData(SdifFrameDataType *FrameData,
			      unsigned int NthMatrix,
			      SdifMatrixDataType *MatrixData)
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
	      "in FrameData '%c%c%c%c' ID:%u T:%g with Last:%d YourNth:%d",
	      FrameData->Header->FrameName[0],
	      FrameData->Header->FrameName[1],
	      FrameData->Header->FrameName[2],
	      FrameData->Header->FrameName[3],
	      FrameData->Header->NumID,
	      FrameData->Header->Time,
	      FrameData->Header->NbMatrix,
	      NthMatrix);
      _SdifError(eAffectationOrder, gSdifErrorMess);
      return NULL;
    }
  return FrameData;
}













SdifFrameDataType*
SdifFrameDataPutComponentMatrixData(SdifFrameDataType *FrameData,
				    char *CompoName,
				    SdifMatrixDataType *MatrixData)
{
  SdifFrameTypeType *FrameType;
  SdifComponentType *Component;

  FrameType = SdifGetFrameType(FrameData->Header->FrameName);
  if (!FrameType)
    {
      sprintf(gSdifErrorMess,
	      "'%c%c%c%c' Frame Type",
	      FrameData->Header->FrameName[0],
	      FrameData->Header->FrameName[1],
	      FrameData->Header->FrameName[2],
	      FrameData->Header->FrameName[3]);
      _SdifError(eUnDefined, gSdifErrorMess);
      return NULL;
    }
  
  Component = SdifFrameTypeGetComponent(FrameType, CompoName);
  if ( ! Component)
    {
      sprintf(gSdifErrorMess,
	      "'%s' not a component of '%c%c%c%c' Frame Type",
	      CompoName,
	      FrameData->Header->FrameName[0],
	      FrameData->Header->FrameName[1],
	      FrameData->Header->FrameName[2],
	      FrameData->Header->FrameName[3]);
      _SdifError(eUnDefined, gSdifErrorMess);
      return NULL;
    }
  
  if (SdifStrNCmp(Component->MatrixTypeName, MatrixData->Header->MatrixName, _SdifNameLen) == 0)
    return SdifFrameDataPutNthMatrixData(FrameData, Component->Num,MatrixData);
  else
    {
      sprintf(gSdifErrorMess,
	      "F:%c%c%c%c ID:%u T:%g\n   %c%c%c%c is not matrix type of %s\n   %c%c%c%c waiting",
	      FrameData->Header->FrameName[0],
	      FrameData->Header->FrameName[1],
	      FrameData->Header->FrameName[2],
	      FrameData->Header->FrameName[3],
	      FrameData->Header->NumID,
	      FrameData->Header->Time,
	      MatrixData->Header->MatrixName[0],
	      MatrixData->Header->MatrixName[1],
	      MatrixData->Header->MatrixName[2],
	      MatrixData->Header->MatrixName[3],
	      CompoName,
	      Component->MatrixTypeName[0],
	      Component->MatrixTypeName[1],
	      Component->MatrixTypeName[2],
	      Component->MatrixTypeName[3]); 
      _SdifError(eBadType, gSdifErrorMess);
      return NULL;
    }
}







SdifMatrixDataType*
SdifFrameDataGetNthMatrixData(SdifFrameDataType *FrameData,
			      unsigned int NthMatrix)
{
  if (FrameData->Header->NbMatrix >= NthMatrix)
    return FrameData->Matrix_s[NthMatrix-1];
  else
    return NULL;
}








SdifMatrixDataType*
SdifFrameDataGetComponentMatrixData(SdifFrameDataType *FrameData,
				    char *CompoName)
{
  SdifFrameTypeType *FrameType;
  SdifComponentType *Component;

  FrameType = SdifGetFrameType(FrameData->Header->FrameName);
  if (!FrameType)
    {
      sprintf(gSdifErrorMess,
	      "'%c%c%c%c' Frame Type",
	      FrameData->Header->FrameName[0],
	      FrameData->Header->FrameName[1],
	      FrameData->Header->FrameName[2],
	      FrameData->Header->FrameName[3]);
      _SdifError(eUnDefined, gSdifErrorMess);
      return NULL;
    }
  
  Component = SdifFrameTypeGetComponent(FrameType, CompoName);
  if ( ! Component)
    {
      sprintf(gSdifErrorMess,
	      "'%'y not a component of '%c%c%c%c' Frame Type",
	      CompoName,
	      FrameData->Header->FrameName[0],
	      FrameData->Header->FrameName[1],
	      FrameData->Header->FrameName[2],
	      FrameData->Header->FrameName[3]);
      _SdifError(eUnDefined, gSdifErrorMess);
      return NULL;
    }
  
  return SdifFrameDataGetNthMatrixData(FrameData, Component->Num);
}













int
SdifFWriteMatrix_s(SdifFrameDataType *FrameData, FILE *fw)
{
  int NbBytesWrite = 0;
  unsigned int iMtrxD;  

  for(iMtrxD=0; iMtrxD<FrameData->Header->NbMatrix; iMtrxD++)
    if (FrameData->Matrix_s[iMtrxD])
      NbBytesWrite += SdifFWriteMatrixData(FrameData->Matrix_s[iMtrxD], fw);
    else
      return NbBytesWrite;

  return NbBytesWrite;
}











int
SdifFWriteFrameData(SdifFrameDataType *FrameData, FILE *fw)
{
  int NbBytesWrite = 0;
  unsigned int iMtrxD;  
  
  NbBytesWrite += SdifFWriteFrameHeader(FrameData->Header, fw);
  NbBytesWrite += SdifFWriteMatrix_s(FrameData, fw);

  return NbBytesWrite;
}










int
SdifFReadFrameMatrix_s(SdifFrameHeaderType *FramH,
		       SdifFrameDataType *FramD,
		       FILE *fr)
{
  int NbBytesRead = 0;
  unsigned int iMtrxD;  
  SdifMatrixHeaderType MtrxH;
  SdifMatrixDataType *MtrxD;
  SdifFrameTypeType *FramT;
  SdifComponentType *Component;

  FramT = SdifGetFrameType(FramH->FrameName);

  for(iMtrxD=0; iMtrxD<FramH->NbMatrix; iMtrxD++)
    {
      NbBytesRead += SdifFReadMatrixHeader(&MtrxH, fr);
      Component = SdifFrameTypeGetNthComponent(FramT, iMtrxD+1);
      if (SdifStrNCmp(MtrxH.MatrixName, Component->MatrixTypeName, _SdifNameLen) == 0)
	{
	  MtrxD = SdifCreateMatrixData(MtrxH.MatrixName,
				       MtrxH.DataType,
				       MtrxH.NbRow,
				       MtrxH.NbCol);
      
	  NbBytesRead += SdifFReadMatrixRows(MtrxD,  fr);
	  NbBytesRead += SdifFReadPadding(SdifFPaddingCalculate(fr, NbBytesRead), fr);
	  SdifFrameDataPutNthMatrixData(FramD, Component->Num, MtrxD);
	}
      else
	{
	  sprintf(gSdifErrorMess,
		  "F:%c%c%c%c ID:%u T:%g\n   %dth matrix type must be %c%c%c%c and not %c%c%c%c",
		  FramH->FrameName[0],
		  FramH->FrameName[1],
		  FramH->FrameName[2],
		  FramH->FrameName[3],
		  FramH->NumID,
		  FramH->Time,
		  Component->Num,
		  Component->MatrixTypeName[0],
		  Component->MatrixTypeName[1],
		  Component->MatrixTypeName[2],
		  Component->MatrixTypeName[3],
		  MtrxH.MatrixName[0],
		  MtrxH.MatrixName[1],
		  MtrxH.MatrixName[2],
		  MtrxH.MatrixName[3]);
	  _SdifError(eBadType, gSdifErrorMess);
	  return NbBytesRead;
	}
    }
  return NbBytesRead;
}



int
SdifSkipFrameData(SdifFrameHeaderType *FramH, FILE *fr)
{
  int
    iMtrx,
    NbBytesRead = 0,
    Boo,
    NbBytesToPass;
  
  if (FramH->Size != _SdifUnknownSize)
    {
      NbBytesToPass = FramH->Size - _SdifFrameHeaderSize;
      if (fseek(fr, NbBytesToPass, SEEK_CUR) != 0)
	{
	  sprintf(gSdifErrorMess,
		  "Skip FrameData %c%c%c%c ID:%u T:%g\n",
		  FramH->FrameName[0],
		  FramH->FrameName[1],
		  FramH->FrameName[2],
		  FramH->FrameName[3],
		  FramH->NumID,
		  FramH->Time);
	  _SdifError(eEof, gSdifErrorMess);
	  return -1;
	}
      else
	return (NbBytesToPass);    
    }
  else
    {
      for (iMtrx = 0; iMtrx<FramH->NbMatrix; iMtrx++)
	{
	  if (Boo = SdifSkipMatrix(fr) == -1)
	    {
	      sprintf(gSdifErrorMess,
		      "Skip Matrix %d in FrameData %c%c%c%c ID:%u T:%g\n",
		      iMtrx,
		      FramH->FrameName[0],
		      FramH->FrameName[1],
		      FramH->FrameName[2],
		      FramH->FrameName[3],
		      FramH->NumID,
		      FramH->Time);
	      _SdifError(eEof, gSdifErrorMess);
	      return -1;
	    }
	  else
	    NbBytesRead += Boo;
	}
      return NbBytesRead;
    }
}
