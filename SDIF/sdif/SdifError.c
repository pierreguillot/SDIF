/* sdifError.c
 *
 *
 *
 */


#include "SdifError.h"

#include <stdio.h>
#include <stdlib.h>




#define _NbOfWarningMax 9




char *SdifErrorFile;

int SdifErrorLine;

unsigned int NbOfWarning = 0;





void
SdifErrorWarning(SdifOutReportType Error, void *ErrorMess)
{
  if (NbOfWarning++ > _NbOfWarningMax)
    {
      fprintf(stderr, "*Sdif* STOP in '%s' line %d\n   ", SdifErrorFile, SdifErrorLine);
      fprintf(stderr, "--- Too Many Warnings ---\n");
      exit(1);
    }

  fprintf(stderr, "*Sdif* Warning in '%s' line %d\n   ", SdifErrorFile, SdifErrorLine);

  switch(Error)
    {
    case  eFalse :
      fprintf(stderr, "False : '%s'\n");
      break;
    case  eTrue :
      fprintf(stderr, "True\n");
      break;
    case  eFreeNull :
      fprintf(stderr, "Attempt to free a NULL pointer : '%s'\n", ErrorMess);
      break;
    case  eAllocFail :
      fprintf(stderr, "Attempt to allocate memory : '%s'\n", ErrorMess);
      break;
    case  eUserdefBefore :
      fprintf(stderr, "Predefined column whereas it exits user definition : '%s'\n", ErrorMess);
      break;      
    case eTypeDataNotSupported :
      fprintf(stderr, "Type of data not actualy supported : '%s'\n", ErrorMess);
      break;
    case eArrayPosition :
      fprintf(stderr, "Attempt to access to a non-existing square in an array : '%s'\n", ErrorMess);
      break;
    case eWordCut :
      fprintf(stderr, "Word cut read : '%s'\n", ErrorMess);
      break;
    case  eEof :
      fprintf(stderr, "End of file : %s\n", ErrorMess);
      break;
    case  eTokenLength :
      fprintf(stderr, "Token too long : '%s'\n", ErrorMess);
      break;
    case  eNameLength :
      fprintf(stderr, "Bad Name : '%s'\n", ErrorMess);
      break;
    case  eReDefined :
      fprintf(stderr, "%s redefined\n", ErrorMess);
      break;
    case  eUnDefined :
      fprintf(stderr, "%s undefined\n", ErrorMess);
      break;
    case  eSyntax :
      fprintf(stderr, "Syntax error\n    %s\n", ErrorMess);
      break;
    case  eAffectationOrder :
      fprintf(stderr, "Affectation must be in order : '%s'\n", ErrorMess);
      break;
    case  eBadType :
      fprintf(stderr, "Bad Type used : '%s'\n", ErrorMess);
      break;
    case eBadHeader:
      fprintf(stderr, "Bad Sdif Header : '%s'\n", ErrorMess);
      break;
    case  eBadTypesFile:
      fprintf(stderr, "'%s' is not a Types Definitions file\n", ErrorMess);
      break;
    case  eRecursiveDetect :
      fprintf(stderr, "Recursive declaration detected : '%s'\n", ErrorMess);
      break;
    case  eUnInterpreted :
      fprintf(stderr, "Attempt to interpret '%s' failed\n", ErrorMess);
      break;
    case  eInfoTableMissing:
      fprintf(stderr, "Information table missing!\n   One of this must be just after the general header\n");
      break;      
    case  eOnlyOneChunkOf:
      fprintf(stderr, "There can be only 0 or 1 '%s' chunk.\n",  ErrorMess);
      break;            
    case  eFileNotFound :
      fprintf(stderr, "File Not Found : \"%s\"\n",ErrorMess);
      break;            
    default :
      fprintf(stderr, "Warning unknown", ErrorMess);
      break;
    }
  fflush(stderr);
}
