/* $Id: SdifError.c,v 2.3 1999-01-23 13:57:20 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 * sdifError.c
 *
 * Fatal or program error management
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.2  1998/12/21  18:27:03  schwarz
 * Inserted copyright message.
 *
 * Revision 2.1  1998/12/09  15:05:24  virolle
 * no segmentation fault when opening failed.
 *
 * Revision 2.0  1998/11/29  11:41:25  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.3  1998/04/24  12:40:31  schwarz
 * Made char * arguments constant for SdifNameValuesLPut and functions called by it.
 *
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
      fflush(stderr);
      exit(1);
      break;
    case  eAllocFail :
      fprintf(stderr, "Attempt to allocate memory : '%s'\n", ErrorMess);
      fflush(stderr);
      exit(1);
      break;
    case  eInvalidPreType:
      fprintf(stderr, "Invalid Predefined Type : %s\n", ErrorMess);
      break;
    case eArrayPosition :
      fprintf(stderr, "Attempt to access to a non-existing square in an array : '%s'\n", ErrorMess);
      fflush(stderr);
      exit(1);
      break;
    case  eEof :
      fprintf(stderr, "End of file : %s\n", ErrorMess);
      fflush(stderr);
      exit(1);
      break;
    case  eFileNotFound :
      fprintf(stderr, "File Not Found or no Authorizations: \"%s\"\n",ErrorMess);
      fflush(stderr);
      break;            
    case  eAffectationOrder :
      fprintf(stderr, "Affectation must be in order : '%s'\n", ErrorMess);
      break;
    case  eNoModifErr :
      fprintf(stderr, "Type has been defined yet: '%s'\n", ErrorMess);
      break;      
    case  eNotInDataTypeUnion :
      fprintf(stderr, "Type of data Not in DataTypeUnion  : '%s'\n", ErrorMess);
      fflush(stderr);
      exit(1);
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
      fflush(stderr);
      exit(1);
      break;
    default :
      fprintf(stderr, "Warning unknown", ErrorMess);
      break;
    }
}
