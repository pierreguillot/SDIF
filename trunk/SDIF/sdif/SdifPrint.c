/* $Id: SdifPrint.c,v 2.3 1999-01-23 15:55:58 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * for debug
 * print of structures with some reference
 * FILE* is usually stdout or stderr
 *
 * author: Dominique Virolle 1997
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.2  1999/01/23  13:57:45  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 *
 */


#include "SdifPrint.h"


/*************** Matrix Type ***************/



void
SdifPrintMatrixType(FILE *fw, SdifMatrixTypeT *MatrixType)
{
    SdifColumnDefT *ColumnDef;
    fprintf(fw, "  %s  %s",
        SdifSignatureToString(e1MTD),
        SdifSignatureToString(MatrixType->Signature));

    if (MatrixType->MatrixTypePre)
    {
        if (!SdifListIsEmpty(MatrixType->MatrixTypePre->ColumnUserList))
        {
            ColumnDef = SdifListGetHead(MatrixType->MatrixTypePre->ColumnUserList); /* Reinit GetNext*/
            fprintf(fw, "\n    Pred {%s(%d)", ColumnDef->Name, ColumnDef->Num);
            while (SdifListIsNext(MatrixType->MatrixTypePre->ColumnUserList))
            {
                ColumnDef = SdifListGetNext(MatrixType->MatrixTypePre->ColumnUserList);
                fprintf(fw, ", %s(%d)",ColumnDef->Name, ColumnDef->Num);
            }
    	    fprintf(fw, "}");  
        }
    }

    if (!SdifListIsEmpty(MatrixType->ColumnUserList))
    {
        ColumnDef = SdifListGetHead(MatrixType->ColumnUserList); /* Reinit GetNext*/
        fprintf(fw, "\n    User {%s(%d)", ColumnDef->Name, ColumnDef->Num);
        while (SdifListIsNext(MatrixType->ColumnUserList))
        {
            ColumnDef = SdifListGetNext(MatrixType->ColumnUserList);
            fprintf(fw, ", %s(%d)",ColumnDef->Name, ColumnDef->Num);
        }
 	    fprintf(fw, "}");  
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


