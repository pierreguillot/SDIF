/* SdifFrameType.h
 *
 *
 *
 */


#ifndef _SdifFrameType_
#define _SdifFrameType_

#include "SdifGlobals.h"

#include <stdio.h>

typedef struct SdifComponentT
{
  char MatrixTypeName[_SdifNameLen];
  char *Name;
  SdifUInt4  Num;
} SdifComponentType;

extern SdifComponentType*
SdifCreateComponent(char *MatrixTypeName, char *Name, SdifUInt4 Num);

extern void
SdifKillComponent(SdifComponentType *Component);




typedef struct SdifComponentNodeT SdifComponentNodeType;

struct SdifComponentNodeT
{
  SdifComponentNodeType *Next;
  SdifComponentType *Component;
};

extern SdifComponentNodeType*
SdifCreateComponentNode(SdifComponentNodeType *Next, SdifComponentType *Component);

extern SdifComponentNodeType*
SdifKillComponentNode(SdifComponentNodeType *ComponentNode);




typedef struct SdifFrameTypeT
{
  char         Name[_SdifNameLen];

  SdifComponentNodeType *HeadComponentPre;
  SdifComponentNodeType *TailComponentPre;
  SdifUInt4             NbComponentPre;

  SdifComponentNodeType *HeadComponentUse;
  SdifComponentNodeType *TailComponentUse;
  SdifUInt4             NbComponentUse;

  SdifUInt4             NbComponent;
  SdifPredefinedEnum    Predefined;
} SdifFrameTypeType;

extern SdifFrameTypeType*
SdifCreateFrameType(char *Name, SdifPredefinedEnum Predefined);

extern void
SdifKillFrameType(SdifFrameTypeType *FrameType);

extern SdifFrameTypeType*
SdifGetFrameType(char *Name);

extern void
SdifPutFrameType(char *Name, SdifFrameTypeType* FrameType);

extern SdifFrameTypeType*
SdifFrameTypeInsertTailComponent(SdifFrameTypeType *FrameType,
				 char *MatrixName,
				 char *NameC,
				 SdifPredefinedEnum Predefined);

extern SdifUInt4
SdifFrameTypeGetNumComponent(SdifFrameTypeType *FrameType, char *NameCD);

extern SdifComponentType*
SdifFrameTypeGetComponent(SdifFrameTypeType *FrameType, char *NameCD);

extern SdifComponentType*
SdifFrameTypeGetNthComponent(SdifFrameTypeType *FrameType, SdifUInt4 NumC);

extern int
SdifFWriteOneFrameType(SdifFrameTypeType *FrameType,
		       SdifPredefinedEnum Predefined,
		       FILE *fw);

extern int
SdifFWriteAllFrameType(SdifPredefinedEnum Predefined, FILE *fw);

extern int
SdifFReadFrameType(SdifPredefinedEnum Predefined, FILE *fr);

#endif /* _SdifFrameType_  */
