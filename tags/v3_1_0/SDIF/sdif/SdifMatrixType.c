/* $Id: SdifMatrixType.c,v 3.3 2000-05-15 16:22:32 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * Matrix Types management (interpreted sdif frame types)
 *  
 * author: Dominique Virolle 1997
 *
 *
 * SdifHashTableType to stock Matrix Types
 *      index : matrix type signature (HashInt4)
 *       Data : matrix type (SdifMatrixTypeT*)
 *     Killer : SdifKillMatrixType (if types are coded, Killer  == NULL)
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 3.2  1999/09/28  13:09:04  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:57:08  virolle
 * SdifStdErr add
 *
 * Revision 2.4  1999/02/28  12:16:51  virolle
 * memory report
 *
 * Revision 2.3  1999/01/23  15:55:55  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:40  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 */


#include <preincluded.h>
#include "SdifMatrixType.h"

#include <stdlib.h>





SdifColumnDefT*
SdifCreateColumnDef(char *Name, SdifUInt4 Num)
{
  SdifColumnDefT *NewColumnDef = NULL;
  
  NewColumnDef = SdifMalloc(SdifColumnDefT);
  if (NewColumnDef != NULL)
    {
      NewColumnDef->Name = SdifCreateStrNCpy(Name, SdifStrLen(Name)+1);
      NewColumnDef->Num = Num;
      
      return NewColumnDef;
    }
  else
    {
      _SdifError(eAllocFail, "ColumnDef allocation");
      return NULL;
    }
}






void
SdifKillColumnDef(void *ColumnDef)
{
  if (ColumnDef)
    {
      SdifKillStr(((SdifColumnDefT *) ColumnDef)->Name);
      SdifFree(ColumnDef);
    }
  else
    _SdifError(eFreeNull, "ColumnDef free");
}








SdifMatrixTypeT*
SdifCreateMatrixType(SdifSignature Signature, SdifMatrixTypeT *PredefinedMatrixType)
{
  SdifMatrixTypeT *NewMatrixType = NULL;

  NewMatrixType = SdifMalloc(SdifMatrixTypeT);
  if (NewMatrixType)
    {
      NewMatrixType->Signature      = Signature;
      NewMatrixType->ColumnUserList = SdifCreateList(SdifKillColumnDef);
      NewMatrixType->NbColumnDef    = 0;
      NewMatrixType->ModifMode      = eCanModif;

      if (PredefinedMatrixType)
	{
	  if (PredefinedMatrixType->Signature != Signature)
	    {
	      sprintf(gSdifErrorMess, "'%s'(Pre) != '%s'",
		      SdifSignatureToString(PredefinedMatrixType->Signature),
		      SdifSignatureToString(Signature));
	      _SdifError(eInvalidPreType, gSdifErrorMess);
	    }
	  else
	    {
	      NewMatrixType->MatrixTypePre  = PredefinedMatrixType;
	      NewMatrixType->NbColumnDef    = PredefinedMatrixType->ColumnUserList->NbData;
	    }
	}
      else
	{
	  NewMatrixType->MatrixTypePre  = NULL;
	}
     
      return NewMatrixType;
    }
  else
    {
      _SdifError(eAllocFail, "MatrixType allocation");
      return NULL;
    }
}






void
SdifKillMatrixType(SdifMatrixTypeT *MatrixType)
{

  /* don't kill the predfined type because it is one of another bank of types (Predefined Types)*/

  if (MatrixType)
    {
        SdifKillList(MatrixType->ColumnUserList);
        SdifFree(MatrixType);
    }
  else
    _SdifError(eFreeNull, "MatrixType free");
}







SdifUInt4
SdifMatrixTypeGetNumColumnDef(SdifMatrixTypeT *MatrixType, char *NameCD)
{
    int Num = 0;
    SdifColumnDefT* CurrColumnDef = NULL;

    if (MatrixType->MatrixTypePre)
        Num = SdifMatrixTypeGetNumColumnDef(MatrixType->MatrixTypePre, NameCD);

    if (Num == 0)
    {
        SdifListInitLoop(MatrixType->ColumnUserList);
        while (SdifListIsNext(MatrixType->ColumnUserList))
        {
            CurrColumnDef = SdifListGetNext(MatrixType->ColumnUserList);
            if (SdifStrCmp(CurrColumnDef->Name, NameCD) == 0)
            {
	            Num = CurrColumnDef->Num;
	            break;
	        }
        }
    }
  
  return Num;
}









SdifColumnDefT*
SdifMatrixTypeGetColumnDef(SdifMatrixTypeT *MatrixType, char *NameCD)
{
  SdifColumnDefT *ColumnDef = NULL;
  SdifColumnDefT *CurrColumnDef = NULL;

  if (MatrixType->MatrixTypePre)
    ColumnDef = SdifMatrixTypeGetColumnDef(MatrixType->MatrixTypePre, NameCD);
  
  if (! ColumnDef)
    {
        SdifListInitLoop(MatrixType->ColumnUserList);
        while (SdifListIsNext(MatrixType->ColumnUserList))
        {
            CurrColumnDef = SdifListGetNext(MatrixType->ColumnUserList);
            if (SdifStrCmp(CurrColumnDef->Name, NameCD) == 0)
            {
	            ColumnDef = CurrColumnDef;
	            break;
	        }
        }
    }
  
  return ColumnDef;
}






SdifColumnDefT*
SdifMatrixTypeGetNthColumnDef(SdifMatrixTypeT *MatrixType, SdifUInt4 NumCD)
{
  SdifColumnDefT *ColumnDef = NULL;
  SdifColumnDefT *CurrColumnDef = NULL;

  if (MatrixType->MatrixTypePre)
    ColumnDef = SdifMatrixTypeGetNthColumnDef(MatrixType->MatrixTypePre, NumCD);

  if (! ColumnDef)
    {
        SdifListInitLoop(MatrixType->ColumnUserList);
        while (SdifListIsNext(MatrixType->ColumnUserList))
        {
            CurrColumnDef = SdifListGetNext(MatrixType->ColumnUserList);
            if (CurrColumnDef->Num == NumCD)
            {
	            ColumnDef = CurrColumnDef;
	            break;
	        }
        }
    }
  
  return ColumnDef;
}






SdifMatrixTypeT*
SdifMatrixTypeInsertTailColumnDef(SdifMatrixTypeT *MatrixType, char *NameCD)
{
  
  if (SdifMatrixTypeGetColumnDef(MatrixType, NameCD))
    {
      _SdifError(eExistYet, NameCD);
      return NULL;
    }

  switch (MatrixType->ModifMode)
    {
    case eNoModif:
        _SdifError(eNoModifErr, SdifSignatureToString(MatrixType->Signature));
        return NULL;
    case eCanModif:
        /* then add ColumnDef in the Columns list */
        SdifListPutTail(MatrixType->ColumnUserList,
                        SdifCreateColumnDef(NameCD, MatrixType->NbColumnDef+1));
        break;
    default :
        return NULL;
    }

  MatrixType->NbColumnDef++;
  return MatrixType;
}







SdifMatrixTypeT*
SdifGetMatrixType(SdifHashTableT *MatrixTypesTable, SdifSignature Signature)
{
  return SdifHashTableSearch(MatrixTypesTable, &Signature, 1);
}


void
SdifPutMatrixType(SdifHashTableT *MatrixTypesTable, SdifMatrixTypeT* MatrixType)
{
  SdifHashTablePut(MatrixTypesTable, &(MatrixType->Signature), 1, MatrixType);
}




SdifUInt2
SdifExistUserMatrixType(SdifHashTableT *MatrixTypesTable)
{
  unsigned int     iName;
  SdifHashNT*      pName;
  SdifMatrixTypeT*  MtrxT;
  
  for(iName=0; iName<MatrixTypesTable->HashSize; iName++)
    for (pName = MatrixTypesTable->Table[iName]; pName;  pName=pName->Next)
      {
        MtrxT = (SdifMatrixTypeT*) pName->Data;
        if (SdifListGetNbData(MtrxT->ColumnUserList) > 0)
            return 1;
      }
  return 0;
}
