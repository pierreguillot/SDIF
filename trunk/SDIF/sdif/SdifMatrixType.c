/* SdifMatrixType.c
 *
 *  
 *
 *
 * SdifHashTableType *gSdifMatrixTypesTable 
 *      index : matrix type name (HashChar : 4 bytes)
 *       Data : matrix type (SdifMatrixTypeType*)
 *     Killer : SdifKillMatrixType
 */


#include "SdifMatrixType.h"
#include "SdifHash.h"
#include "SdifRWLowLevel.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>







SdifColumnDefType*
SdifCreateColumnDef(char *Name, SdifUInt4 Num)
{
  SdifColumnDefType *NewColumnDef;
  
  NewColumnDef = (SdifColumnDefType*) malloc (sizeof(SdifColumnDefType));
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
SdifKillColumnDef(SdifColumnDefType *ColumnDef)
{
  if (ColumnDef)
    {
      SdifKillStr(ColumnDef->Name);
      free(ColumnDef);
    }
  else
    _SdifError(eFreeNull, "ColumnDef free");
}







SdifColumnDefNodeType*
SdifCreateColumnDefNode(SdifColumnDefNodeType *Next, SdifColumnDefType *ColumnDef)
{
  SdifColumnDefNodeType *NewColDefNode;

  if (NewColDefNode = (SdifColumnDefNodeType*) malloc (sizeof(SdifColumnDefNodeType)))
    {
      NewColDefNode->Next = Next;
      NewColDefNode->ColumnDef = ColumnDef;

      return NewColDefNode;
    }
  else
    {
      _SdifError(eAllocFail, "ColumnDefNode allocation");
      return NULL;
    }
}






SdifColumnDefNodeType*
SdifKillColumnDefNode(SdifColumnDefNodeType *ColDefNode)
{
  SdifColumnDefNodeType *Next;

  if (ColDefNode)
    {
      Next = ColDefNode->Next;
      SdifKillColumnDef(ColDefNode->ColumnDef);
      free(ColDefNode);
      
      return Next;
    }
  else
    {
      _SdifError(eFreeNull, "ColumnDefNode free");
      return NULL;
    }
}







SdifMatrixTypeType*
SdifCreateMatrixType(char *Name, SdifPredefinedEnum Predefined)
{
  SdifMatrixTypeType *NewMatrixType;

  if (NewMatrixType = (SdifMatrixTypeType*) malloc (sizeof(SdifMatrixTypeType)))
    {
      SdifStrNCpy(NewMatrixType->Name, Name, _SdifNameLen);
      NewMatrixType->HeadColumnDefPre = NULL;
      NewMatrixType->TailColumnDefPre = NULL;
      NewMatrixType->NbColumnDefPre = 0;
      NewMatrixType->HeadColumnDefUse = NULL;
      NewMatrixType->TailColumnDefUse = NULL;
      NewMatrixType->NbColumnDefUse = 0;
      NewMatrixType->NbColumnDef = 0;
      NewMatrixType->Predefined = Predefined;
      
      return NewMatrixType;
    }
  else
    {
      _SdifError(eAllocFail, "MatrixType allocation");
      return NULL;
    }
}






void
SdifKillMatrixType(SdifMatrixTypeType *MatrixType)
{
  if (MatrixType)
    {
      while (MatrixType->HeadColumnDefPre)
	MatrixType->HeadColumnDefPre = SdifKillColumnDefNode(MatrixType->HeadColumnDefPre);

      while (MatrixType->HeadColumnDefUse)
	MatrixType->HeadColumnDefUse = SdifKillColumnDefNode(MatrixType->HeadColumnDefUse);

      free(MatrixType);
    }
  else
    _SdifError(eFreeNull, "MatrixType free");
}









SdifMatrixTypeType*
SdifGetMatrixType(char *Name)
{
  return SdifHashTableSearch(gSdifMatrixTypesTable, Name, _SdifNameLen);
}







void
SdifPutMatrixType(char *Name, SdifMatrixTypeType* MatrixType)
{
  SdifHashTablePut(gSdifMatrixTypesTable, Name, _SdifNameLen, MatrixType);
}






SdifMatrixTypeType*
SdifMatrixTypeInsertTailColumnDef(SdifMatrixTypeType *MatrixType,
				  char *NameCD,
				  SdifPredefinedEnum Predefined)
{
  SdifColumnDefNodeType *NewDefNode;
  
  if (SdifMatrixTypeGetColumnDef(MatrixType, NameCD))
    {
      sprintf(gSdifErrorMess, "Column definition '%s' ", NameCD);
      _SdifError(eReDefined, gSdifErrorMess);	      
      return NULL;
    }
  else
    {
      NewDefNode = SdifCreateColumnDefNode(NULL, SdifCreateColumnDef(NameCD, ++MatrixType->NbColumnDef));
      
      switch (Predefined)
	{
	case ePredefined :       /* then add ColumnDef in the Predefined Columns list */
	  if (MatrixType->Predefined == ePredefined)
	    {
	      if (! MatrixType->HeadColumnDefPre)
		MatrixType->HeadColumnDefPre = NewDefNode;
	      else
		MatrixType->TailColumnDefPre->Next = NewDefNode;
	      
	      MatrixType->TailColumnDefPre = NewDefNode;
	      MatrixType->NbColumnDefPre++;
	    }
	  else
	    {
	      _SdifError(eUserdefBefore, NameCD);
	      return NULL;
	    }
	  break;
	  
	case eUserdefined :      /* then add ColumnDef in the Userdefined Columns list */
	  if (! MatrixType->HeadColumnDefUse)
	    MatrixType->HeadColumnDefUse = NewDefNode;
	  else
	    MatrixType->TailColumnDefUse->Next = NewDefNode;
	  
	  MatrixType->TailColumnDefUse = NewDefNode;
	  MatrixType->NbColumnDefUse++;
	  MatrixType->Predefined = eUserdefined;
	  break;
	  
	default :
	  return NULL;
	}
      return MatrixType;
    }
}








SdifUInt4
SdifMatrixTypeGetNumColumnDef(SdifMatrixTypeType *MatrixType, char *NameCD)
{
  SdifColumnDefNodeType *Node;
  
  for (Node = MatrixType->HeadColumnDefPre; Node; Node = Node->Next)
    if (SdifStrCmp (Node->ColumnDef->Name, NameCD) == 0)
      return Node->ColumnDef->Num;
  
  for (Node = MatrixType->HeadColumnDefUse; Node; Node = Node->Next)
    if (SdifStrCmp (Node->ColumnDef->Name, NameCD) == 0)
      return Node->ColumnDef->Num;
  
  return 0;
}









SdifColumnDefType*
SdifMatrixTypeGetColumnDef(SdifMatrixTypeType *MatrixType, char *NameCD)
{
  SdifColumnDefNodeType *Node;
  
  for (Node = MatrixType->HeadColumnDefPre; Node; Node = Node->Next)
    if (SdifStrCmp (Node->ColumnDef->Name, NameCD) == 0)
      return Node->ColumnDef;
  
  for (Node = MatrixType->HeadColumnDefUse; Node; Node = Node->Next)
    if (SdifStrCmp (Node->ColumnDef->Name, NameCD) == 0)
      return Node->ColumnDef;
  
  return NULL;
}








SdifColumnDefType*
SdifMatrixTypeGetNthColumnDef(SdifMatrixTypeType *MatrixType, SdifUInt4 NumCD)
{
  SdifColumnDefNodeType *Node;

  if (NumCD <= MatrixType->NbColumnDefPre)
    {
      for (Node = MatrixType->HeadColumnDefPre; Node; Node = Node->Next)
	if (Node->ColumnDef->Num == NumCD)
	  return Node->ColumnDef;
      return NULL;
    }
  else
    {
      for (Node = MatrixType->HeadColumnDefUse; Node; Node = Node->Next)
	if (Node->ColumnDef->Num == NumCD)
	  return Node->ColumnDef;
      return NULL;
    }
}








int
SdifFWriteOneMatrixType(SdifMatrixTypeType *MatrixType, SdifPredefinedEnum Predefined, FILE *fw)
{
  SdifColumnDefNodeType *Node;
  int NbBytesWrite = 0;

  switch (Predefined)
    {
    case ePredefined :
      if (MatrixType->HeadColumnDefPre)
	{
	  NbBytesWrite += fprintf(fw, "  Mtrx\t");
	  NbBytesWrite += SdifPrintName(fw, MatrixType->Name);
	  NbBytesWrite += fprintf(fw, "\t{");
	  for(Node = MatrixType->HeadColumnDefPre; Node->Next;  Node = Node->Next)
	    NbBytesWrite += fprintf(fw, "%s, ",Node->ColumnDef->Name);
	  NbBytesWrite += fprintf(fw, "%s}\n",Node->ColumnDef->Name);  
	}
      break;
    case eUserdefined :
      if (MatrixType->HeadColumnDefUse)
	{
	  NbBytesWrite += fprintf(fw, "  Mtrx\t");
	  NbBytesWrite += SdifPrintName(fw, MatrixType->Name);
	  NbBytesWrite += fprintf(fw, "\t{");
	  for(Node = MatrixType->HeadColumnDefUse; Node->Next;  Node = Node->Next)
	    NbBytesWrite += fprintf(fw, "%s, ",Node->ColumnDef->Name);
	  NbBytesWrite += fprintf(fw, "%s}\n",Node->ColumnDef->Name);  
	}
      break;
    default :
      return 0;
    }
  return NbBytesWrite;
}







int
SdifFWriteAllMatrixType(SdifPredefinedEnum Predefined, FILE *fw)
{
  unsigned int
    iName;
  SdifHashNodeType
    *pName;
  int
    NbBytesWrite = 0;
  
  for(iName=0; iName<gSdifMatrixTypesTable->HashSize; iName++)
    for (pName = gSdifMatrixTypesTable->Table[iName]; pName;  pName=pName->Next)
      NbBytesWrite += SdifFWriteOneMatrixType(pName->Data, Predefined, fw);
  
  return NbBytesWrite;
}





int
SdifFReadMatrixType(SdifPredefinedEnum Predefined, FILE *fr)
{
  SdifMatrixTypeType
    *MatrixType;
  int
    NbCharRead = 0,
    CharEnd;
  
  char Name[_SdifNameLen];  

  CharEnd = SdifFGetName(Name, fr, &NbCharRead);
  if ( (SdifIsAReservedChar((char) CharEnd) != -1) || (isspace((char) CharEnd)) )
    {
      sprintf(gSdifErrorMess,
	      "Matrix Name not correctly read : '%c%c%c%c'. Last char read : '%d'",
	      Name[0],
	      Name[1],
	      Name[2],
	      Name[3],
	      (char) CharEnd);
      _SdifError(eNameLength, gSdifErrorMess);
      return NbCharRead;
    }
  
  
  /* Matrix type Creation, Put or Recuperation from gSdifMatrixTypesTable */
  if (! (MatrixType = SdifGetMatrixType(Name)))
    {
      MatrixType = SdifCreateMatrixType(Name, Predefined);
      SdifPutMatrixType(Name, MatrixType);
    }
  
  /* ColumnDefs */
  CharEnd = SdifFGetStringUntil(gSdifString, _SdifStringLen, fr, &NbCharRead, _SdifReservedChars);
  if ( (CharEnd != (int) '{') || (SdifStrLen(gSdifString)!=0) )
    {
      sprintf(gSdifErrorMess, "Attempt to read '{' failed : '%s%c'", gSdifString, CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return NbCharRead;
    }
  else
    {
      while ((CharEnd = SdifFGetStringUntil(gSdifString,
					    _SdifStringLen,
					    fr,
					    &NbCharRead,
					    _SdifReservedChars)) == (int) ',')
	{
	  SdifMatrixTypeInsertTailColumnDef(MatrixType, gSdifString, Predefined);
	}

      if (CharEnd != (int) '}')
	{
	  sprintf(gSdifErrorMess, "Attempt to read '}' failed : '%s%c' ", gSdifString, CharEnd);
	  _SdifError(eSyntax, gSdifErrorMess);
	  return NbCharRead;
	}
      else
	if (SdifStrLen(gSdifString) != 0)
	  SdifMatrixTypeInsertTailColumnDef(MatrixType, gSdifString, Predefined);
    }
  return NbCharRead;
}
