/*--------------------------------------------------------------------------*/
/*	UniversalEnvVar.h							Sept.96 - A.Lefèvre IRCAM	*/
/*																			*/
/*																			*/
/*--------------------------------------------------------------------------*/

#ifndef _UNIVERSALENVVAR_
#define _UNIVERSALENVVAR_

/*--------------------------------------------------------------------------*/
/*	File Extensions															*/
/*--------------------------------------------------------------------------*/

#if HOST_OS_MAC | macintosh

#define extens_aFundamental		"F0"
#define extens_bFundamental		"f0"
#define extens_FloatSnd			"floatSnd"
#define extens_AiffSnd			"aiff"

#else

#define extens_aFundamental		"F0"
#define extens_bFundamental		"f0"
#define extens_FloatSnd			"sf"
#define extens_AiffSnd			"aiff"

#endif

/*--------------------------------------------------------------------------*/
/*	Environment Variables													*/
/*--------------------------------------------------------------------------*/

#if HOST_OS_MAC | macintosh

#define varenv_Fundamental	"F0DIR"
#define varenv_Sound		"SFDIR"

#else

#define varenv_Fundamental	"DATADIR"
#define varenv_Sound		"SFDIR"

#endif

/*--------------------------------------------------------------------------*/

#endif



