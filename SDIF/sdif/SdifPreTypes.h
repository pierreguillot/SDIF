/* SdifPreTypes.h
 *
 * Compiled Predefined Types. If the predefined types file isn't find or understood,
 * then this types are used. But in this case, some types can miss.
 *
 * author: Dominique Virolle 1997
 *
 *
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

extern SdifComponentT   F_1FOB_PitchModeHit;
extern SdifComponentT   F_1FOB_Formants;
extern SdifComponentT   F_1FOB_FormantsChannels;
extern SdifComponentNT  F_1FOB_FormantsChannelsNode;
extern SdifComponentNT  F_1FOB_FormantsNode;
extern SdifComponentNT  F_1FOB_PitchModeHitNode;
extern SdifFrameTypeT   F_1FOB;

extern SdifComponentT   F_1REB_Filters;
extern SdifComponentT   F_1REB_FiltersChannels;
extern SdifComponentNT  F_1REB_FiltersChannelsNode;
extern SdifComponentNT  F_1REB_FiltersNode;
extern SdifFrameTypeT   F_1REB;

extern SdifComponentT   F_1NOI_NoiseInfo;
extern SdifComponentNT  F_1NOI_NoiseInfoNode;
extern SdifFrameTypeT   F_1NOI;

#endif /* _SdifPreTypes_ */
