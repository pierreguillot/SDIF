/* $Id: SdifPrint.c,v 3.6 2000-11-15 14:53:33 lefevre Exp $
 *
 * IRCAM SDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 1998, 1999, 2000 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            sdif@ircam.fr
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
 * Revision 3.5  2000/10/27  20:03:41  roebel
 * autoconf merged back to main trunk
 *
 * Revision 3.4.2.1  2000/08/21  21:35:42  tisseran
 * *** empty log message ***
 *
 * Revision 3.4  2000/05/15  16:23:10  schwarz
 * Avoided avoidable warnings.
 *
 * Revision 3.3  1999/10/13  16:05:54  schwarz
 * Changed data type codes (SdifDataTypeET) to SDIF format version 3, as
 * decided with Matt Wright June 1999, added integer data types.
 * Added writing of 1NVT with real frame header (but data is still not in
 * matrices).
 * The data type handling makes heavy use of code-generating macros,
 * called for all data types with the sdif_foralltypes macro, thus
 * adding new data types is easy.
 *
 * Revision 3.2  1999/09/28  13:09:09  schwarz
 * Included #include <preincluded.h> for cross-platform uniformisation,
 * which in turn includes host_architecture.h and SDIF's project_preinclude.h.
 *
 * Revision 3.1  1999/03/14  10:57:16  virolle
 * SdifStdErr add
 *
 * Revision 2.3  1999/01/23  15:55:58  virolle
 * add querysdif.dsp, delete '\r' chars from previous commit
 *
 * Revision 2.2  1999/01/23  13:57:45  virolle
 * General Lists, and special chunk preparation to become frames
 *
 *
 *
 *
 */


#include <preincluded.h>

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


/* static const char *formatText	  = "%d  ";   todo */
static const char *formatChar     = "%d  ";
static const char *formatFloat4   = "%8g  ";
static const char *formatFloat8   = "%8g  ";
static const char *formatInt2     = "%hd  ";
static const char *formatInt4     = "%d  ";
static const char *formatUInt2    = "%hu  ";
static const char *formatUInt4    = "%u  ";
/* l or ll?
static const char *formatInt8     = "%ld  ";
static const char *formatUInt8    = "%lu  ";
*/


void
SdifPrintOneRow(FILE *f, SdifOneRowT *OneRow)
{
    int iCol;

    /* case template for type from SdifDataTypeET */
#   define printrowcase(type)						     \
    case e##type:   for (iCol = 0; iCol < OneRow->NbData; iCol++)	     \
			fprintf (f, format##type, OneRow->Data.type [iCol]); \
    break;

  switch (OneRow->DataType)
    {
      /* generate cases for all types */
      sdif_foralltypes (printrowcase);

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


