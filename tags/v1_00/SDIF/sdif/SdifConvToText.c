/* SdifConvToText.c
 *
 *
 *
 *
 */

#include "SdifConvToText.h"


#include "SdifRWLowLevel.h"
#include "SdifRWHighLevel.h"
#include "SdifFrame.h"
#include "SdifNameValue.h"
#include "SdifHash.h"


#include <string.h>



int
SdifFConvRowsFloat4ToText(SdifMatrixHeaderType *MtrxH, FILE *fsdif, FILE *ftext)
{
  int
    SizeR = 0;
  SdifOneRowType
    *Row;
  SdifUInt4
    iRow;
  
  if ( (MtrxH->NbCol > 0) && (MtrxH->NbRow > 0) )
    {
      Row = SdifCreateOneRow(eFloat4, MtrxH->NbCol);
      
      for(iRow=0; iRow<MtrxH->NbRow; iRow++)
	{
	  SizeR += SdifFReadOneRow(Row, fsdif);
	  SdifFPrintOneRow(ftext, Row);
	}
      
      SdifKillOneRow(Row);
      
      SizeR += SdifFReadPadding(SdifFPaddingCalculate(fsdif, SizeR), fsdif);
      return SizeR;
    }
  else
    return 0; /* empty matrix */
}










int
SdifFConvRowsFloat8ToText(SdifMatrixHeaderType *MtrxH, FILE *fsdif, FILE *ftext)
{
  int
    SizeR = 0;
  SdifOneRowType
    *Row;
  SdifUInt4
    iRow;
  
  
  if ( (MtrxH->NbCol > 0) && (MtrxH->NbRow > 0) )
    {
      Row = SdifCreateOneRow(eFloat8, MtrxH->NbCol);
      
      for(iRow=0; iRow<MtrxH->NbRow; iRow++)
	{
	  SizeR += SdifFReadOneRow(Row, fsdif);
	  SdifFPrintOneRow(ftext, Row);
	}
      
      SdifKillOneRow(Row);
      
      SizeR += SdifFReadPadding(SdifFPaddingCalculate(fsdif, SizeR), fsdif);
      
      return SizeR;
    }
  else
    return 0; /* empty matrix */
}









int
SdifFConvFrameToText(char *Name, FILE *fsdif, FILE *ftext)
{
  SdifFrameHeaderType
    FramH;
  SdifMatrixHeaderType
    MtrxH;
  int
    iMtrx,
    SizeR = 0;


  SizeR += SdifFReadFrameHeader(Name, &FramH, fsdif);
  SdifFPrintFrameHeader(ftext, &FramH);
  
  for(iMtrx=0; iMtrx<FramH.NbMatrix; iMtrx++)
    {
      SizeR += SdifFReadMatrixHeader(&MtrxH, fsdif);
      SdifFPrintMatrixHeader(ftext, &MtrxH);
  
      switch (MtrxH.DataType)
	{
	case eFloat4 :
	  SizeR += SdifFConvRowsFloat4ToText(&MtrxH, fsdif, ftext);
	  break;
	  
	case eFloat8 :
	  SizeR += SdifFConvRowsFloat8ToText(&MtrxH, fsdif, ftext);
	  break;
	  
	default :
	  sprintf(gSdifErrorMess,
		  "Data of a OneRow : 0x%x\n  F:%c%c%c%c ID:%d T:%g Mtrx_num:%d, at %d",
		  MtrxH.DataType,
		  FramH.FrameName[0],
		  FramH.FrameName[1],
		  FramH.FrameName[2],
		  FramH.FrameName[3],
		  FramH.NumID,
		  FramH.Time,
		  iMtrx,
		  ftell(fsdif));
	  _SdifError(eTypeDataNotSupported, gSdifErrorMess);
	  break;
	}
    }
  fprintf(ftext,"\n");
  return SizeR;
}











int
SdifFConvFramesChunkToText(FILE *fsdif, FILE *ftext)
{
  int
    CharEnd,
    SizeR = 0;
  unsigned int
    ChunkSize = 0;
  char
    Name[_SdifNameLen];


  SizeR += sizeof(SdifUInt4) * SdifFReadUInt4(&ChunkSize, 1, fsdif);
  
  while(    (SizeR < ChunkSize-_SdifNameLen)
         && (CharEnd = SdifFGetName(Name, fsdif, &SizeR) != eEof) )
    {      
      if (    (SdifStrNCmp(Name, _STYP, _SdifNameLen)==0)
	   || (SdifStrNCmp(Name, _SSIC, _SdifNameLen)==0)
	   || (SdifStrNCmp(Name, _SITC, _SdifNameLen)==0)
	   || (SdifStrNCmp(Name, _SDFC, _SdifNameLen)==0) )
	{
	  fseek(fsdif, -(_SdifNameLen), SEEK_CUR);
	  return SizeR-_SdifNameLen;
	}
      else    
	SizeR += SdifFConvFrameToText(Name, fsdif, ftext);
    }

  return SizeR;
}





static SdifUInt2 STYPpass;
static SdifUInt2 SSICpass;



int
SdifFConvChunkToText(char *Name, FILE *fsdif, FILE *ftext)
{
  int
    SizeR = 0;

  if (SdifStrNCmp(Name, _STYP, _SdifNameLen)==0)
    {
      if (STYPpass == 0)
	{
	  SizeR += SdifFReadAllType(eUserdefined, fsdif);
	  SdifFPrintAllType(ftext, eUserdefined);
	  STYPpass = 1;
	}
      else
	_SdifError(eOnlyOneChunkOf, _STYP);
    }
  else
    
    if (SdifStrNCmp(Name, _SSIC, _SdifNameLen)==0)
      {
	if (SSICpass == 0)
	  {
	    SizeR += SdifFReadAllStreamID(fsdif);
	    SdifFPrintAllStreamID(ftext);
	    SSICpass = 1;
	  }
	else
	  _SdifError(eOnlyOneChunkOf, _SSIC);
      }
    else
      
      if  (SdifStrNCmp(Name, _SITC, _SdifNameLen)==0)
	{
	  SdifNameValuesLNewHT(gSdifNameValues);
	  SizeR += SdifFReadNameValueCurrHT(fsdif);
	  SdifFPrintNameValueCurrHT(ftext);
	}
      else
	
	if  (SdifStrNCmp(Name, _SDFC, _SdifNameLen)==0)      
	  {
	    fprintf(ftext, "%s\n\n", _SDFC);
	    SizeR += SdifFConvFramesChunkToText(fsdif, ftext);
	    fprintf(ftext, "%s\n\n", _ENDC);
	    fflush(ftext);
	  }
	else
	  {
	    sprintf(gSdifErrorMess,
		    "It is not a chunk name : '%c%c%c%c'",
		    Name[0],
		    Name[1],
		    Name[2],
		    Name[3]);
	    _SdifError(eSyntax, gSdifErrorMess);
	  }
  
  
  return SizeR;
}





SdifUInt8
SdifFConvToText(FILE *fsdif, FILE *ftext)
{
  char
    Name[_SdifNameLen];
  int
    CharEnd,
    SizeW = 0,
    SizeR = 0;
  SdifUInt8
    SizeR8,
    TotalSize;



  _SdifUInt8Set(TotalSize, _ZeroUInt4, _ZeroUInt4);
  _SdifUInt8Set(SizeR8, _ZeroUInt4, _ZeroUInt4);

  

  _SdifUInt8Incr(SizeR8, SdifFReadGeneralHeader(&TotalSize, fsdif));
  fprintf(ftext, "%s\n\n", _SDIF);

  SizeR = 0;
  while ( (_SdifUInt8Inf(SizeR8, TotalSize)) && (CharEnd = SdifFGetName(Name, fsdif, &SizeR)!=eEof) )
    {
      _SdifUInt8Incr(SizeR8, SizeR);
      SizeR = 0;
      _SdifUInt8Incr(SizeR8, SdifFConvChunkToText(Name, fsdif, ftext));
    }

  fprintf(ftext, "%s\n\n", _ENDF);
  return SizeR8;
}









SdifUInt8
SdifConvToText(char *NameFSdif, char *NameFText)
{
  FILE
    *fsdif,
    *ftext;
  SdifUInt8
    FileSizeR;
  


  _SdifUInt8Set(FileSizeR, _ZeroUInt4, _ZeroUInt4);
  
  STYPpass = 0;
  SSICpass = 0;
  
  
  if (SdifStrCmp(NameFText, "stdout")!=0)
    {
      ftext = fopen(NameFText, "w");
      
      if (SdifStrCmp(NameFSdif, "stdin")!=0)
	{
	  if (fsdif = fopen(NameFSdif, "r"))
	    {
	      _SdifUInt8Cpy(FileSizeR, SdifFConvToText(fsdif, ftext));
	      fflush(ftext);
	      
	      fclose(fsdif);
	    }
	  else
	    _SdifError(eFileNotFound, NameFSdif);	    
	}
      else
	{
	  _SdifUInt8Cpy(FileSizeR, SdifFConvToText(stdin, ftext));
	  fflush(ftext);
	}
      
      fclose(ftext);
    }
  else
    {
      if (SdifStrCmp(NameFSdif, "stdin")!=0)
	{
	  if (fsdif = fopen(NameFSdif, "r"))
	    {
	      _SdifUInt8Cpy(FileSizeR, SdifFConvToText(fsdif, stdout));
	      fflush(stdout);
	      
	      fclose(fsdif);
	    }
	  else
	    _SdifError(eFileNotFound, NameFSdif);
	}
      else
	{
	  _SdifUInt8Cpy(FileSizeR, SdifFConvToText(stdin, stdout));  
	  fflush(stdout);
	}
    }
  
  return FileSizeR;
}
