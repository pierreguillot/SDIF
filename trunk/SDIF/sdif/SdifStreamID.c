/* SdifStreamID.c
 *
 *
 *
 */



#include "SdifStreamID.h"
#include "SdifRWLowLevel.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>







SdifStreamIDType*
SdifCreateStreamID(SdifUInt4 NumID, char *Source, char *TreeWay)
{
  SdifStreamIDType *NewID;
  
  if (NewID =  (SdifStreamIDType*) malloc (sizeof(SdifStreamIDType)))
    {
      NewID->NumID = NumID;
      NewID->Source  = SdifCreateStrNCpy(Source, SdifStrLen(Source)+1);
      NewID->TreeWay = SdifCreateStrNCpy(TreeWay, SdifStrLen(TreeWay)+1);
      
      return NewID;
    }
  else
    {
      _SdifError(eAllocFail, "StreamID allocation");
      return NULL;
    }
}






void
SdifKillStreamID(SdifStreamIDType *StreamID)
{
  if (StreamID)
    {
      SdifKillStr(StreamID->TreeWay);
      SdifKillStr(StreamID->Source);
      free(StreamID);
    }
  else
    _SdifError(eFreeNull, "StreamID free");
}





int
SdifFPrintOneStreamID(FILE *fw, SdifStreamIDType *StreamID)
{
  int
    NbCharWrite = 0;

  NbCharWrite += fprintf(fw, "  %u", StreamID->NumID); /* Ascii Number */
  NbCharWrite += fprintf(fw, "\t%s:", StreamID->Source);
  NbCharWrite += fprintf(fw, "%s;\n", StreamID->TreeWay);

  return NbCharWrite;
}






int
SdifFWriteOneStreamID(SdifStreamIDType *StreamID, FILE *fw)
{
  int NbBytesWrite = 0;

  /*NbBytesWrite += sizeof(SdifUInt4) * SdifFWriteUInt4(&(StreamID->NumID), 1, fw);
   */
  NbBytesWrite += fprintf(fw, "%u ", StreamID->NumID);
  NbBytesWrite += SdifFWriteString(StreamID->Source, fw);
  NbBytesWrite += fprintf(fw, ":");
  NbBytesWrite += SdifFWriteString(StreamID->TreeWay, fw);
  /* Padding */
  /*NbBytesWrite += SdifFWritePadding(SdifPaddingCalculate(NbBytesWrite)-1, fw);
   */
  NbBytesWrite += fprintf(fw, ";\n");

  return  NbBytesWrite;
}









int
SdifFScanOneStreamID(FILE *fr, int *NbBytesRead)
{
  SdifStreamIDType
    *StreamID;
  SdifUInt4
    NumID;
  char
    CharEnd;
  static char 
    CharsEnd[] = {' ', '\t', '\n', '\f', '\r', '\v', '{', '}', ',', ';',':','\0'};

  

  /* ID */
  CharEnd = SdifFGetStringUntil(gSdifString, _SdifStringLen, fr, NbBytesRead, CharsEnd);
  if ( (CharEnd == '}') && (SdifStrLen(gSdifString) == 0) ) /* no more IDStream */
    return  CharEnd;
  if (! isspace(CharEnd))
    {
      sprintf(gSdifErrorMess, "Wait a space_char after %s,  read char : %d", gSdifString, CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return  CharEnd;
    }

  NumID = atoi(gSdifString);
  if (SdifHashTableSearch(gSdifStreamIDsTable, &(NumID), 1))
    {
      sprintf(gSdifErrorMess, "StreamID : %u ", NumID);
      _SdifError(eReDefined, gSdifErrorMess);
      return  CharEnd;
    }
  


  /* source */
  CharEnd = SdifFGetStringUntil(gSdifString, _SdifStringLen, fr, NbBytesRead, CharsEnd);
  if (CharEnd != (unsigned) ':')
    {
      sprintf(gSdifErrorMess,
	      "Attempt to read ':' : '%s%c' ",
	      gSdifString,
	      CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return  CharEnd;
    }
  

  /* TreeWay : simple string pour le moment */
  CharEnd = SdifFGetStringUntil(gSdifString2, _SdifStringLen, fr, NbBytesRead, _SdifReservedChars);
  if (CharEnd != (unsigned) ';')
    {
      sprintf(gSdifErrorMess,
	      "Attempt to read ';' : '%s%c' ",
	      gSdifString2,
	      CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return  CharEnd;
    }
  
  StreamID = SdifCreateStreamID(NumID, gSdifString, gSdifString2);
  /* printf("%d %s %s\n",  StreamID->NumID, StreamID->Source, StreamID->TreeWay);
   */
  if (StreamID)
    {
      SdifHashTablePut(gSdifStreamIDsTable, &(StreamID->NumID), 1, StreamID);
      return  CharEnd;
    }

  return  CharEnd;  
}
