/* SdifFrameType.h
 *
 * Frame Types management (interpreted sdif frame types)
 *
 * author: Dominique Virolle 1997
 *
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


extern SdifComponentT*  SdifCreateComponent (SdifSignature MatrixSignature, char *Name, SdifUInt4 Num);
extern void             SdifKillComponent   (SdifComponentT *Component);

extern SdifComponentNT* SdifCreateComponentN (SdifComponentNT *Next, SdifComponentT *Component);
extern SdifComponentNT* SdifKillComponentN   (SdifComponentNT *ComponentNode);

extern SdifFrameTypeT*  SdifCreateFrameType (SdifSignature FrameSignature,
					     SdifFrameTypeT *PredefinedFrameType);
extern void             SdifKillFrameType   (SdifFrameTypeT *FrameType);

extern SdifUInt4       SdifFrameTypeGetNumComponent (SdifFrameTypeT *FrameType, char *NameCD);
extern SdifComponentT* SdifFrameTypeGetComponent    (SdifFrameTypeT *FrameType, char *NameCD);
extern SdifComponentT* SdifFrameTypeGetNthComponent (SdifFrameTypeT *FrameType, SdifUInt4 NumC);

extern SdifFrameTypeT* SdifFrameTypeInsertTailComponent(SdifHashTableT *MatrixTypesTable,
							SdifFrameTypeT *FrameType,
							SdifSignature MatrixSignature,
							char *NameC);

extern SdifFrameTypeT* SdifGetFrameType(SdifHashTableT *FrameTypesTable, SdifSignature FrameSiganture);
extern void            SdifPutFrameType(SdifHashTableT *FrameTypesTable, SdifFrameTypeT *FrameType);
extern SdifUInt2       SdifExistUserFrameType(SdifHashTableT *FrameTypesTable);

#endif /* _SdifFrameType_  */
