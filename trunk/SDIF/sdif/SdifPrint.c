/* SdifPrint.c
 *
 * for debug
 * print of structures with some reference
 * FILE* is usually stdout or stderr
 *
 * author: Dominique Virolle 1997
 *
 */


#include "SdifPrint.h"


/*************** Matrix Type ***************/



void
SdifPrintMatrixType(FILE *fw, SdifMatrixTypeT *MatrixType)
{
  SdifColumnDefNT *Node;

  fprintf(fw, "  %s  %s",
	  SdifSignatureToString(e1MTD),
	  SdifSignatureToString(MatrixType->Signature));

  if (MatrixType->MatrixTypePre)
    {
      if (MatrixType->MatrixTypePre->HeadUse)
	{
	  fprintf(fw, "\n    Pre {");
	  for(Node = MatrixType->MatrixTypePre->HeadUse; Node->Next;  Node = Node->Next)
	    fprintf(fw, "%s(%d), ",Node->ColumnDef->Name, Node->ColumnDef->Num);
	  fprintf(fw, "%s(%d)}",Node->ColumnDef->Name, Node->ColumnDef->Num);  
	}
    }

  if (MatrixType->HeadUse)
    {
      fprintf(fw, "\n    Use {");
      for(Node = MatrixType->HeadUse; Node->Next;  Node = Node->Next)
	fprintf(fw, "%s(%d), ",Node->ColumnDef->Name, Node->ColumnDef->Num);
      fprintf(fw, "%s(%d)}",Node->ColumnDef->Name, Node->ColumnDef->Num);
    }
  fprintf(fw, "\n\n");
}





void
SdifPrintAllMatrixType(FILE *fw, SdifFileT* SdifF)
{
  unsigned int
    iName;
  SdifHashNT
    *pName;
  
  for(iName=0; iName<SdifF->MatrixTypesTable->HashSize; iName++)
    for (pName = SdifF->MatrixTypesTable->Table[iName]; pName;  pName=pName->Next)
      SdifPrintMatrixType(fw, pName->Data);
  
}




/*************** Frame Type ***************/

void
SdifPrintFrameType(FILE *fw, SdifFrameTypeT *FrameType)
{
  SdifUInt4 iC;
  SdifComponentT* Component;
  
  fprintf(fw, "  %s  %s",
	  SdifSignatureToString(e1FTD),
	  SdifSignatureToString(FrameType->Signature));

  if (FrameType->FrameTypePre)
    {
      if (FrameType->FrameTypePre->NbComponentUse > 0)
        {
	      fprintf(fw, "\n    Pre {\n");
          for(iC = 1;
              iC<= FrameType->FrameTypePre->NbComponentUse;
              iC++)
	        {
              Component = SdifFrameTypeGetNthComponent(FrameType->FrameTypePre, iC);
	          fprintf(fw, "          ");
	          fprintf(fw, "%s  %s(%d);\n",
		              SdifSignatureToString(Component->MtrxS),
                      Component->Name,
                      Component->Num);
              }
          fprintf(fw, "        }");
	    }
    }
  
    if (FrameType->NbComponentUse > 0)
      {
	    fprintf(fw, "\n    Use {\n");
        for(iC = 1;
            iC<= FrameType->NbComponentUse;
            iC++)
	      {
            Component = SdifFrameTypeGetNthComponent(FrameType, iC);
	        fprintf(fw, "          ");
	        fprintf(fw, "%s  %s(%d);\n",
	             SdifSignatureToString(Component->MtrxS),
                    Component->Name,
                    Component->Num);
            }
        fprintf(fw, "        }");
	  }

  fprintf(fw, "\n\n");
}




void
SdifPrintAllFrameType(FILE *fw, SdifFileT* SdifF)
{
  unsigned int
    iNode;
  SdifHashNT
    *pNode;
  
  for(iNode=0; iNode<SdifF->FrameTypesTable->HashSize; iNode++)
    for(pNode = SdifF->FrameTypesTable->Table[iNode]; pNode; pNode = pNode->Next)
      SdifPrintFrameType(fw, pNode->Data);
  
}


/********** Matrix **********/

void
SdifPrintMatrixHeader(FILE *f, SdifMatrixHeaderT *MatrixHeader)
{
  fprintf(f,
	  "%s  DataWidth: 0x%04x       Rows: %d   \t   Columns: %d\n",
	  SdifSignatureToString(MatrixHeader->Signature),
	  MatrixHeader->DataType,
	  MatrixHeader->NbRow,
	  MatrixHeader->NbCol);
}


void
SdifPrintOneRow(FILE *f, SdifOneRowT *OneRow)
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
      fprintf(f, "data type not supported: 0x%x\n", OneRow->DataType);
      break;
    }
  fprintf(f, "\n");
}

/********** Frame ***********/

void
SdifPrintFrameHeader(FILE *f, SdifFrameHeaderT* FrameHeader)
{
  fprintf(f,
	  "%s         Size: 0x%04x   NbMatrix: %u    \t NumID: %u     \t Time: %g\n",
	  SdifSignatureToString(FrameHeader->Signature),
	  FrameHeader->Size,
	  FrameHeader->NbMatrix,
	  FrameHeader->NumID,
	  FrameHeader->Time);
}


/************ High ***********/

void
SdifPrintAllType(FILE *fw, SdifFileT* SdifF)
{
  fprintf(fw, "%s\n{\n", SdifSignatureToString(e1TYP));
  SdifPrintAllMatrixType(fw, SdifF);
  SdifPrintAllFrameType(fw, SdifF);  
  fprintf(fw, "}\n\n");
}


