/* $Id: SdifPreTypes.h,v 2.0 1998-11-29 11:42:01 virolle Exp $
 *
 * SdifPreTypes.h
 *
 * Compiled Predefined Types. If the predefined types file isn't find or understood,
 * then this types are used. But in this case, some types can miss.
 *
 * author: Dominique Virolle 1997
 *
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.2  1998/11/10  15:31:53  schwarz
 * Removed all 'extern' keywords for prototypes, since this is redundant
 * (function prototypes are automatically linked extern), and it
 * prohibits cocoon from generating an entry in the HTML documentation
 * for this function.
 *
 */


#ifndef _SdifPreTypes_
#define _SdifPreTypes_

#include "SdifMatrixType.h"
#include "SdifFrameType.h"
#include "SdifGlobals.h"
#include "SdifFileStruct.h"


extern SdifColumnDefT   M_1FQ0_Frequency;
extern SdifColumnDefT   M_1FQ0_Mode;
extern SdifColumnDefT   M_1FQ0_Hit;
extern SdifColumnDefNT  M_1FQ0_HitNode;
extern SdifColumnDefNT  M_1FQ0_ModeNode;
extern SdifColumnDefNT  M_1FQ0_FrequencyNode;
extern SdifMatrixTypeT  M_1FQ0;

extern SdifColumnDefT   M_1FOF_Frequency;
extern SdifColumnDefT   M_1FOF_Amplitude;
extern SdifColumnDefT   M_1FOF_BandWidth;
extern SdifColumnDefT   M_1FOF_Tex;
extern SdifColumnDefT   M_1FOF_DebAtt;
extern SdifColumnDefT   M_1FOF_AtteNode;
extern SdifColumnDefT   M_1FOF_Phase;
extern SdifColumnDefNT  M_1FOF_PhaseNode;
extern SdifColumnDefNT  M_1FOF_AttenNode;
extern SdifColumnDefNT  M_1FOF_DebAttNNode;
extern SdifColumnDefNT  M_1FOF_TexNode;
extern SdifColumnDefNT  M_1FOF_BandWidthNode;
extern SdifColumnDefNT  M_1FOF_AmplitudeNode;
extern SdifColumnDefNT  M_1FOF_FrequencyNode;
extern SdifMatrixTypeT  M_1FOF;

extern SdifColumnDefT   M_1CHA_Channel;
extern SdifColumnDefT   M_1CHA_Channel2;
extern SdifColumnDefT   M_1CHA_Channel3;
extern SdifColumnDefT   M_1CHA_Channel4;
extern SdifColumnDefNT  M_1CHA_Channel4Node;
extern SdifColumnDefNT  M_1CHA_Channel3Node;
extern SdifColumnDefNT  M_1CHA_Channel2Node;
extern SdifColumnDefNT  M_1CHA_ChannelNode;
extern SdifMatrixTypeT  M_1CHA;

extern SdifColumnDefT   M_1RES_Frequency;
extern SdifColumnDefT   M_1RES_Amplitude;
extern SdifColumnDefT   M_1RES_BandWidth;
extern SdifColumnDefT   M_1RES_Saliance;
extern SdifColumnDefT   M_1RES_CorrectioNode;
extern SdifColumnDefNT  M_1RES_CorrectionNode;
extern SdifColumnDefNT  M_1RES_SalianceNode;
extern SdifColumnDefNT  M_1RES_BandWidthNode;
extern SdifColumnDefNT  M_1RES_AmplitudeNode;
extern SdifColumnDefNT  M_1RES_FrequencyNode;
extern SdifMatrixTypeT  M_1RES;

extern SdifColumnDefT   M_1DIS_DistributioNode;
extern SdifColumnDefT   M_1DIS_Amplitude;
extern SdifColumnDefNT  M_1DIS_AmplitudeNode;
extern SdifColumnDefNT  M_1DIS_DistributionNode;
extern SdifMatrixTypeT  M_1DIS;

SdifFrameTypeT* CreateF_1FOB(void);
SdifFrameTypeT* CreateF_1REB(void);
SdifFrameTypeT* CreateF_1NOI(void);
void SdifCreatePredefinedTypes(SdifHashTableT *MatrixTypesHT,
                                      SdifHashTableT *FrameTypesHT);

#endif /* _SdifPreTypes_ */
