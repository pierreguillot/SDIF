/* SdifRWHighLevel.c
 *
 *
 *
 */

#include "SdifRWHighLevel.h"
#include "SdifRWLowLevel.h"

#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifNameValue.h"

#include <string.h>
#include <ctype.h>








void
/*SdifUpdateFileSize(SdifInt4 FileSize, FILE *fw)*/
SdifUpdateFileSize(SdifUInt8 FileSize, FILE *fw)
{
  SdifInt4 CurrPos;

  if (fw != stdout)
    {
      CurrPos = ftell(fw);
      fseek(fw, 4, SEEK_SET);
      /*      SdifFWriteInt4(&FileSize, 1, fw);*/
      SdifFWriteUInt8(&FileSize, 1, fw);
      fseek(fw, CurrPos, SEEK_SET);
    }     
}






void
SdifUpdateChunkSize(SdifInt4 Size, FILE *fw)
{
  SdifInt4 CurrPos;
  
  if (fw != stdout)
    {
      CurrPos = ftell(fw);
      fseek(fw, _SdifNameLen - Size, SEEK_CUR);
      SdifFWriteInt4(&Size, 1, fw);
      fseek(fw, CurrPos, SEEK_SET);
    }
}







int
/*SdifFWriteGeneralHeader(SdifInt4 FileSize, FILE *fw)*/
SdifFWriteGeneralHeader(SdifUInt8 FileSize, FILE *fw)
{
  int NbBytesWrite = 0;
  
  NbBytesWrite += SdifFWrite4Char ( _FORM , fw);
  /*  NbBytesWrite += sizeof(SdifInt4) * SdifFWriteInt4(&FileSize, 1, fw);*/
  NbBytesWrite += sizeof(SdifUInt8) * SdifFWriteUInt8(&FileSize, 1, fw);
  NbBytesWrite += SdifFWrite4Char ( _SDIF , fw);

  return NbBytesWrite;
}







int 
/*SdifFReadGeneralHeader(SdifInt4 *oFileSize, FILE *fr)*/
SdifFReadGeneralHeader(SdifUInt8 *oFileSize, FILE *fr)
{
  char Name[_SdifNameLen];
  int NbBytesRead = 0;
  
  SdifFGetName(Name, fr, &NbBytesRead);
  if (SdifStrNCmp(Name, _FORM, _SdifNameLen) != 0)
      _SdifError(eBadHeader, "FORM not correctly read");

  /*  NbBytesRead += sizeof(SdifInt4) * SdifFReadInt4(oFileSize, 1, fr);*/
  NbBytesRead += sizeof(SdifUInt8) * SdifFReadUInt8(oFileSize, 1, fr);

  SdifFGetName(Name, fr, &NbBytesRead);
  if (SdifStrNCmp(Name, _SDIF, _SdifNameLen) != 0)
    _SdifError(eBadHeader, "SDIF not correctly read");
      
  return NbBytesRead;
}











int
SdifFWriteAllType(SdifPredefinedEnum Predefined, FILE *fw)
{
  int
    NbBytesWrite = 0;
  SdifUInt4
    UnknownSize;
  
  UnknownSize = _SdifUnknownSize;
  
  NbBytesWrite += fprintf(fw, "%s", _STYP);
  NbBytesWrite += sizeof(SdifUInt4) * SdifFWriteUInt4(&UnknownSize, 1, fw);
  NbBytesWrite += fprintf(fw, "\n{\n");  
  NbBytesWrite += SdifFWriteAllMatrixType(Predefined, fw);
  NbBytesWrite += SdifFWriteAllFrameType(Predefined, fw);  
  NbBytesWrite += fprintf(fw, "}");
  
  NbBytesWrite += SdifFWritePadding(SdifFPaddingCalculate(fw, NbBytesWrite), fw);

  SdifUpdateChunkSize(NbBytesWrite, fw);

  return NbBytesWrite;
}






int
SdifFPrintAllType(FILE *fw, SdifPredefinedEnum Predefined)
{
  int
    NbCharWrite = 0;
 
  
  NbCharWrite += fprintf(fw, "%s", _STYP);
  NbCharWrite += fprintf(fw, "\n{\n");  
  NbCharWrite += SdifFWriteAllMatrixType(Predefined, fw);
  NbCharWrite += SdifFWriteAllFrameType(Predefined, fw);  
  NbCharWrite += fprintf(fw, "}\n\n");
  
  return NbCharWrite;
}




/* SdifFReadAllType ne lit pas "STYP" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "STYP"
 */
int
SdifFReadAllType(SdifPredefinedEnum Predefined, FILE *fr)
{
  int
    CharEnd,
    NbBytesRead = 0;
  char
    TypeOfType[_SdifNameLen];
  SdifInt4
    FSize;



  NbBytesRead += sizeof(SdifInt4) * SdifFReadInt4(&FSize, 1, fr);


  CharEnd = SdifFGetStringUntil(gSdifString,
				_SdifStringLen,
				fr,
				&NbBytesRead,
				_SdifReservedChars);
  if ( (CharEnd != (int) '{') || (SdifStrLen(gSdifString)!=0) )
    {
      sprintf(gSdifErrorMess, "Attempt to read '{' failed : '%s%c'", gSdifString, CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return NbBytesRead;
    }
  
  while( (CharEnd = SdifFGetName(TypeOfType, fr, &NbBytesRead)) != (unsigned) '}' )
    {
      if ( (SdifIsAReservedChar((char) CharEnd) != -1) || (isspace((char) CharEnd)) )
	{
	  sprintf(gSdifErrorMess,
		  "Frame Name not correctly read : '%c%c%c%c'. Last char read : '%d'",
		  TypeOfType[0],
		  TypeOfType[1],
		  TypeOfType[2],
		  TypeOfType[3],
		  (char) CharEnd);
	  _SdifError(eNameLength, gSdifErrorMess);
	  return NbBytesRead;
	}
      
      SdifStrNCpy(gSdifString, TypeOfType, _SdifNameLen);
      gSdifString[_SdifNameLen] = '\0';
      
      if (SdifStrNCmp (gSdifString, _Mtrx, _SdifNameLen+1) == 0)
	{
	  NbBytesRead += SdifFReadMatrixType(Predefined, fr);
	}
      else
	if (SdifStrCmp (gSdifString, "Fram") == 0)
	  {
	    NbBytesRead += SdifFReadFrameType(Predefined, fr);
	  }
	else
	  {
	    _SdifError(eSyntax, gSdifString);
	    return NbBytesRead;
	  }
    }
  
  if (TypeOfType[0] != '\0')
    {
      _SdifError(eSyntax, gSdifString);
      return NbBytesRead;
    }

  NbBytesRead += SdifFReadPadding(SdifFPaddingCalculate(fr, NbBytesRead + _SdifNameLen), fr);

  if ((NbBytesRead != FSize - _SdifNameLen) && ((unsigned) FSize != (unsigned) _SdifUnknownSize))
    fprintf(stderr,
	    "erreur size STYP: lu: %u  Attendu: %u\n",
	    NbBytesRead + _SdifNameLen,
	    FSize);

  return NbBytesRead;
}






/* SdifFScanAllType (ASCII version : pas de size, pas de padding)
 * ne lit pas "STYP" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "STYP"
 */
int
SdifFScanAllType(SdifPredefinedEnum Predefined, FILE *fr)
{
  int
    CharEnd,
    NbCharRead = 0;
  char
    TypeOfType[_SdifNameLen];
  
  
  CharEnd = SdifFGetStringUntil(gSdifString,
				_SdifStringLen,
				fr,
				&NbCharRead,
				_SdifReservedChars);
  if ( (CharEnd != (int) '{') || (SdifStrLen(gSdifString)!=0) )
    {
      sprintf(gSdifErrorMess, "Attempt to read '{' failed : '%s%c'", gSdifString, CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return NbCharRead;
    }
  
  while( (CharEnd = SdifFGetName(TypeOfType, fr, &NbCharRead)) != (unsigned) '}' )
    {
      if ( (SdifIsAReservedChar((char) CharEnd) != -1) || (isspace((char) CharEnd)) )
	{
	  sprintf(gSdifErrorMess,
		  "Frame Name not correctly read : '%c%c%c%c'. Last char read : '%d'",
		  TypeOfType[0],
		  TypeOfType[1],
		  TypeOfType[2],
		  TypeOfType[3],
		  (char) CharEnd);
	  _SdifError(eNameLength, gSdifErrorMess);
	  return NbCharRead;
	}
      
      SdifStrNCpy(gSdifString, TypeOfType, _SdifNameLen);
      gSdifString[_SdifNameLen] = '\0';
      
      if (SdifStrNCmp (gSdifString, _Mtrx, _SdifNameLen+1) == 0)
	{
	  NbCharRead += SdifFReadMatrixType(Predefined, fr);
	}
      else
	if (SdifStrCmp (gSdifString, "Fram") == 0)
	  {
	    NbCharRead += SdifFReadFrameType(Predefined, fr);
	  }
	else
	  {
	    _SdifError(eSyntax, gSdifString);
	    return NbCharRead;
	  }
    }
  
  if (TypeOfType[0] != '\0')
    {
      _SdifError(eSyntax, gSdifString);
      return NbCharRead;
    }
  return NbCharRead;
}












int
SdifLoadPredefinedTypes(char* TypesFileName)
{
  FILE
    *TypesFile;
  int
    NbBytesRead = 0;
  char
    Name[_SdifNameLen];
  int
    SizeR = 0;

  
  if (TypesFile = fopen(TypesFileName, "r"))
    {
      SizeR += SdifFGetName(Name, TypesFile, &SizeR);
      if (SdifStrNCmp(Name, _SDIF, _SdifNameLen) == 0)
	{
	  SdifFGetName(Name, TypesFile, &SizeR);
	  while ((SdifStrNCmp(Name, _ENDF, _SdifNameLen) != 0) && (!feof(TypesFile)))
	    {
	      if (SdifStrNCmp(Name, _STYP, _SdifNameLen) == 0)
		SizeR += SdifFScanAllType(ePredefined, TypesFile);
	      
	      SizeR += SdifFGetName(Name, TypesFile, &SizeR);
	    }
	}
      else
	_SdifError(eBadTypesFile, TypesFileName);
    }
  else
    _SdifError(eFileNotFound, TypesFileName);
  

  fclose(TypesFile);
  return SizeR;
}










int
SdifFWriteAllStreamID(FILE *fw)
{
  unsigned int
    iID;
  SdifHashNodeType
    *pID;
  int
    NbBytesWrite = 0;
  SdifUInt4
    UnknownSize;
  
  UnknownSize = _SdifUnknownSize;
  
  NbBytesWrite += fprintf(fw, "%s", _SSIC);
  NbBytesWrite += sizeof(SdifUInt4) * SdifFWriteUInt4(&UnknownSize, 1, fw);
  NbBytesWrite += fprintf(fw, "\n{\n");  
  for(iID=0; iID<gSdifStreamIDsTable->HashSize; iID++)
    for (pID = gSdifStreamIDsTable->Table[iID]; pID; pID = pID->Next)
      NbBytesWrite += SdifFWriteOneStreamID(pID->Data, fw);

  NbBytesWrite += fprintf(fw, "}");

  NbBytesWrite += SdifFWritePadding(SdifFPaddingCalculate(fw, NbBytesWrite), fw);
  
  SdifUpdateChunkSize(NbBytesWrite, fw);

  return NbBytesWrite;
  
}










int
SdifFPrintAllStreamID(FILE *fw)
{
  int
    NbCharWrite = 0;
  unsigned int
    iID;
  SdifHashNodeType
    *pID;
    
  NbCharWrite += fprintf(fw, "%s\n{\n", _SSIC);
  for(iID=0; iID<gSdifStreamIDsTable->HashSize; iID++)
    for (pID = gSdifStreamIDsTable->Table[iID]; pID; pID = pID->Next)
      NbCharWrite += SdifFPrintOneStreamID(fw, pID->Data);

  NbCharWrite += fprintf(fw, "}\n\n");
  
  return NbCharWrite;
}









/* SdifFReadAllStreamID ne lit pas "SSIC" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "SSIC"
 */
int
SdifFReadAllStreamID(FILE *fr)
{
  SdifUInt4
    IDSize;
  int
    NbBytesRead = 0,
    CharEnd;
  
  
  NbBytesRead += sizeof(SdifUInt4) * SdifFReadUInt4(&IDSize, 1 , fr);

  if (CharEnd = SdifFGetStringUntil(gSdifString,
				    _SdifStringLen,
				    fr,
				    &NbBytesRead,
				    _SdifReservedChars) == '{')
    while (SdifFScanOneStreamID(fr, &NbBytesRead) != (int) '}')
      ;
  else
    {
      sprintf(gSdifErrorMess, "Attempt to read '{' failed : '%s%c'", gSdifString, CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return NbBytesRead;
    }
 
  NbBytesRead += SdifFReadPadding(SdifFPaddingCalculate(fr, NbBytesRead + _SdifNameLen), fr);

  if ((NbBytesRead + _SdifNameLen != IDSize) && ((unsigned) IDSize != (unsigned) _SdifUnknownSize))
    {
      fprintf(stderr,
	      "erreur size SSIC: lu: %d  Attendu: %d\n",
	      NbBytesRead + _SdifNameLen,
	      IDSize);
    }

  return NbBytesRead;
}










/* SdifFScanAllStreamID (ASCII version : pas de size, pas de padding)
 * ne lit pas "SSIC" puisque l'on est aiguillie sur cette fonction 
 * apres lecture de "SSIC"
 */
int
SdifFScanAllStreamID(FILE *fr)
{
  int
    NbBytesRead = 0,
    CharEnd;

  
  if (CharEnd = SdifFGetStringUntil(gSdifString,
				    _SdifStringLen,
				    fr,
				    &NbBytesRead,
				    _SdifReservedChars) == '{')
    while (SdifFScanOneStreamID(fr, &NbBytesRead) != '}')
      ;
  else
    {
      sprintf(gSdifErrorMess, "Attempt to read '{' failed : '%s%c'", gSdifString, CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
    }

  return NbBytesRead;
}











int
SdifFWriteNameValueCurrHT(FILE *fw)
{
  SdifHashNodeType
    *pNV;
  int
    NbBytesWrite = 0;
  SdifUInt4
    UnknownSize,
    iNV;
  SdifHashTableType *HTable;

  HTable = gSdifNameValues->CurrHT;

  UnknownSize = _SdifUnknownSize;
  
  NbBytesWrite += fprintf(fw, "%s", _SITC);
  NbBytesWrite += sizeof(SdifUInt4) * SdifFWriteUInt4(&UnknownSize, 1, fw);
  NbBytesWrite += fprintf(fw, "\n{\n");  
  for(iNV=0; iNV<HTable->HashSize; iNV++)
    for (pNV = HTable->Table[iNV]; pNV; pNV = pNV->Next)
      NbBytesWrite += SdifFWriteOneNameValue(pNV->Data, fw);

  NbBytesWrite += fprintf(fw, "}");

  /* if CurrHT = HeadHTN->NameValueHT
   * then it is the first SITC
   * and we count the HeaderFileSize for the Padding
   */
  if (HTable == gSdifNameValues->HeadHTN->NameValueHT)
    NbBytesWrite += SdifFWritePadding(SdifFPaddingCalculate(fw, NbBytesWrite+12), fw);
  else
    NbBytesWrite += SdifFWritePadding(SdifFPaddingCalculate(fw, NbBytesWrite), fw);
  
  SdifUpdateChunkSize(NbBytesWrite, fw);
  
  return NbBytesWrite;
  
}






int
SdifFPrintNameValueCurrHT(FILE *fw)
{
  int
    NbCharWrite = 0;
  SdifUInt4
    iNV;
  SdifHashNodeType
    *pNV;
  SdifHashTableType *HTable;

  HTable = gSdifNameValues->CurrHT;
  
  NbCharWrite += fprintf(fw, "%s\n{\n", _SITC);
  for(iNV=0; iNV<HTable->HashSize; iNV++)
    for (pNV = HTable->Table[iNV]; pNV; pNV = pNV->Next)
      NbCharWrite += SdifFWriteOneNameValue(pNV->Data, fw);
  
  NbCharWrite += fprintf(fw, "}\n\n");  

  return NbCharWrite;
}







/* SdifFReadNameValueCurrHT ne lit pas "SITC" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "SITC"
 */
int
SdifFReadNameValueCurrHT(FILE *fr)
{
  SdifUInt4
    NVSize;
  int
    NbBytesRead = 0,
    CharEnd;
  
  
  NbBytesRead += sizeof(SdifUInt4) * SdifFReadUInt4(&NVSize, 1 , fr);

  if (CharEnd = SdifFGetStringUntil(gSdifString,
				    _SdifStringLen,
				    fr,
				    &NbBytesRead,
				    _SdifReservedChars) == '{')
    while (SdifFScanOneNameValue(fr, &NbBytesRead) != (int) '}')
      ;
  else
    {
      sprintf(gSdifErrorMess, "Attempt to read '{' failed : '%s%c'", gSdifString, CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return NbBytesRead;
    }
 

  /* if CurrHT = HeadHTN->NameValueHT
   * then it is the first SITC
   * and we count the HeaderFileSize for the Padding
   */
  if (gSdifNameValues->CurrHT == gSdifNameValues->HeadHTN->NameValueHT)
    NbBytesRead += SdifFReadPadding(SdifFPaddingCalculate(fr, NbBytesRead+12+_SdifNameLen), fr);
  else
    NbBytesRead += SdifFReadPadding(SdifFPaddingCalculate(fr, NbBytesRead+ _SdifNameLen), fr);


  if ((NbBytesRead + _SdifNameLen != NVSize) && ((unsigned) NVSize != (unsigned) _SdifUnknownSize))
    {
      fprintf(stderr,
	      "erreur size SITC: lu: %d  Attendu: %d\n",
	      NbBytesRead + _SdifNameLen,
	      NVSize);
    }
  
  return NbBytesRead;
}







/* SdifFScanNameValueCurrHT (ASCII version : pas de size, pas de padding)
 * ne lit pas "SITC" puisque l'on est aiguillie sur cette fonction 
 * apres lecture de "SITC"
 */
int
SdifFScanNameValueCurrHT(FILE *fr)
{
  int
    NbBytesRead = 0,
    CharEnd;
  
  
  if (CharEnd = SdifFGetStringUntil(gSdifString,
				    _SdifStringLen,
				    fr,
				    &NbBytesRead,
				    _SdifReservedChars) == '{')
    while (SdifFScanOneNameValue(fr, &NbBytesRead) != '}')
      ;
  else
    {
      sprintf(gSdifErrorMess, "Attempt to read '{' failed : '%s%c'", gSdifString, CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
    }

  return NbBytesRead;
}







/* Read from the end of File Heade until TO HAVE READ 'SDFC' */
int
SdifFReadAllASCIIChunks(FILE *fr)
{
  char
    Name[_SdifNameLen];
  short
    STYPpass = 0,
    SSICpass = 0;
  int
    SizeR = 0;
  
  while (  (SdifFGetName(Name, fr, &SizeR)!=eEof)  )
    {
      if (SdifStrNCmp(Name, _STYP, _SdifNameLen)==0)
	{
	  if (STYPpass == 0)
	    {
	      SizeR += SdifFReadAllType(eUserdefined, fr);
	      STYPpass = 1;
	    }
	  else
	    _SdifError(eOnlyOneChunkOf, _STYP);
	}
      else 
	{
	  if (SdifStrNCmp(Name, _SSIC, _SdifNameLen)==0)
	    {
	      if (SSICpass == 0)
		{
		  SizeR += SdifFReadAllStreamID(fr);
		  SSICpass = 1;
		}
	      else
		_SdifError(eOnlyOneChunkOf, _SSIC);
	    }
	  else
	    {
	      if  (SdifStrNCmp(Name, _SITC, _SdifNameLen)==0)
		{
		  SdifNameValuesLNewHT(gSdifNameValues);
		  SizeR += SdifFReadNameValueCurrHT(fr);
		}
	      else
		{
		  if  (SdifStrNCmp(Name, _SDFC, _SdifNameLen)==0)      
		    {
		      break;
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
    }
  return SizeR;
}





/* After SdifFReadAllASCIIChunks, read the data frames chunk size */
int
SdifFReadFramesChunkSize(SdifInt4 *pSize, FILE *fr)
{
  int NbBytesRead = 0;

  NbBytesRead += sizeof(SdifInt4) * SdifFReadInt4(pSize, 1, fr);

  return NbBytesRead;
}










int
SdifFWriteAllASCIIChunks(FILE *fw)
{
  int
    iNVHT,
    NbBytesWrite = 0;



  for (iNVHT = 1; iNVHT <= gSdifNameValues->NbHTN; iNVHT++)
    {
      SdifNameValuesLSetCurrHT(gSdifNameValues, iNVHT);
      NbBytesWrite += SdifFWriteNameValueCurrHT(fw);
    }

  NbBytesWrite += SdifFWriteAllType(eUserdefined, fw);
  NbBytesWrite += SdifFWriteAllStreamID(fw);
  
  return NbBytesWrite;
}








int
SdifFWriteFramesChunkHeader(SdifInt4 Size, FILE *fw)
{
  int NbBytesWrite = 0;

  NbBytesWrite += SdifFWrite4Char(_SDFC, fw);
  NbBytesWrite += sizeof(SdifInt4)  * SdifFWriteInt4( &(Size), 1, fw);

  return NbBytesWrite;
}





