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
#include "easdif/easdif_config.h"
#include "easdif/sdifexception.h"

namespace Easdif {
  extern "C" {
    
    void
    ExceptionThrower(SdifErrorTagET errnum, 
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
	  throw SDIFFreeNull(errlev, msg, file, errnum, 
			     sourcefilename, sourcefileline);
	  break;
	  
	case eAllocFail :
	  throw SDIFAllocFail(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;
	  
	case eArrayPosition :
	  throw SDIFArrayPosition(errlev, msg, file, errnum, 
				  sourcefilename, sourcefileline);
	  break;
	  
	case  eEof :	
	  throw SDIFEof(errlev, msg, file, errnum, 
			sourcefilename, sourcefileline);
	  break;

	case  eNotInDataTypeUnion :	
	  throw SDIFNotInDataTypeUnion(errlev, msg, file, errnum, 
				       sourcefilename, sourcefileline);
	  break;      

	case  eTokenLength :	
	  throw SDIFTokenLength(errlev, msg, file, errnum, 
				sourcefilename, sourcefileline);
	  break;

	/* the SdifErrMess.c errors */
	case eReDefined :
	  throw SDIFReDefined(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;

	case eUnDefined :
	  throw SDIFUnDefined(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;
	  
	case eBadHeader :
	  throw SDIFBadHeader(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;

	case eBadMode :
	  throw SDIFBadMode(errlev, msg, file, errnum, 
			    sourcefilename, sourcefileline);
	  break;

	case eBadStdFile :
	  throw SDIFBadStdFile(errlev, msg, file, errnum, 
			       sourcefilename, sourcefileline);
	  break;

	case eReadWriteOnSameFile :
	  throw SDIFReadWriteOnSameFile(errlev, msg, file, errnum, 
					sourcefilename, sourcefileline);
	  break;

	case eBadFormatVersion :
	  throw SDIFBadFormatVersion(errlev, msg, file, errnum, 
				     sourcefilename, sourcefileline);
	  break;

	default :	/*a revoir*/
	  throw SDIFException(errlev, msg, file, errnum, 
			      sourcefilename, sourcefileline);
	  break;
	}
    }

}
} // end of namespace Easdif
