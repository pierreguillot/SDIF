/*
 * This file is part of the IRCAM EASDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 2002-2003 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * The EASDIF library provides an Easy Api for IRCAMs SDIF library. 
 *
 */


#include <iostream>
#include "easdif/sdifexception.h"

namespace Easdif {
  extern "C" {
    
    void
    ExceptionThrower(int errnum, 
		     SdifErrorLevelET errlev, 
		     char* msg, 
		     SdifFileT* file, 
		     SdifErrorT* error, 
		     char* sourcefilename, 
		     int sourcefileline)
    {   
      switch(errnum)
	{
	  /* the SdifError.c errors :*/
	case eFreeNull:
	  {
	    SDIFFreeNull exc;
	    /* initialise the SDFIFreeNull object (substitute the constructor,
	     * this is more efficient for the SdifFileT and SdifErrorT 
	     * pointers)*/
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;	
	  }
	  break;
	  
	case eAllocFail :
	  {
	    SDIFAllocFail exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	case eArrayPosition :
	{
	    SDIFArrayPosition exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	case  eEof :	
	{
	    SDIFEof exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
 	break;

	case  eNotInDataTypeUnion :	
	{
	    SDIFNotInDataTypeUnion exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;      

	case  eTokenLength :	
	{
	    SDIFTokenLength exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	/* the SdifErrMess.c errors */
	case eReDefined :
	{
	    SDIFReDefined exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	case eUnDefined :
	{
	    SDIFUnDefined exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	case eBadHeader :
	{
	    SDIFBadHeader exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	case eBadMode :
	{
	    SDIFBadMode exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	case eBadStdFile :
	{
	    SDIFBadStdFile exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	case eReadWriteOnSameFile :
	{
	    SDIFReadWriteOnSameFile exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	case eBadFormatVersion :
	{
	    SDIFBadFormatVersion exc;
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
	    throw exc;
	}
	break;

	default :	/*a revoir*/
	    std::cerr <<"default";
/*
  exc.initException(errlev, msg, file, error, 
  sourcefilename, sourcefileline);
  throw exc;
*/
	    SDIFException exc;
///
	    exc.initException(errlev, msg, file, error, 
			      sourcefilename, sourcefileline);
///
	    throw exc;
	    break;
	}
    }

}
} // end of namespace Easdif
