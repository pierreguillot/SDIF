/* $Id: SdifMatrixType.h,v 2.3 1999-01-23 15:55:55 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifMatrixType.h
 *
 * Matrix Types management (interpreted sdif frame types)
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 * Revision 2.2  1999/01/23  13:57:41  virolle
 * General Lists, and special chunk preparation to become frames
 *
 * Revision 2.1  1998/12/21  18:27:31  schwarz
 * Inserted copyright message.
 *
 * Revision 2.0  1998/11/29  11:41:57  virolle
 * - New management of interpretation errors.
 * - Alignement of frames with CNMAT (execpt specials Chunk 1NVT, 1TYP, 1IDS).
 * _ Sdif Header File has a Sdif format version.
 * - Matrices order in frames is not important now. (only one occurence of
 *   a Matrix Type in a Frame Type declaration )
 * - Hard coded predefined types more dynamic management.
 * - Standart streams (stdin, stdout, stderr) set as binary for Windows32 to
 *   have exactly the same result on each plateforme.
 *
 * Revision 1.4  1998/11/10  15:31:52  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifMatrixType_
#define _SdifMatrixType_

#include "SdifGlobals.h"
#include "SdifList.h"
#include "SdifHash.h"




typedef struct SdifColumnDefS SdifColumnDefT;

struct SdifColumnDefS
{
  char *Name;
  SdifUInt4 Num;
} ;





typedef struct SdifMatrixTypeS SdifMatrixTypeT;

struct SdifMatrixTypeS
{
  SdifSignature     Signature;

  SdifMatrixTypeT*  MatrixTypePre;

  SdifListT*        ColumnUserList;

  SdifUInt4       NbColumnDef;
  SdifModifModeET ModifMode;
};




SdifColumnDefT*  SdifCreateColumnDef (char *Name,  unsigned int Num);
void             SdifKillColumnDef   (SdifColumnDefT *ColumnDef);

SdifMatrixTypeT* SdifCreateMatrixType              (SdifSignature Signature,
                    							   SdifMatrixTypeT *PredefinedMatrixType);
void             SdifKillMatrixType                (SdifMatrixTypeT *MatrixType);
SdifMatrixTypeT* SdifMatrixTypeInsertTailColumnDef (SdifMatrixTypeT *MatrixType, char *NameCD);
SdifUInt4        SdifMatrixTypeGetNumColumnDef     (SdifMatrixTypeT *MatrixType, char *NameCD);
SdifColumnDefT*  SdifMatrixTypeGetColumnDef        (SdifMatrixTypeT *MatrixType, char *NameCD);
SdifColumnDefT*  SdifMatrixTypeGetNthColumnDef     (SdifMatrixTypeT *MatrixType, SdifUInt4 NumCD);

SdifMatrixTypeT* SdifGetMatrixType(SdifHashTableT *MatrixTypesTable, SdifSignature Signature);
void             SdifPutMatrixType(SdifHashTableT *MatrixTypesTable, SdifMatrixTypeT* MatrixType);
SdifUInt2        SdifExistUserMatrixType(SdifHashTableT *MatrixTypesTable);

#endif /* _SdifMatrixType_  */
