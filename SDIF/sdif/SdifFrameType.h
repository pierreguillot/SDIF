/* $Id: SdifFrameType.h,v 1.4 1998-11-10 15:31:47 schwarz Exp $
 *
 * SdifFrameType.h
 *
 * Frame Types management (interpreted sdif frame types)
 *
 * author: Dominique Virolle 1997
 *
 * $Log: not supported by cvs2svn $
 */


#ifndef _SdifFrameType_
#define _SdifFrameType_

#include "SdifGlobals.h"
#include "SdifHash.h"

typedef struct SdifComponentS SdifComponentT;
struct SdifComponentS
{
  SdifSignature MatrixSignature;
  char *Name;
  SdifUInt4  Num;
} ;



typedef struct SdifComponentNS SdifComponentNT;
struct SdifComponentNS
{
  SdifComponentNT *Next;
  SdifComponentT *Component;
};



typedef struct SdifFrameTypeS SdifFrameTypeT;
struct SdifFrameTypeS
{
  SdifSignature Signature;

  SdifFrameTypeT* FrameTypePre;

  SdifComponentNT *HeadUse;
  SdifComponentNT *TailUse;
  SdifUInt4       NbComponentUse;

  SdifUInt4       NbComponent;
  SdifModifModeET ModifMode;
};


SdifComponentT*  SdifCreateComponent  (SdifSignature MatrixSignature, 
				       char *Name, 
				       SdifUInt4 Num);
void             SdifKillComponent    (SdifComponentT *Component);

SdifComponentNT* SdifCreateComponentN (SdifComponentNT *Next, 
				       SdifComponentT *Component);
SdifComponentNT* SdifKillComponentN   (SdifComponentNT *ComponentNode);

SdifFrameTypeT*  SdifCreateFrameType  (SdifSignature FrameSignature,
				       SdifFrameTypeT *PredefinedFrameType);
void             SdifKillFrameType    (SdifFrameTypeT *FrameType);

SdifUInt4        SdifFrameTypeGetNumComponent (SdifFrameTypeT *FrameType, 
					       char *NameCD);
SdifComponentT*  SdifFrameTypeGetComponent    (SdifFrameTypeT *FrameType, 
					       char *NameCD);
SdifComponentT*  SdifFrameTypeGetNthComponent (SdifFrameTypeT *FrameType, 
					       SdifUInt4 NumC);
 
SdifFrameTypeT*  SdifFrameTypeInsertTailComponent(SdifHashTableT *MatrixTTable,
						  SdifFrameTypeT *FrameType,
						  SdifSignature MatrixSignatur,
						  char *NameC);
 
SdifFrameTypeT*  SdifGetFrameType       (SdifHashTableT *FrameTypesTable, 
				         SdifSignature FrameSiganture);
void             SdifPutFrameType       (SdifHashTableT *FrameTypesTable, 
				         SdifFrameTypeT *FrameType);
SdifUInt2        SdifExistUserFrameType (SdifHashTableT *FrameTypesTable);

#endif /* _SdifFrameType_  */
