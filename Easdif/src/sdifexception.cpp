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
