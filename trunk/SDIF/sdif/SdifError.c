/* $Id: SdifError.c,v 1.3 1998-04-24 12:40:31 schwarz Exp $
 * sdifError.c
 *
 * Fatal or program error management
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */


#include "SdifError.h"
#include <stdio.h>
#include <stdlib.h>

char *SdifErrorFile;
int SdifErrorLine;



void
SdifErrorWarning(SdifErrorEnum Error, const void *ErrorMess)
{
  fprintf(stderr, "*Sdif* Error (%s, %d)\n  ", SdifErrorFile, SdifErrorLine);

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
    case  eInvalidPreType:
      fprintf(stderr, "Invalid Predefined Type : %s\n", ErrorMess);
      break;
    case eArrayPosition :
      fprintf(stderr, "Attempt to access to a non-existing square in an array : '%s'\n", ErrorMess);
      break;
    case  eEof :
      fprintf(stderr, "End of file : %s\n", ErrorMess);
      break;
    case  eFileNotFound :
      fprintf(stderr, "File Not Found : \"%s\"\n",ErrorMess);
      break;            
    case  eAffectationOrder :
      fprintf(stderr, "Affectation must be in order : '%s'\n", ErrorMess);
      break;
    case  eNoModifErr :
      fprintf(stderr, "Type has been defined yet: '%s'\n", ErrorMess);
      break;      
    case  eNotInDataTypeUnion :
      fprintf(stderr, "Type of data Not in DataTypeUnion  : '%s'\n", ErrorMess);
      break;      
    case  eNotFound :
      fprintf(stderr, "Not Find : '%s'\n", ErrorMess);
      break;      
    case  eExistYet :
      fprintf(stderr, "Object Exists Yet : '%s'\n", ErrorMess);
      break;      
    case eWordCut :
      fprintf(stderr, "Word cut read : '%s'\n", ErrorMess);
      break;
    case  eTokenLength :
      fprintf(stderr, "Token too long : '%s'\n", ErrorMess);
      break;
    default :
      fprintf(stderr, "Warning unknown", ErrorMess);
      break;
    }
  fflush(stderr);
  exit(1);
}
