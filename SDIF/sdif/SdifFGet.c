/* $Id: SdifFGet.c,v 2.3 1999-01-23 15:55:39 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * F : SdifFileT*, Get : ascii frames reading,
 * common to read a sdif file and a pseudo-sdif text file
 * 'verbose' allows to choise if the file is a sdif file or a pseudo-sdif text file
 *
 * author: Dominique Virolle 1997
 *
 *
 */



#include "SdifFGet.h"
#include "SdifTest.h"
#include "SdifFile.h"

#include "SdifRWLowLevel.h"

#include "SdifNameValue.h"
#include "SdifHash.h"
#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifStreamID.h"
#include "SdifErrMess.h"

#include "SdifTimePosition.h"


#include <ctype.h>
#include <stdlib.h>



int
SdifFGetSignature(SdifFileT *SdifF, size_t *NbCharRead)
{
  return SdiffGetSignature(SdifF->Stream, &(SdifF->CurrSignature), NbCharRead);
}




int
SdifFGetOneNameValue(SdifFileT *SdifF, int Verbose, size_t *SizeR)
{
  FILE         *file;
  int          CharEnd;
  static char  CharsEnd[] = " \t\n\f\r\v{},;:";


  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  /* Name */
  CharEnd = SdiffGetStringUntil(file, gSdifString, _SdifStringLen, SizeR, CharsEnd);

  if ( (CharEnd == '}') && (SdifStrLen(gSdifString) == 0) ) /* no more NameValue */
    return  CharEnd;
  if (! isspace(CharEnd))
    {
      sprintf(gSdifErrorMess,
	      "Wait a space_char after '%s', read char : (%d) '%c'",
	      gSdifString,
	      CharEnd,
	      CharEnd);
      _SdifFError(SdifF, eSyntax, gSdifErrorMess);
      return  CharEnd;
    }
  
  if (SdifNameValuesLGetCurrNVT(SdifF->NameValues, gSdifString))
    {
      sprintf(gSdifErrorMess, "NameValue : %s ", gSdifString);
      _SdifFError(SdifF, eReDefined, gSdifErrorMess);
      CharEnd = SdiffGetStringUntil(file, gSdifString, _SdifStringLen, SizeR, ";");
      return  CharEnd;
    }
  
  
  
  /* Value */
  CharEnd = SdiffGetStringUntil(file, gSdifString2, _SdifStringLen, SizeR, _SdifReservedChars);

  if (CharEnd != (unsigned) ';')
    {
      sprintf(gSdifErrorMess,
	      "Attempt to read ';' : '%s%c' ",
	      gSdifString2,
	      CharEnd);
      _SdifFError(SdifF, eSyntax, gSdifErrorMess);
      return  CharEnd;
    }
  
  SdifNameValuesLPutCurrNVT(SdifF->NameValues, gSdifString, gSdifString2);
  return  CharEnd;
}





size_t
SdifFGetNameValueLCurrNVT(SdifFileT *SdifF, int Verbose)
{
/* SdifFGetNameValueLCurrNVT ne lit pas "SITC" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "SITC"
 */
  size_t    SizeR = 0;
  int       CharEnd;
  FILE      *file;

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
  
  CharEnd = SdiffGetStringUntil(file, gSdifString, _SdifStringLen, &SizeR,_SdifReservedChars);
  if (SdifTestCharEnd(SdifF,     CharEnd,    '{',    gSdifString, 
		   (short)(SdifStrLen(gSdifString)!= 0),
		      "Begin of NameValue Table declarations") == eFalse)
    {
      return SizeR;
    }
  else
    {
      while (SdifFGetOneNameValue(SdifF, Verbose, &SizeR) != (int) '}')
      ;
    }
   
  return SizeR;
}






size_t
SdifFGetOneMatrixType(SdifFileT *SdifF, int Verbose)
{
  SdifMatrixTypeT  *MatrixType;
  size_t           SizeR = 0;
  int              CharEnd;
  SdifSignature    Signature = 0;
  FILE             *file;


  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  CharEnd = SdiffGetSignature(file, &Signature, &SizeR);

  if (SdifTestSignature(SdifF, CharEnd, Signature, "Matrix")== eFalse)
    return SizeR;  
 

  /* Matrix type Creation, Put or Recuperation from SdifF->MatrixTypesTable */
  MatrixType = SdifGetMatrixType(SdifF->MatrixTypesTable, Signature);
  if (! MatrixType)
    {
      MatrixType = SdifCreateMatrixType(Signature,
					SdifGetMatrixType(gSdifPredefinedTypes->MatrixTypesTable, Signature));
      SdifPutMatrixType(SdifF->MatrixTypesTable, MatrixType);
    }
  else
    {
      if (SdifTestMatrixTypeModifMode(SdifF, MatrixType)== eFalse)
	{
	  /* Skip matrix type def, search '}' */
	  SdifTestCharEnd(SdifF,
			  SdifSkipASCIIUntil(file, &SizeR, "}:[];"),
			  '}', "", eFalse,
			  "end of matrix type skiped missing");
	  return SizeR;
	}
    }


  /* ColumnDefs */
  CharEnd = SdiffGetStringUntil(file, gSdifString, _SdifStringLen, &SizeR, _SdifReservedChars);
  if (SdifTestCharEnd(SdifF, CharEnd, '{', gSdifString,
              (short)(SdifStrLen(gSdifString)!=0), "Matrix Type") == eFalse)
    return SizeR;
  else
    {
      while (   (CharEnd = SdiffGetStringUntil(file, gSdifString, _SdifStringLen, &SizeR, _SdifReservedChars))
	     == (int) ','  )
	{
	  SdifMatrixTypeInsertTailColumnDef(MatrixType, gSdifString);
	}

      if (SdifTestCharEnd(SdifF, CharEnd, '}', gSdifString, eFalse, "end of matrix type missing") == eFalse)
	return SizeR;
      else
	if (SdifStrLen(gSdifString) != 0)
	  SdifMatrixTypeInsertTailColumnDef(MatrixType, gSdifString);
    }

  MatrixType->ModifMode = eNoModif;

  return SizeR;
}









int
SdifFGetOneComponent(SdifFileT *SdifF, int Verbose,
		     SdifSignature *MatrixSignature, char *ComponentName, size_t *SizeR)
{
  int   CharEnd;
  FILE *file;


  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
  
  
  ComponentName[0]= '\0';
  *MatrixSignature = eEmptySignature;

  /* Matrix Signature */
  CharEnd = SdiffGetSignature(file, MatrixSignature, SizeR); 
  
  if (CharEnd == (unsigned) '}')
    {
      /* no more Component */
      if  (*MatrixSignature == 0) 
	return  CharEnd;
      else
	{
	  sprintf(gSdifErrorMess,
		  "Incomplete Component : '%s%c'",
		  SdifSignatureToString(*MatrixSignature),
		  CharEnd);
	  _SdifFError(SdifF, eSyntax, gSdifErrorMess);
	  return CharEnd;
	}
    }
  else
    if (SdifTestSignature(SdifF, CharEnd, *MatrixSignature, "matrix signature of Component")== eFalse)
      return CharEnd;
  
  /* Component Name */
  CharEnd = SdiffGetStringUntil(file, gSdifString,
				_SdifStringLen, SizeR, _SdifReservedChars);
  if (SdifTestCharEnd(SdifF, CharEnd, ';', gSdifString, eFalse,
		      "Component must be finished by ';'") == eFalse)
    return  CharEnd;
  
  return CharEnd;
}










size_t
SdifFGetOneFrameType(SdifFileT *SdifF, int Verbose)
{
  size_t          SizeR = 0;
  int             CharEnd;
  FILE           *file;
  SdifFrameTypeT *FramT;
  SdifSignature
    FramSignature = 0,
    MtrxSignature = 0;  


  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
  

  /* FramSignature */
  CharEnd = SdiffGetSignature(file, &FramSignature, &SizeR);
  if (SdifTestSignature(SdifF, CharEnd, FramSignature, "Frame")== eFalse)
    {
      return SizeR;
    }
 

  /* Frame type Creation, Put or Recuperation from SdifF->FrameTypesTable */
  FramT =  SdifGetFrameType(SdifF->FrameTypesTable, FramSignature);
  if (! FramT)
    {
      FramT = SdifCreateFrameType(FramSignature,
				  SdifGetFrameType(gSdifPredefinedTypes->FrameTypesTable, FramSignature));
      SdifPutFrameType(SdifF->FrameTypesTable, FramT);
    }
  else
    if (SdifTestFrameTypeModifMode(SdifF, FramT)== eFalse)
      {
	/* Skip frame type def, search '}' */
	SdifTestCharEnd(SdifF,
			SdifSkipASCIIUntil(file, &SizeR, "}:[]"),
			'}', "", eFalse,
			"end of frame type skiped missing");
	return SizeR;
      }



  /* Components */
  CharEnd = SdiffGetStringUntil(file, gSdifString,
				_SdifStringLen, &SizeR, _SdifReservedChars);
  if (   SdifTestCharEnd(SdifF, CharEnd, '{',
			 gSdifString, (short)(SdifStrLen(gSdifString)!=0) ,
			 "Frame")
	 ==eFalse   )
    {
      return SizeR;
    }
  else
    {
      while ( SdifFGetOneComponent(SdifF,
                                   Verbose,
                                   &MtrxSignature,
                                   gSdifString, &SizeR)
              != (unsigned) '}'  )
        {
          if (SdifTestMatrixType(SdifF, MtrxSignature))
	        {
	          SdifFrameTypePutComponent(FramT, MtrxSignature, gSdifString);
	          MtrxSignature = 0;
	        }
	    }
    }


  FramT->ModifMode = eNoModif;
    
  return SizeR;
}







/* SdifFGetAllType ne lit pas "1TYP" puisque l'on sera aiguillie sur cette fonction 
 * apres lecture de "1TYP"
 */
size_t
SdifFGetAllType(SdifFileT *SdifF, int Verbose)
{
  int            CharEnd;
  size_t         SizeR = 0;
  SdifSignature  TypeOfType = 0;
  FILE          *file;
  

  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
  
  
  if (SdifF->TypeDefPass != eNotPass)
    _SdifFError(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1TYP));
  /* Read anyway */
  
  CharEnd = SdiffGetStringUntil(file, gSdifString, _SdifStringLen, &SizeR, _SdifReservedChars);
  if (SdifTestCharEnd(SdifF,     CharEnd,    '{',    gSdifString, 
		      (short)(SdifStrLen(gSdifString)!=0),
		      "Begin of Types declarations") == eFalse)
    {
      return SizeR;
    }
  
  
  while( (CharEnd = SdiffGetSignature(file, &TypeOfType, &SizeR)) != (unsigned) '}' )
    {      
      switch (TypeOfType)
	{
	case e1MTD :
	  SizeR += SdifFGetOneMatrixType(SdifF, Verbose);
	  break;
	case e1FTD :
	  SizeR += SdifFGetOneFrameType(SdifF, Verbose);
	  break;
	default :
	  sprintf(gSdifErrorMess, "Wait '%s' or '%s' : '%s'",
		  SdifSignatureToString(e1MTD),
		  SdifSignatureToString(e1FTD),
		  SdifSignatureToString(TypeOfType));
	  _SdifFError(SdifF, eSyntax, gSdifErrorMess);
	  SdifTestCharEnd(SdifF,
			  SdifSkipASCIIUntil(file, &SizeR, "}:[]"),
			  '}', "", eFalse,
			  "end of unknown type def skiped missing");
	  break;
	}
      TypeOfType = 0;
    }
  
  if (TypeOfType != 0)
    {
      _SdifFError(SdifF, eSyntax, SdifSignatureToString(TypeOfType));
      return SizeR;
    }
  
  SdifF->TypeDefPass = eReadPass;

  return SizeR;
}









int
SdifFGetOneStreamID(SdifFileT *SdifF, int Verbose, size_t *SizeR)
{
  SdifUInt4        NumID;
  char             CharEnd;
  static char      CharsEnd[] = " \t\n\f\r\v{},;:";
  FILE            *file;


  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);
  
  

  CharEnd = (char) SdiffGetStringUntil(file, gSdifString, _SdifStringLen, SizeR, CharsEnd);

  /* test if it's the last or not */
  if ( (CharEnd == '}') && (SdifStrLen(gSdifString) == 0) )
    {
      /* no more IDStream */
      return  CharEnd;
    }

  if (! isspace(CharEnd))
    {
      sprintf(gSdifErrorMess,
	      "Wait a space_char after NumId '%s', read char: (%d) '%c'",
	      gSdifString, CharEnd, CharEnd);
      _SdifFError(SdifF, eSyntax, gSdifErrorMess);
      if (CharEnd != (unsigned)';')
	SdifTestCharEnd(SdifF,
			SdifSkipASCIIUntil(file, SizeR, ";"),
			';',
			"", eFalse, "end of Stream ID skiped missing");
      return  CharEnd;
    }


  /* ID */
  NumID = atoi(gSdifString);
  if (SdifStreamIDTableGetSID(SdifF->StreamIDsTable, NumID))
    {
      sprintf(gSdifErrorMess, "StreamID : %u ", NumID);
      _SdifFError(SdifF, eReDefined, gSdifErrorMess);
      if (CharEnd != (unsigned)';')
	SdifTestCharEnd(SdifF,
			SdifSkipASCIIUntil(file, SizeR, ";"),
			';',
			"", eFalse, "end of Stream ID skiped missing");
      return  CharEnd;
    }
  


  /* source */
  CharEnd = (char) SdiffGetStringUntil(file, gSdifString, _SdifStringLen, SizeR, CharsEnd);
  if (SdifTestCharEnd(SdifF, CharEnd, ':', gSdifString, eFalse, "Stream ID Source") == eFalse)
    {
      if (CharEnd != (unsigned) ';')
	SdifTestCharEnd(SdifF,
			SdifSkipASCIIUntil(file, SizeR, ";"),
			';',
			"", eFalse, "end of Stream ID skiped missing");
      return  CharEnd;
    }
  

  /* TreeWay : simple string pour le moment */
  CharEnd = (char) SdiffGetStringWeakUntil(file, gSdifString2, _SdifStringLen, SizeR, ";");
  /*CharEnd = SdiffGetStringUntil    (file, gSdifString2, _SdifStringLen, SizeR, _SdifReservedChars);*/
  if (SdifTestCharEnd(SdifF, CharEnd, ';', gSdifString2, eFalse, "end of Stream ID TreeWay") == eFalse)
    {
      return  CharEnd;
    }
  

  SdifStreamIDTablePutSID(SdifF->StreamIDsTable, NumID, gSdifString, gSdifString2);
  return  CharEnd;
}





size_t
SdifFGetAllStreamID(SdifFileT *SdifF, int Verbose)
{
  size_t     SizeR = 0;
  int        CharEnd;
  FILE      *file;
  
  file = SdifFGetFILE_SwitchVerbose(SdifF, Verbose);

  if (SdifF->StreamIDPass != eNotPass)
    _SdifFError(SdifF, eOnlyOneChunkOf, SdifSignatureToString(e1IDS));
  /* Read anyway */
  
  CharEnd = SdiffGetStringUntil(file, gSdifString, _SdifStringLen, &SizeR, _SdifReservedChars);
  if (SdifTestCharEnd(SdifF,     CharEnd,    '{',    gSdifString, 
		      (short)(SdifStrLen(gSdifString)!=0),
		      "Begin of StreamID declarations") == eFalse)
    {
      return SizeR;
    }
  else
    {
      while (SdifFGetOneStreamID(SdifF, Verbose, &SizeR) != (int) '}')
	;
    }
  
  SdifF->StreamIDPass = eReadPass; 

  return SizeR;
}







size_t
SdifFGetNameValueCurrHT(SdifFileT *SdifF, int Verbose)
{
    /* obsolete */
    _Debug("SdifFGetNameValueCurrHT is obsolete, use SdifFGetNameValueLCurrNVT");
    return SdifFGetNameValueLCurrNVT(SdifF, Verbose);

}


