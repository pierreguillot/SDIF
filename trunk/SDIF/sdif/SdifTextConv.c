/* SdifTextConv.c
 *
 *
 *
 *
 *
 */


#include "SdifTextConv.h"


#include "SdifRWLowLevel.h"
#include "SdifRWHighLevel.h"
#include "SdifFrame.h"
#include "SdifNameValue.h"
#include "SdifHash.h"

#include <string.h>







int
SdifFTextRowsConvFloat4(SdifMatrixHeaderType *MtrxH, FILE *ftext, FILE *fsdif)
{
  int
    SizeW = 0;
  SdifOneRowType
    *Row;
  SdifUInt4
    iRow;

  if ( (MtrxH->NbCol > 0) && (MtrxH->NbRow > 0) )
    {
      Row = SdifCreateOneRow(eFloat4, MtrxH->NbCol);

      for(iRow=0; iRow<MtrxH->NbRow; iRow++)
	{
	  SdifFScanFloat4(ftext, Row->Data.F4, Row->NbData);
	  SizeW += SdifFWriteOneRow(Row, fsdif);
	}
      
      SdifKillOneRow(Row);
      return SizeW;
    }
  else
    return 0; /* empty matrix */
}








int
SdifFTextRowsConvFloat8(SdifMatrixHeaderType *MtrxH, FILE *ftext, FILE *fsdif)
{
  int
    SizeW = 0;
  SdifOneRowType
    *Row;
  SdifUInt4
    iRow;
  

  if ( (MtrxH->NbCol > 0) && (MtrxH->NbRow > 0) )
    {
      Row = SdifCreateOneRow(eFloat8, MtrxH->NbCol);
      
      for(iRow=0; iRow<MtrxH->NbRow; iRow++)
	{
	  SdifFScanFloat8(ftext, Row->Data.F8, Row->NbData);
	  SizeW += SdifFWriteOneRow(Row, fsdif);
	}

      SdifKillOneRow(Row);
      
      return SizeW;
    }
  else
    return 0; /* empty matrix */
}









int
SdifFTextFrameConv(char *Name, FILE *ftext, FILE *fsdif)
{
  SdifFrameHeaderType
    FramH;
  SdifMatrixHeaderType
    MtrxH;
  SdifOneRowType
    *Row;
  SdifUInt4
    iMtrx,
    iRow;
  int
    SizeW = 0;


  SdifFScanFrameHeader(ftext, Name, &FramH);
  SizeW += SdifFWriteFrameHeader(&FramH, fsdif);
  
  for(iMtrx=0; iMtrx<FramH.NbMatrix; iMtrx++)
    {
      SdifFScanMatrixHeader(ftext, Name, &MtrxH);
      SizeW += SdifFWriteMatrixHeader(&MtrxH, fsdif);
      
      switch (MtrxH.DataType)
	{
	case eFloat4 :
	  SizeW += SdifFTextRowsConvFloat4(&MtrxH, ftext, fsdif);
	  break;
	  
	case eFloat8 :
	  SizeW += SdifFTextRowsConvFloat8(&MtrxH, ftext, fsdif);
	  break;
	  
	default :
	  sprintf(gSdifErrorMess,
		  "Data of a OneRow : 0x%x\n  F:%c%c%c%c ID:%d T:%g Mtrx_num:%d",
		  MtrxH.DataType,
		  FramH.FrameName[0],
		  FramH.FrameName[1],
		  FramH.FrameName[2],
		  FramH.FrameName[3],
		  FramH.NumID,
		  FramH.Time,
		  iMtrx);
	  _SdifError(eTypeDataNotSupported, gSdifErrorMess);
	  break;
	}
      
      SizeW += SdifFWritePadding(SdifFPaddingCalculate(fsdif, SizeW),fsdif);
    }
  
  SdifUpdateChunkSize(SizeW, fsdif);

  return SizeW;
}











int
SdifFTextFramesChunkConv(FILE *ftext, FILE *fsdif)
{
  int
    CharEnd,
    SizeW = 0,
    SizeR = 0;
  char
    Name[_SdifNameLen];
  

  SizeW = SdifFWriteFramesChunkHeader(_SdifUnknownSize, fsdif);
  
  CharEnd = SdifFGetName(Name, ftext, &SizeR);  
  while (    (SdifStrNCmp(Name, _ENDC, _SdifNameLen) != 0)
	  && (SdifStrNCmp(Name, _ENDF, _SdifNameLen) !=0)
	  && (CharEnd != eEof) )
    {
      SizeW += SdifFTextFrameConv(Name, ftext, fsdif);
      CharEnd = SdifFGetName(Name, ftext, &SizeR);  
    }
  
  
  if (CharEnd != eEof)
    if (SdifStrNCmp(Name, _ENDC, _SdifNameLen) == 0)
      SdifUpdateChunkSize(SizeW, fsdif);
    else
      _SdifError(eSyntax, "Attempt to read 'ENDC' failed");
  else
    {
      sprintf(gSdifErrorMess,
	      "Frame: '%c%c%c%c' ",
	      Name[0],
	      Name[1],
	      Name[2],
	      Name[3]);
      _SdifError(eEof, gSdifErrorMess);
    }

  
  
  return SizeW;
}









SdifUInt8
SdifFTextConv(FILE *ftext, FILE *fsdif)
{
  static SdifUInt2
    STYPpass = 0,
    SSICpass = 0;
  char
    Name[_SdifNameLen];
  int
    CharEnd,
    SizeW = 0,
    SizeR = 0; /* only here to assume the third argument of SdifFGetName */
  SdifUInt8
    UnknownFileSize,
    FileSizeW;


  _SdifUInt8Set(UnknownFileSize, (unsigned int) _SdifUnknownSize, (unsigned int) _SdifUnknownSize);
  _SdifUInt8Set(FileSizeW, _ZeroUInt4, _ZeroUInt4);
  
  

  CharEnd = SdifFGetName(Name, ftext, &SizeR);
  if (SdifStrNCmp(Name, _SDIF, _SdifNameLen) == 0)
    {

      SizeW = SdifFWriteGeneralHeader(UnknownFileSize, fsdif);
      _SdifUInt8Incr(FileSizeW, (unsigned int) SizeW);

      CharEnd = SdifFGetName(Name, ftext, &SizeR);

      while ((SdifStrNCmp(Name, _ENDF, _SdifNameLen) != 0) && (CharEnd != eEof))
	{
	  if (SdifStrNCmp(Name, _SITC, _SdifNameLen) == 0)
	    {
	      SdifNameValuesLNewHT(gSdifNameValues);
	      SdifFScanNameValueCurrHT(ftext);
	      SizeW = SdifFWriteNameValueCurrHT(fsdif);
	      _SdifUInt8Incr(FileSizeW, (unsigned int) SizeW);
      
	    }
	  else
	    {
	      if (SdifStrNCmp(Name, _STYP, _SdifNameLen) == 0)
		{
		  if (STYPpass == 0)
		    {
		      SdifFScanAllType(eUserdefined, ftext);
		      SizeW = SdifFWriteAllType(eUserdefined, fsdif);
		      _SdifUInt8Incr(FileSizeW, (unsigned int) SizeW);
		      STYPpass = 1;
		    }
		  else
		    _SdifError(eOnlyOneChunkOf, _STYP);
		}
	      else
		{
		  if (SdifStrNCmp(Name, _SSIC, _SdifNameLen) == 0)
		    {
		      if (SSICpass == 0)
			{
			  SdifFScanAllStreamID(ftext);
			  SizeW = SdifFWriteAllStreamID(fsdif);
			  _SdifUInt8Incr(FileSizeW, (unsigned int) SizeW);
			  SSICpass = 1;
			}
		      else
			_SdifError(eOnlyOneChunkOf, _SSIC);
		    }
		  else
		    {
		      if (SdifStrNCmp(Name, _SDFC, _SdifNameLen) == 0)
			{
			  SizeW = SdifFTextFramesChunkConv(ftext, fsdif);
			  _SdifUInt8Incr(FileSizeW, (unsigned int) SizeW);
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
		    }
		}
	    }
	  CharEnd = SdifFGetName(Name, ftext, &SizeR);
	}
    }
  else
    fprintf(stderr, "It isn't a sdif text file\n");
  
  SdifUpdateFileSize(FileSizeW, fsdif);

  if (CharEnd == eEof)
    _SdifError(eSyntax, "Attempt to read 'ENDF' failed");

  return FileSizeW;
}









SdifUInt8
SdifTextConv(char *NameFText, char *NameFSdif)
{
  FILE
    *ftext,
    *fsdif;
  SdifUInt8
    FileSizeW;
  



  _SdifUInt8Set(FileSizeW, _ZeroUInt4, _ZeroUInt4);



  if (SdifStrCmp(NameFSdif, "stdout")!=0)
    {
      fsdif = fopen(NameFSdif, "w");
      
      if (SdifStrCmp(NameFText, "stdin")!=0)
	{
	  if (ftext = fopen(NameFText, "r"))
	    {
	      _SdifUInt8Cpy(FileSizeW, SdifFTextConv(ftext, fsdif));
	      fflush(fsdif);
	      
	      fclose(ftext);
	    }
	  else
	    _SdifError(eFileNotFound, NameFText);
	}
      else
	{
	  _SdifUInt8Cpy(FileSizeW, SdifFTextConv(stdin, fsdif));  
	  fflush(fsdif);
	}
      
      fclose(fsdif);
    }
  else
    {
      if (SdifStrCmp(NameFText, "stdin")!=0)
	{
	  if (ftext = fopen(NameFText, "r"))
	    {
	      _SdifUInt8Cpy(FileSizeW, SdifFTextConv(ftext, stdout));
	      fflush(stdout);
	      
	      fclose(ftext);
	    }
	  else
	    _SdifError(eFileNotFound, NameFText);
	}
      else
	{
	  _SdifUInt8Cpy(FileSizeW, SdifFTextConv(stdin, stdout));
	  fflush(stdout);
	}
    }


  return FileSizeW;
}
