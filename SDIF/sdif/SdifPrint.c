/* SdifPrint.c
 *
 * for debug
 * print of structures with some reference
 * FILE* is usually stdout or stderr
 *
 */


#include "SdifPrint.h"


/*************** Matrix Type ***************/



void
SdifPrintMatrixType(FILE *fw, SdifMatrixTypeType *MatrixType)
{
  SdifColumnDefNodeType *Node;

  fprintf(fw, "  Mtrx  ");
  SdifPrintName(fw, MatrixType->Name);
  if (MatrixType->HeadColumnDefPre)
    {
      fprintf(fw, "\n    Pre {");
      for(Node = MatrixType->HeadColumnDefPre; Node->Next;  Node = Node->Next)
	fprintf(fw, "%s(%d), ",Node->ColumnDef->Name, Node->ColumnDef->Num);
      fprintf(fw, "%s(%d)}",Node->ColumnDef->Name, Node->ColumnDef->Num);  
    }
  if (MatrixType->HeadColumnDefUse)
    {
      fprintf(fw, "\n    Use {");
      for(Node = MatrixType->HeadColumnDefUse; Node->Next;  Node = Node->Next)
	fprintf(fw, "%s(%d), ",Node->ColumnDef->Name, Node->ColumnDef->Num);
      fprintf(fw, "%s(%d)}",Node->ColumnDef->Name, Node->ColumnDef->Num);
    }
  fprintf(fw, "\n\n");
}


void
SdifPrintAllMatrixType(FILE *fw)
{
  unsigned int
    iName;
  SdifHashNodeType
    *pName;
  
  for(iName=0; iName<gSdifMatrixTypesTable->HashSize; iName++)
    for (pName = gSdifMatrixTypesTable->Table[iName]; pName;  pName=pName->Next)
      SdifPrintMatrixType(fw, pName->Data);
  
}

/*************** Frame Type ***************/

void
SdifPrintFrameType(FILE *fw, SdifFrameTypeType *FrameType)
{
  SdifComponentNodeType *Node;

  fprintf(fw, "  Fram  ");
  SdifPrintName(fw, FrameType->Name);
  if (FrameType->HeadComponentPre)
    {
      fprintf(fw, "\n    Pre {\n");
      for(Node = FrameType->HeadComponentPre; Node;  Node = Node->Next)
	{
	  fprintf(fw, "          ");
	  SdifPrintName(fw,  Node->Component->MatrixTypeName);
	  fprintf(fw, "  %s(%d);\n", Node->Component->Name, Node->Component->Num);
	}
      fprintf(fw, "        }");
    }
  if (FrameType->HeadComponentUse)
    {
      fprintf(fw, "\n    Use {\n");
      for(Node = FrameType->HeadComponentUse; Node;  Node = Node->Next)
	{
	  fprintf(fw, "          ");
	  SdifPrintName(fw,  Node->Component->MatrixTypeName);
	  fprintf(fw, "  %s(%d);\n", Node->Component->Name, Node->Component->Num);
	}
      fprintf(fw, "        }");
    }
  fprintf(fw, "\n\n");
}




void
SdifPrintAllFrameType(FILE *fw)
{
  unsigned int
    iNode;
  SdifHashNodeType
    *pNode;
  
  for(iNode=0; iNode<gSdifFrameTypesTable->HashSize; iNode++)
    for(pNode = gSdifFrameTypesTable->Table[iNode]; pNode; pNode = pNode->Next)
      SdifPrintFrameType(fw, pNode->Data);
  
}


/********** Matrix **********/

void
SdifPrintMatrixHeader(FILE *f, SdifMatrixHeaderType *MatrixHeader)
{
  SdifPrintName(f, MatrixHeader->MatrixName);
  fprintf(f,
	  "  DW: 0x%x     R: %d     C: %d\n",
	  MatrixHeader->DataType,
	  MatrixHeader->NbRow,
	  MatrixHeader->NbCol);
}


void
SdifPrintOneRow(FILE *f, SdifOneRowType *OneRow)
{
  unsigned int iCol;
  
  switch (OneRow->DataType)
    {
    case eFloat4 :
      for(iCol=0; iCol<OneRow->NbData; iCol++)
	fprintf(f, "%8g  ", OneRow->Data.F4[iCol]);
      break;
    case eFloat8 :
      for(iCol=0; iCol<OneRow->NbData; iCol++)
	fprintf(f, "%8g  ", OneRow->Data.F8[iCol]);
      break;
    default :
      sprintf(gSdifErrorMess, "Data of a OneRow : 0x%x", OneRow->DataType);
      _SdifError(eTypeDataNotSupported, gSdifErrorMess);
      break;
    }
  fprintf(f, "\n");
}

void
SdifPrintMatrixRows(FILE* f, SdifMatrixDataType *MatrixData)
{
  SdifUInt4 iRow;

  for(iRow=0; iRow<MatrixData->Header->NbRow; iRow++)
    {
      fprintf(f, "  ");
      SdifPrintOneRow(f, MatrixData->Rows[iRow]);
    }
}

void
SdifPrintMatrixData(FILE* f, SdifMatrixDataType *MtrxD)
{
  SdifMatrixTypeType *MtrxT;
  SdifColumnDefNodeType *Node;
  SdifUInt4 iCol;
  
  SdifPrintMatrixHeader(f, MtrxD->Header);
  fprintf(f, "  ");  
  if (MtrxT = SdifGetMatrixType(MtrxD->Header->MatrixName))
    {
      iCol = 0;
      if (MtrxT->HeadColumnDefPre)
	for(Node = MtrxT->HeadColumnDefPre;
	    Node && (iCol<MtrxD->Header->NbCol);
	      Node = Node->Next)
	  {
	    fprintf(f, "%8s  ",Node->ColumnDef->Name);
	    iCol++;
	  }
      if (MtrxT->HeadColumnDefUse)
	for(Node = MtrxT->HeadColumnDefUse;
	    Node && (iCol<MtrxD->Header->NbCol);
	    Node = Node->Next)
	  {
	    fprintf(f, "%8s  ",Node->ColumnDef->Name);
	    iCol++;
	  }
    }
  fprintf(f, "\n");  
  SdifPrintMatrixRows(f, MtrxD);
}

/********** Frame ***********/

void
SdifPrintFrameHeader(FILE *f, SdifFrameHeaderType* FrameHeader)
{
  SdifPrintName(f, FrameHeader->FrameName);
  fprintf(f,
	  "  S: 0x%x    Mc: %u     ID: %u     T: %g\n",
	  FrameHeader->Size,
	  FrameHeader->NbMatrix,
	  FrameHeader->NumID,
	  FrameHeader->Time);
}


void
SdifPrintFrameData(FILE *f, SdifFrameDataType *FrameData)
{
  SdifFrameTypeType *FrameType;
  SdifComponentType *Component;
  unsigned int iMtrxD;  
  
  FrameType = SdifGetFrameType(FrameData->Header->FrameName);
  
  SdifPrintFrameHeader(f, FrameData->Header);
  if(FrameData->Matrix_s)
    for(iMtrxD=0; iMtrxD<FrameData->Header->NbMatrix; iMtrxD++)
      if (FrameData->Matrix_s[iMtrxD])
	{
	  Component = SdifFrameTypeGetNthComponent(FrameType, iMtrxD+1);
	  fprintf(f, "'%s'\n", Component->Name);
	  SdifPrintMatrixData(f, SdifFrameDataGetNthMatrixData(FrameData, Component->Num));
	}
  fprintf(f,"\n");
}


/************ High ***********/

void
SdifPrintAllType(FILE *fw)
{
  fprintf(fw, "%s\n{\n", _STYP);
  SdifPrintAllMatrixType(fw);
  SdifPrintAllFrameType(fw);  
  fprintf(fw, "}\n\n");
}
