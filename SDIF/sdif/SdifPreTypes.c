/* SdifPreTypes.c
 *
 * Compiled Predefined Types. If the predefined types file isn't find or understood,
 * then this types are used. But in this case, some types can miss.
 *
 * author: Dominique Virolle 1997
 *
 *
 */


#include "SdifPreTypes.h"


#include "SdifHash.h"
#include "SdifGlobals.h"
#include "SdifFileStruct.h"
#include "SdifFile.h"



/*****************************************
******************************************
*          PREDEFINED MATRIX             *
******************************************
******************************************/


/* 1FQ0 matrix
 *  Mtrx  1FQ0    {Frequency, Mode, Hit}
 */
SdifColumnDefT   M_1FQ0_Frequency     = {"Frequency", 1};
SdifColumnDefT   M_1FQ0_Mode          = {"Mode", 2};
SdifColumnDefT   M_1FQ0_Hit           = {"Hit", 3};

SdifColumnDefNT  M_1FQ0_HitNode       = { NULL,             &M_1FQ0_Hit};
SdifColumnDefNT  M_1FQ0_ModeNode      = { &M_1FQ0_HitNode,  &M_1FQ0_Mode};
SdifColumnDefNT  M_1FQ0_FrequencyNode = { &M_1FQ0_ModeNode, &M_1FQ0_Frequency};

SdifMatrixTypeT  M_1FQ0 = { '1FQ0' ,
			    NULL,
			    &M_1FQ0_FrequencyNode,
			    &M_1FQ0_HitNode,
			    3, 3,
			    eNoModif};









/* 1FOF matrix
 *  Mtrx  1FOF  {Frequency, Amplitude, BandWidth, Tex, DebAtt, Atten, Phase}
 */
SdifColumnDefT   M_1FOF_Frequency     = {"Frequency", 1};
SdifColumnDefT   M_1FOF_Amplitude     = {"Amplitude", 2};
SdifColumnDefT   M_1FOF_BandWidth     = {"BandWidth", 3};
SdifColumnDefT   M_1FOF_Tex           = {"Tex",       4};
SdifColumnDefT   M_1FOF_DebAtt        = {"DebAtt",    5};
SdifColumnDefT   M_1FOF_Atten         = {"Atten",     6};
SdifColumnDefT   M_1FOF_Phase         = {"Phase",     7};

SdifColumnDefNT  M_1FOF_PhaseNode     = { NULL,                  &M_1FOF_Phase};
SdifColumnDefNT  M_1FOF_AttenNode     = { &M_1FOF_PhaseNode,     &M_1FOF_Atten};
SdifColumnDefNT  M_1FOF_DebAttNode    = { &M_1FOF_AttenNode,     &M_1FOF_DebAtt};
SdifColumnDefNT  M_1FOF_TexNode       = { &M_1FOF_DebAttNode,    &M_1FOF_Tex};
SdifColumnDefNT  M_1FOF_BandWidthNode = { &M_1FOF_TexNode,       &M_1FOF_BandWidth};
SdifColumnDefNT  M_1FOF_AmplitudeNode = { &M_1FOF_BandWidthNode, &M_1FOF_Amplitude};
SdifColumnDefNT  M_1FOF_FrequencyNode = { &M_1FOF_AmplitudeNode, &M_1FOF_Frequency};


SdifMatrixTypeT  M_1FOF = { '1FOF' ,
			    NULL,
			    &M_1FOF_FrequencyNode,
			    &M_1FOF_PhaseNode,
			    7, 7,
			    eNoModif};








/* 1CHA matrix
 *  Mtrx  1CHA	{Channel1, Channel2, Channel3, Channel4}
 */
SdifColumnDefT   M_1CHA_Channel1     = {"Channel1", 1};
SdifColumnDefT   M_1CHA_Channel2     = {"Channel2", 2};
SdifColumnDefT   M_1CHA_Channel3     = {"Channel3", 3};
SdifColumnDefT   M_1CHA_Channel4     = {"Channel4", 4};

SdifColumnDefNT  M_1CHA_Channel4Node = { NULL,                 &M_1CHA_Channel4};
SdifColumnDefNT  M_1CHA_Channel3Node = { &M_1CHA_Channel4Node, &M_1CHA_Channel3};
SdifColumnDefNT  M_1CHA_Channel2Node = { &M_1CHA_Channel3Node, &M_1CHA_Channel2};
SdifColumnDefNT  M_1CHA_Channel1Node = { &M_1CHA_Channel2Node, &M_1CHA_Channel1};

SdifMatrixTypeT  M_1CHA = { '1CHA',
			    NULL,
			    &M_1CHA_Channel1Node,
			    &M_1CHA_Channel4Node,
			    4, 4,
			    eNoModif};





/* 1RES matrix
 *  Mtrx  1RES	{Frequency, Amplitude, BandWidth, Saliance, Correction}
 */

SdifColumnDefT   M_1RES_Frequency      = {"Frequency",  1};
SdifColumnDefT   M_1RES_Amplitude      = {"Amplitude",  2};
SdifColumnDefT   M_1RES_BandWidth      = {"BandWidth",  3};
SdifColumnDefT   M_1RES_Saliance       = {"Saliance",   4};
SdifColumnDefT   M_1RES_Correction     = {"Correction", 5};

SdifColumnDefNT  M_1RES_CorrectionNode = { NULL,                   &M_1RES_Correction};
SdifColumnDefNT  M_1RES_SalianceNode   = { &M_1RES_CorrectionNode, &M_1RES_Saliance};
SdifColumnDefNT  M_1RES_BandWidthNode  = { &M_1RES_SalianceNode,   &M_1RES_BandWidth};
SdifColumnDefNT  M_1RES_AmplitudeNode  = { &M_1RES_BandWidthNode,  &M_1RES_Amplitude};
SdifColumnDefNT  M_1RES_FrequencyNode  = { &M_1RES_AmplitudeNode,  &M_1RES_Frequency};

SdifMatrixTypeT  M_1RES = { '1RES',
			    NULL,
			    &M_1RES_FrequencyNode,
			    &M_1RES_CorrectionNode,
			    5, 5, eNoModif};







/* 1DIS matrix
 *  Mtrx  1DIS	{Distribution, Amplitude}
 */

SdifColumnDefT   M_1DIS_Distribution      = {"Distribution",  1};
SdifColumnDefT   M_1DIS_Amplitude         = {"Amplitude",     2};

SdifColumnDefNT  M_1DIS_AmplitudeNode     = { NULL,                  &M_1DIS_Amplitude};
SdifColumnDefNT  M_1DIS_DistributionNode  = { &M_1DIS_AmplitudeNode, &M_1DIS_Distribution};

SdifMatrixTypeT  M_1DIS = { '1DIS' ,
			    NULL,
			    &M_1DIS_DistributionNode,
			    &M_1DIS_AmplitudeNode,
			    2, 2, eNoModif};




/*****************************************
******************************************
*          PREDEFINED FRAMES             *
******************************************
******************************************/


/* 1FOB frame
 *  Fram	1FOB
 *	{
 *	  1FQ0  PitchModeHit;
 *	  1FOF	Formants;
 *	  1CHA  FormantsChannels;
 *	}
 */

SdifComponentT   F_1FOB_PitchModeHit         = { '1FQ0', "PitchModeHit", 1};
SdifComponentT   F_1FOB_Formants             = { '1FOF', "Formants",     2};
SdifComponentT   F_1FOB_FormantsChannels     = { '1CHA', "Formants",     3};

SdifComponentNT  F_1FOB_FormantsChannelsNode = { NULL,                         &F_1FOB_FormantsChannels};
SdifComponentNT  F_1FOB_FormantsNode         = { &F_1FOB_FormantsChannelsNode, &F_1FOB_Formants};
SdifComponentNT  F_1FOB_PitchModeHitNode     = { &F_1FOB_FormantsNode,         &F_1FOB_PitchModeHit};

SdifFrameTypeT   F_1FOB = { '1FOB',
			    NULL,
			    &F_1FOB_PitchModeHitNode,
			    &F_1FOB_FormantsChannelsNode,
			    3, 3,
			    eNoModif};






/* 1REB frame
 *  Fram	1REB
 *	{
 *	  1RES  Filters;
 *	  1CHA  FiltersChannels;
 *	}
 */

SdifComponentT   F_1REB_Filters             = { '1RES', "Filters", 1};
SdifComponentT   F_1REB_FiltersChannels     = { '1CHA', "FiltersChannels", 2};

SdifComponentNT  F_1REB_FiltersChannelsNode = { NULL,                &F_1REB_FiltersChannels};
SdifComponentNT  F_1REB_FiltersNode         = { &F_1REB_FiltersChannelsNode, &F_1REB_Filters};

SdifFrameTypeT   F_1REB = { '1REB',
			    NULL,
			    &F_1REB_FiltersNode,
			    &F_1REB_FiltersChannelsNode,
			    2, 2,
			    eNoModif};




/* 1NOI frame
 *  Fram  1NOI
 *	{
 *	  1DIS  NoiseInfo;
 *	}
 */

SdifComponentT   F_1NOI_NoiseInfo     = { '1DIS', "NoiseInfo", 1};
SdifComponentNT  F_1NOI_NoiseInfoNode = { NULL, &F_1NOI_NoiseInfo};

SdifFrameTypeT   F_1NOI = { '1NOI',
			    NULL,
			    &F_1NOI_NoiseInfoNode,
			    &F_1NOI_NoiseInfoNode,
			    1, 1,
			    eNoModif};

