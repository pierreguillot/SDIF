/* SdifFrameType.c
 *
 *
 *
 * SdifHashTableType *gSdifFrameTypesTable is a hash table : SdifGlobals.h
 * gSdifFrameTypesTable        index : frame type name (HashChar, use _SdifNameLen for size)
 *                              Data : Frame type (SdifFrameTypeType*)
 *                            Killer : SdifKillFrameType
 *
 */



#include "SdifFrameType.h"
#include "SdifHash.h"
#include "SdifRWLowLevel.h"
#include "SdifMatrixType.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>








SdifComponentType*
SdifCreateComponent(char *MatrixTypeName, char *Name, SdifUInt4 Num)
{
  SdifComponentType *NewComponent;

  if (NewComponent = (SdifComponentType*) malloc(sizeof(SdifComponentType)))
    {
      NewComponent->Name = SdifCreateStrNCpy(Name, SdifStrLen(Name)+1);
      SdifStrNCpy(NewComponent->MatrixTypeName, MatrixTypeName, _SdifNameLen);
      NewComponent->Num = Num;
      
      return NewComponent;
    }
  else
    {
      _SdifError(eAllocFail, "Component allocation");
      return NULL;
    }
}







void
SdifKillComponent(SdifComponentType *Component)
{
  if (Component)
    {
      SdifKillStr(Component->Name);
      free(Component);
    }
  else
    _SdifError(eFreeNull, "Component free");
}








SdifComponentNodeType*
SdifCreateComponentNode(SdifComponentNodeType *Next, SdifComponentType *Component)
{
  SdifComponentNodeType *NewComponentNode;

  if (NewComponentNode = (SdifComponentNodeType*) malloc (sizeof(SdifComponentNodeType)))
    {
      NewComponentNode->Next = Next;
      NewComponentNode->Component = Component;

      return NewComponentNode;
    }
  else
    {
      _SdifError(eAllocFail, "ComponentNode allocation");
      return NULL;
    }
}







SdifComponentNodeType*
SdifKillComponentNode(SdifComponentNodeType *ComponentNode)
{
  SdifComponentNodeType *Next;

  if (ComponentNode)
    {
      Next = ComponentNode->Next;
      SdifKillComponent(ComponentNode->Component);
      free(ComponentNode);
      
      return Next;
    }
  else
    {
      _SdifError(eFreeNull, "ComponentNode free");
      return NULL;
    }
}








SdifFrameTypeType*
SdifCreateFrameType(char *Name, SdifPredefinedEnum Predefined)
{
  SdifFrameTypeType *NewFrameType;
  
  if (NewFrameType = (SdifFrameTypeType*) malloc (sizeof(SdifFrameTypeType)))
    {
      SdifStrNCpy(NewFrameType->Name, Name, _SdifNameLen);
      NewFrameType->HeadComponentPre = NULL;
      NewFrameType->TailComponentPre = NULL;
      NewFrameType->NbComponentPre   = 0;
      NewFrameType->HeadComponentUse = NULL;
      NewFrameType->TailComponentUse = NULL;
      NewFrameType->NbComponentUse   = 0;
      NewFrameType->NbComponent      = 0;
      NewFrameType->Predefined       = Predefined;
      
      return NewFrameType;
    }
  else
    {
      _SdifError(eAllocFail, "FrameType allocation");
      return NULL;
    }
}








void
SdifKillFrameType(SdifFrameTypeType *FrameType)
{
  if (FrameType)
    {
      while (FrameType->HeadComponentPre)
	FrameType->HeadComponentPre = SdifKillComponentNode(FrameType->HeadComponentPre);

      while (FrameType->HeadComponentUse)
	FrameType->HeadComponentUse = SdifKillComponentNode(FrameType->HeadComponentUse);

      free(FrameType);
    }
  else
    _SdifError(eFreeNull, "FrameType free");
}









SdifFrameTypeType*
SdifGetFrameType(char *Name)
{
  return SdifHashTableSearch(gSdifFrameTypesTable, Name, _SdifNameLen);
}




void
SdifPutFrameType(char *Name, SdifFrameTypeType *FrameType)
{
  SdifHashTablePut(gSdifFrameTypesTable, Name, _SdifNameLen, FrameType);
}





SdifFrameTypeType*
SdifFrameTypeInsertTailComponent(SdifFrameTypeType *FrameType,
				 char *MatrixName,
				 char *NameC,
				 SdifPredefinedEnum Predefined)
{
  SdifComponentNodeType *NewDefNode;
  
  if (SdifFrameTypeGetComponent(FrameType, NameC))
    {
      sprintf(gSdifErrorMess, "Component definition '%s' ", NameC);
      _SdifError(eReDefined, gSdifErrorMess);	      
      return NULL;
    }
  else
    {
      if (SdifGetMatrixType(MatrixName))
	{
	  NewDefNode = SdifCreateComponentNode(NULL,
					       SdifCreateComponent(MatrixName,
								   NameC,
								   ++FrameType->NbComponent));
	  
	  switch (Predefined)
	    {
	    case ePredefined :
	      if (FrameType->Predefined == ePredefined)
		{
		  if (! FrameType->HeadComponentPre)
		    FrameType->HeadComponentPre = NewDefNode;
		  else
		    FrameType->TailComponentPre->Next = NewDefNode;
		  
		  FrameType->TailComponentPre = NewDefNode;
		  FrameType->NbComponentPre++;
		}
	      else
		{
		  _SdifError(eUserdefBefore, NameC);
		  return NULL;
		}
	      break;
	      
	    case eUserdefined :
	      if (! FrameType->HeadComponentUse)
		FrameType->HeadComponentUse = NewDefNode;
	      else
		FrameType->TailComponentUse->Next = NewDefNode;
	      
	      FrameType->TailComponentUse = NewDefNode;
	      FrameType->NbComponentUse++;
	      FrameType->Predefined =eUserdefined;
	      break;
	      
	    default :
	      return NULL;
	    }
	  return FrameType;
	}
      else
	{
	  sprintf(gSdifErrorMess,
		  "Matrix type in Component definition '%c%c%c%c %s' ",
		  MatrixName[0],
		  MatrixName[1],
		  MatrixName[2],
		  MatrixName[3],
		  NameC);
	  _SdifError(eUnDefined, gSdifErrorMess);	      
	  return NULL;
	}
    }
}








SdifUInt4
SdifFrameTypeGetNumComponent(SdifFrameTypeType *FrameType, char *NameC)
{
  SdifComponentNodeType *Node;
  
  for (Node = FrameType->HeadComponentPre; Node; Node = Node->Next)
    if (SdifStrCmp (Node->Component->Name, NameC) == 0)
      return Node->Component->Num;

  for (Node = FrameType->HeadComponentUse; Node; Node = Node->Next)
    if (SdifStrCmp (Node->Component->Name, NameC) == 0)
      return Node->Component->Num;

  return 0;
}







SdifComponentType*
SdifFrameTypeGetComponent(SdifFrameTypeType *FrameType, char *NameC)
{
  SdifComponentNodeType *Node;
  
  for (Node = FrameType->HeadComponentPre; Node; Node = Node->Next)
    if (SdifStrCmp (Node->Component->Name, NameC) == 0)
      return Node->Component;

  for (Node = FrameType->HeadComponentUse; Node; Node = Node->Next)
    if (SdifStrCmp (Node->Component->Name, NameC) == 0)
      return Node->Component;

  return NULL;
}






SdifComponentType*
SdifFrameTypeGetNthComponent(SdifFrameTypeType *FrameType, SdifUInt4 NumC)
{
  SdifComponentNodeType *Node;

  if (NumC <= FrameType->NbComponentPre)
    {
      for (Node = FrameType->HeadComponentPre; Node; Node = Node->Next)
	if (Node->Component->Num == NumC)
	  return Node->Component;
      return NULL;
    }
  else
    {
      for (Node = FrameType->HeadComponentUse; Node; Node = Node->Next)
	if (Node->Component->Num == NumC)
	  return Node->Component;
      return NULL;
    }
}
















int
SdifFWriteOneFrameType(SdifFrameTypeType *FrameType, SdifPredefinedEnum Predefined, FILE *fw)
{
  int NbBytesWrite = 0;
  SdifComponentNodeType *Node;
  
  switch (Predefined)
    {
    case ePredefined :
      if (FrameType->HeadComponentPre)
	{
	  NbBytesWrite += fprintf(fw, "  Fram\t");
	  NbBytesWrite += SdifPrintName(fw, FrameType->Name);
	  NbBytesWrite += fprintf(fw, "\n\t{\n");
	  for(Node = FrameType->HeadComponentPre; Node;  Node = Node->Next)
	    {
	      NbBytesWrite += fprintf(fw, "\t  ");
	      NbBytesWrite += SdifPrintName(fw,  Node->Component->MatrixTypeName);
	      NbBytesWrite += fprintf(fw, "\t%s;\n", Node->Component->Name);
	    }
	  NbBytesWrite += fprintf(fw, "\t}\n");
	}
      break;
	  
    case eUserdefined :
      if (FrameType->HeadComponentUse)
	{
	  NbBytesWrite += fprintf(fw, "  Fram\t");
	  NbBytesWrite += SdifPrintName(fw, FrameType->Name);
	  NbBytesWrite += fprintf(fw, "\n\t{\n");
	  for(Node = FrameType->HeadComponentUse; Node;  Node = Node->Next)
	    {
	      NbBytesWrite += fprintf(fw, "\t  ");
	      NbBytesWrite += SdifPrintName(fw,  Node->Component->MatrixTypeName);
	      NbBytesWrite += fprintf(fw, "\t%s;\n", Node->Component->Name);
	    }
	  NbBytesWrite += fprintf(fw, "\t}\n");
	}
      break;
      
    default :
      return NbBytesWrite;
    }
  return NbBytesWrite;
}








int
SdifFWriteAllFrameType(SdifPredefinedEnum Predefined, FILE *fw)
{
  unsigned int
    iNode;
  SdifHashNodeType
    *pNode;
  int
    NbBytesWrite = 0;
  
  for(iNode=0; iNode<gSdifFrameTypesTable->HashSize; iNode++)
    for(pNode = gSdifFrameTypesTable->Table[iNode]; pNode; pNode = pNode->Next)
      NbBytesWrite += SdifFWriteOneFrameType(pNode->Data, Predefined, fw);
  
  return NbBytesWrite;
}












int
SdifFGetOneComponent(char *MatrixName,
		     char *ComponentName,
		     FILE *fr,
		     int *NbBytesRead)
{
  char
    CharEnd;
  SdifUInt4
    iC;
  
  
  ComponentName[0]= '\0';
  
  /* MatrixName */
  CharEnd = SdifFGetName(MatrixName, fr, NbBytesRead); 
  
  if (CharEnd == (unsigned) '}')
    /* no more Component */
    if  (MatrixName[0] == '\0') 
      return  CharEnd;
    else
      {
	sprintf(gSdifErrorMess,
		"Incomplete Component : '%s%c'",
		MatrixName,
		CharEnd);
	_SdifError(eSyntax, gSdifErrorMess);
	return CharEnd;
      }
  else
    if ( (SdifIsAReservedChar((char) CharEnd) != -1) || (isspace((char) CharEnd)) )
      {
	sprintf(gSdifErrorMess,
		"Component Matrix Name not correctly read : '%c%c%c%c'. Last char read : '%d'",
		MatrixName[0],
		MatrixName[1],
		MatrixName[2],
		MatrixName[3],
		CharEnd);
	_SdifError(eNameLength, gSdifErrorMess);
	return CharEnd;
      }
  
  /* Component Name */
  CharEnd = SdifFGetStringUntil(gSdifString, _SdifStringLen, fr, NbBytesRead, _SdifReservedChars);
  if (CharEnd != (unsigned) ';')
    {
      sprintf(gSdifErrorMess,
	      "Component Name : Attempt to read ';' : '%s%c' ",
	      gSdifString,
	      CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return  CharEnd;
    }
  
  return CharEnd;
}










int
SdifFReadFrameType(SdifPredefinedEnum Predefined, FILE *fr)
{
  SdifFrameTypeType
    *FrameType;
  int
    NbCharRead = 0,
    CharEnd;
  char
    FramName[_SdifNameLen],
    MtrxName[_SdifNameLen];  
  

  /* FramName */
  CharEnd = SdifFGetName(FramName, fr, &NbCharRead);
  if ( (SdifIsAReservedChar((char) CharEnd) != -1) || (isspace((char) CharEnd)) )
    {
      sprintf(gSdifErrorMess,
	      "Frame Name not correctly read : '%c%c%c%c'. Last char read : '%d'",
	      FramName[0],
	      FramName[1],
	      FramName[2],
	      FramName[3],
	      (char) CharEnd);
      _SdifError(eNameLength, gSdifErrorMess);
      return NbCharRead;
    }

 
  /* Frame type Creation, Put or Recuperation from gSdifFrameTypesTable */
  if (! (FrameType =  SdifGetFrameType(FramName)))
    {
      FrameType = SdifCreateFrameType(FramName, Predefined);
      SdifPutFrameType(FramName, FrameType);
    }

  
  /* Components */
  CharEnd = SdifFGetStringUntil(gSdifString, _SdifStringLen, fr, &NbCharRead, _SdifReservedChars);
  if ( (CharEnd != (int) '{') || (SdifStrLen(gSdifString)!=0) )
    {
      sprintf(gSdifErrorMess, "Attempt to read '{' failed : '%s%c'", gSdifString, CharEnd);
      _SdifError(eSyntax, gSdifErrorMess);
      return NbCharRead;
    }
  else
    {
      while ( (CharEnd = SdifFGetOneComponent(MtrxName, gSdifString, fr, &NbCharRead)) != (unsigned) '}' )
	SdifFrameTypeInsertTailComponent(FrameType, MtrxName, gSdifString, Predefined);	
    }
    
  return NbCharRead;
}
