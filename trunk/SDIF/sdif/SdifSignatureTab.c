/* $Id: SdifSignatureTab.c,v 3.1 2000-05-04 14:59:32 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifSignatureTab.c		4. May 2000		Diemo Schwarz
 * 
 * Handling of a Table of Signatures
 *
 * $Log: not supported by cvs2svn $
 */

#include "SdifSignatureTab.h"


SdifSignatureTabT*
SdifCreateSignatureTab (const SdifUInt4 NbSignMax)
{
  SdifSignatureTabT* NewSignTab = NULL;
  SdifUInt4 iSign;

  NewSignTab = SdifMalloc(SdifSignatureTabT);
  if (NewSignTab)
    {
      NewSignTab->Tab = SdifCalloc(SdifSignature, NbSignMax);
      if (NewSignTab->Tab)
        {
          NewSignTab->NbSignMax = NbSignMax;
          for (iSign=0; iSign<NewSignTab->NbSignMax; iSign++)
            NewSignTab->Tab[iSign] = 0;
          NewSignTab->NbSign    = 0;
        }
      else
        {
          _SdifError(eAllocFail, "NewSignTab->Tab");
          return NULL;
        }
    }
  else
    {
      _SdifError(eAllocFail, "NewSignTab");
      return NULL;
    }

  return NewSignTab;
}



void
SdifKillSignatureTab (SdifSignatureTabT* SignTab)
{
  if (SignTab)
    {
      if (SignTab->Tab)
      {
	  SdifFree(SignTab->Tab);
      }
      SdifFree(SignTab);
    }
  else
    {
      _SdifError(eAllocFail, "NewSignTab");
    }
}



SdifSignatureTabT*
SdifReAllocSignatureTab (SdifSignatureTabT* SignTab,
			 const  SdifUInt4 NewNbSignMax)
{
    if (SignTab->NbSignMax  < NewNbSignMax)
    {
	SignTab->Tab = SdifRealloc(SignTab->Tab, SdifSignature, NewNbSignMax);
	if (SignTab->Tab )
	{
	    SignTab->NbSignMax = NewNbSignMax;
	}
	else
	{
	    _SdifError(eAllocFail, "SignTab->Tab RE-allocation");
	    return NULL;
	}
    }

    return SignTab;
}



SdifSignatureTabT*
SdifReInitSignatureTab (SdifSignatureTabT* SignTab, 
			const SdifUInt4 NewNbSignMax)
{
    SdifUInt4 iSign;

    SdifReAllocSignatureTab (SignTab, NewNbSignMax);

    for (iSign=0; iSign<NewNbSignMax; iSign++)
        SignTab->Tab[iSign] = 0;

    SignTab->NbSign  = 0;

    return SignTab;
}




SdifSignature
SdifIsInSignatureTab (const SdifSignatureTabT* SignTab, 
		      const SdifSignature Sign)
{
  SdifUInt4 iSign;

  for (iSign=0; iSign<SignTab->NbSign; iSign++)
    if (SignTab->Tab[iSign] == Sign)
      return Sign;

  return 0;
}



int
SdifFindInSignatureTab (const SdifSignatureTabT* SignTab, 
			const SdifSignature Sign)
{
  SdifUInt4 iSign;

  for (iSign=0; iSign<SignTab->NbSign; iSign++)
    if (SignTab->Tab[iSign] == Sign)
      return iSign;

  return -1;
}


SdifSignatureTabT*
SdifPutInSignatureTab (SdifSignatureTabT* SignTab, const SdifSignature Sign)
{
  SignTab->Tab[SignTab->NbSign] = Sign;
  SignTab->NbSign++;
  return SignTab;
}


SdifSignatureTabT*
SdifAddToSignatureTab (SdifSignatureTabT* SignTab, const SdifSignature Sign)
{
    if (SignTab->NbSign >= SignTab->NbSignMax)
    {
	SdifReAllocSignatureTab (SignTab, SignTab->NbSign + 
					  _SdifSignatureTabGranule);
    }
    return SdifPutInSignatureTab (SignTab, Sign);
}


SdifSignature
SdifGetFromSignatureTab (const SdifSignatureTabT* SignTab, const int index)
{
    if (index >= 0  &&  index < SignTab->NbSign)
	return (SignTab->Tab[index]);
    else
	return eEmptySignature;
}
