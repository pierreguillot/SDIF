/* $Id: SdifString.c,v 3.3.2.1 2000-08-21 14:04:23 tisseran Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * String management
 *
 * author : Patrice Tisserand 2000
 *
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.1  2000/07/18  15:08:40  tisseran
 * This release implements the New SDIF Specification (june 1999):
 * - Name Values Table are written in a 1NVT frame which contains a 1NVT matrix
 * - Frame and matrix type declaration are written in a 1TYP frame which contains a 1TYP matrix.
 * - Stream ID are written in a 1IDS frame which contains a 1IDS matrix.
 *
 * Read function accept the previous version of the specification (read a text frame without matrix) to be compatible with older SDIF files.
 *
 * SdifString.h and SdifString.c implements some string mangement (creation, destruction, append, test of end of string, getc, ungetc).
 *
 * WATCH OUT:
 *      We don't care about the old SDIF Specification (_SdifFormatVersion < 3)
 * To use _SdifFormatVersion < 3, get the previous release.
 *
 */

#include <string.h>
#include "SdifString.h"
#include "SdifError.h"

/* Function declaration */
/*DOC:
  Make a memory allocation for a SdifStringT structure.
  The size for the string is defined in SdifString.h; define _SdifStringGranule 128.
*/
SdifStringT * SdifStringNew(void)
{
  SdifStringT *SdifString;
  size_t SizeW = 0;
  size_t TotalSize = 0;

  SdifString = (SdifStringT *)malloc(sizeof(SdifStringT));
  TotalSize = _SdifStringGranule;
  
  SdifString->str = (char *)malloc(TotalSize * sizeof(char));
  SdifString->SizeW = SizeW;
  SdifString->TotalSize = TotalSize;
  SdifString->NbCharRead = 0;

  return(SdifString);
}


/*DOC:
  Free memory allocated for SdifString.
*/
void SdifStringFree(SdifStringT *SdifString)
{
  free(SdifString->str);
  free(SdifString);

  return;
}


/*DOC:
  Append a string to another one.
  Manage memory reallocation.
  Return a boolean for the succes of the function's call.
*/
int SdifStringAppend(SdifStringT * SdifString ,char *strToAppend)
{
  char   *tmpStr; /* Temporary string for memory allocation */
  char   *str = SdifString->str;
  size_t SizeW = SdifString->SizeW;
  size_t TotalSize = SdifString->TotalSize;
  size_t memoryNeeded;
  int    memoryQuantuum;

  /* enough space to append (including terminating zero)? */
  if ((TotalSize - SizeW) < strlen(strToAppend) + 1) 
    {
      /* Need a memory reallocation */
      memoryQuantuum =(int) ((SizeW - TotalSize + strlen(strToAppend)) / _SdifStringGranule) ;
      memoryNeeded = (memoryQuantuum + 1) * _SdifStringGranule; 
      tmpStr = (char *)realloc(str, TotalSize + memoryNeeded);
      if (!tmpStr)
	{
	  fprintf(stderr,"No more memory available!!!\n");
	  return(0);
	}
      str = tmpStr;
      TotalSize += memoryNeeded;
    }
  SizeW += sprintf(str + SizeW, "%s", strToAppend);

  /* Variable reaffectation */
  SdifString->str = str;
  SdifString->SizeW = SizeW;
  SdifString->TotalSize = TotalSize;
  
  return(1);
}

/*DOC:
  Read the current char (= fgetc).
*/
int SdifStringGetC(SdifStringT *SdifString)
{
  int NbCharRead = SdifString->NbCharRead;
  int chint;

  chint = (int) SdifString->str[NbCharRead];

  SdifString->NbCharRead++;

  return chint;
}

/*DOC:
  Equivalent of ungetc
*/
int SdifStringUngetC(SdifStringT *SdifString)
{
  if (SdifStringIsEOS(SdifString) || (SdifString->NbCharRead == 0))
      return eFalse;
  else
    {
      SdifString->NbCharRead--;
      return eTrue;
    }
}


/*DOC:
  Test the end of the string (= feof)
*/
int SdifStringIsEOS(SdifStringT *SdifString)
{
  int NbCharRead = SdifString->NbCharRead;
  int SizeW = SdifString->SizeW;
  
  if (NbCharRead >= SizeW)
    return eTrue;
  else
    return eFalse;
}
