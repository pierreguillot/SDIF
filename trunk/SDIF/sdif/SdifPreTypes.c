/* $Id: SdifPreTypes.c,v 2.2 1999-01-23 13:57:43 virolle Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * Compiled Predefined Types. If the predefined types file isn't find or understood,
 * then this types are used. But in this case, some types can miss.
 *
 * author: Dominique Virolle 1997
 *
 *
 *
 * $Log: not supported by cvs2svn $
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
SdifMatrixTypeT*
CreateM_1FQ0(void)
{
  SdifMatrixTypeT*  M_1FQ0;


  M_1FQ0 = SdifCreateMatrixType('1FQ0', NULL);
  SdifMatrixTypeInsertTailColumnDef(M_1FQ0, M_1FQ0_Frequency);
  SdifMatrixTypeInsertTailColumnDef(M_1FQ0, M_1FQ0_Mode);
  SdifMatrixTypeInsertTailColumnDef(M_1FQ0, M_1FQ0_Hit);
  M_1FQ0->ModifMode = eNoModif;
  return M_1FQ0;
}






/* 1FOF matrix
 *  Mtrx  1FOF  {Frequency, Amplitude, BandWidth, Tex, DebAtt, Atten, Phase}
 */
SdifMatrixTypeT*
CreateM_1FOF(void)
{
  SdifMatrixTypeT*  M_1FOF;


  M_1FOF = SdifCreateMatrixType('1FOF', NULL);
  SdifMatrixTypeInsertTailColumnDef(M_1FOF, M_1FOF_Frequency);
  SdifMatrixTypeInsertTailColumnDef(M_1FOF, M_1FOF_Amplitude);
  SdifMatrixTypeInsertTailColumnDef(M_1FOF, M_1FOF_BandWidth);
  SdifMatrixTypeInsertTailColumnDef(M_1FOF, M_1FOF_Tex);
  SdifMatrixTypeInsertTailColumnDef(M_1FOF, M_1FOF_DebAtt);
  SdifMatrixTypeInsertTailColumnDef(M_1FOF, M_1FOF_Atten);
  SdifMatrixTypeInsertTailColumnDef(M_1FOF, M_1FOF_Phase);
  M_1FOF->ModifMode = eNoModif;
  return M_1FOF;
}





/* 1CHA matrix
 *  Mtrx  1CHA	{Channel1, Channel2, Channel3, Channel4}
 */
SdifMatrixTypeT*
CreateM_1CHA(void)
{
  SdifMatrixTypeT*  M_1CHA;


  M_1CHA = SdifCreateMatrixType('1CHA', NULL);
  SdifMatrixTypeInsertTailColumnDef(M_1CHA, M_1CHA_Channel1);
  SdifMatrixTypeInsertTailColumnDef(M_1CHA, M_1CHA_Channel2);
  SdifMatrixTypeInsertTailColumnDef(M_1CHA, M_1CHA_Channel3);
  SdifMatrixTypeInsertTailColumnDef(M_1CHA, M_1CHA_Channel4);
  M_1CHA->ModifMode = eNoModif;
  return M_1CHA;
}



/* 1RES matrix
 *  Mtrx  1RES	{Frequency, Amplitude, BandWidth, Saliance, Correction}
 */
SdifMatrixTypeT*
CreateM_1RES(void)
{
  SdifMatrixTypeT*  M_1RES;


  M_1RES = SdifCreateMatrixType('1RES', NULL);
  SdifMatrixTypeInsertTailColumnDef(M_1RES, M_1RES_Frequency);
  SdifMatrixTypeInsertTailColumnDef(M_1RES, M_1RES_Amplitude);
  SdifMatrixTypeInsertTailColumnDef(M_1RES, M_1RES_BandWidth);
  SdifMatrixTypeInsertTailColumnDef(M_1RES, M_1RES_Saliance);
  SdifMatrixTypeInsertTailColumnDef(M_1RES, M_1RES_Correction);
  M_1RES->ModifMode = eNoModif;
  return M_1RES;
}



/* 1DIS matrix
 *  Mtrx  1DIS	{Distribution, Amplitude}
 */
SdifMatrixTypeT*
CreateM_1DIS(void)
{
  SdifMatrixTypeT*  M_1DIS;


  M_1DIS = SdifCreateMatrixType('1DIS', NULL);
  SdifMatrixTypeInsertTailColumnDef(M_1DIS, M_1DIS_Distribution);
  SdifMatrixTypeInsertTailColumnDef(M_1DIS, M_1DIS_Amplitude);
  M_1DIS->ModifMode = eNoModif;
  return M_1DIS;
}




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

SdifFrameTypeT*
CreateF_1FOB(void)
{
  SdifFrameTypeT*  F_1FOB;

  F_1FOB = SdifCreateFrameType('1FOB', NULL);
  SdifFrameTypePutComponent(F_1FOB, '1FQ0', "PitchModeHit");
  SdifFrameTypePutComponent(F_1FOB, '1FOF', "Formants");
  SdifFrameTypePutComponent(F_1FOB, '1CHA', "FormantsChannels");
  F_1FOB->ModifMode = eNoModif;
  return F_1FOB;
}


/* 1REB frame
 *  Fram	1REB
 *	{
 *	  1RES  Filters;
 *	  1CHA  FiltersChannels;
 *	}
 */

SdifFrameTypeT*
CreateF_1REB(void)
{
  SdifFrameTypeT*   F_1REB;

  F_1REB = SdifCreateFrameType('1REB', NULL);
  SdifFrameTypePutComponent(F_1REB, '1RES', "Filters");
  SdifFrameTypePutComponent(F_1REB, '1CHA', "FiltersChannels");
  F_1REB->ModifMode = eNoModif;
  return F_1REB;
}



/* 1NOI frame
 *  Fram  1NOI
 *	{
 *	  1DIS  NoiseInfo;
 *	}
 */

SdifFrameTypeT*
CreateF_1NOI(void)
{
  SdifFrameTypeT*   F_1NOI;

  F_1NOI = SdifCreateFrameType('1NOI', NULL);
  SdifFrameTypePutComponent(F_1NOI, '1DIS', "NoiseInfo");
  F_1NOI->ModifMode = eNoModif;
  return F_1NOI;
}


void
SdifCreatePredefinedTypes(SdifHashTableT *MatrixTypesHT, SdifHashTableT *FrameTypesHT)
{
  SdifPutMatrixType(MatrixTypesHT,   CreateM_1FQ0());
  SdifPutMatrixType(MatrixTypesHT,   CreateM_1FOF());
  SdifPutMatrixType(MatrixTypesHT,   CreateM_1CHA());
  SdifPutMatrixType(MatrixTypesHT,   CreateM_1RES());
  SdifPutMatrixType(MatrixTypesHT,   CreateM_1DIS());

  SdifPutFrameType(FrameTypesHT,   CreateF_1FOB() );
  SdifPutFrameType(FrameTypesHT,   CreateF_1REB() );
  SdifPutFrameType(FrameTypesHT,   CreateF_1NOI() );
}

