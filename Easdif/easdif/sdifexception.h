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


#ifndef SDIFEXCEPTION_H_
#define SDIFEXCEPTION_H_ 1

#include <iostream>
#include <string>
#include <sdif.h>

namespace Easdif {

  /*
    in this class, we are interested just in errors (not warnings)
  */
  class SDIFException
  {
  public:
    SDIFException(): mIsInit(false) {};
    
    SDIFException(SdifErrorLevelET level,
		  char* message,
		  SdifFileT* sdifFile,	
		  SdifErrorT* error,
		  char* sourceFileName,
		  int sourceFileLine) {
      initException(level,message,sdifFile,error,
		    sourceFileName,sourceFileLine);
    };

    ~SDIFException() 
	{
	};

    inline bool initException(SdifErrorLevelET level,
			      char* message,
			      SdifFileT* sdifFile,	
			      SdifErrorT* error,
			      char* sourceFileName,
			      int sourceFileLine)
	{
	    if (!mIsInit)
	    {
		mSourceFileLine = sourceFileLine;
		if(sourceFileName)
		  mSourceFileName = std::string(sourceFileName);
		else
		  mSourceFileName = std::string("");

		if(message)
		  mMessage = std::string(message);
		else
		  mMessage = std::string("");

		mLevel = level;
		mSdifFile = sdifFile;
		mError = error;
		mIsInit = true;
	    }
	    return mIsInit;
	}

    void ErrorMessage()
	{

	  if (mMessage != "")
	    std::cerr << mMessage << std::endl;

	    
	  if (mSourceFileName != "")
	    std::cerr << "Source file : "
		      << mSourceFileName 
		      << "\nat line : "
		      << mSourceFileLine
		      << std::endl;
/*		    
		    << "\nError Level : "	
		    << mLevel 1 = fatal, 2 = error, 3 = warning 
		    << "\nError message : "  
		    << mMessage 
		    << "\nError : "
		    << mError
		    << std::endl;
*/
	}

    SdifFileT* sdifFile() { return mSdifFile;}

protected:
    bool mIsInit;
    int mSourceFileLine;
    std::string mSourceFileName;
    std::string mMessage;
    SdifFileT* mSdifFile;
    SdifErrorT* error;

    SdifErrorT* mError;

    SdifErrorLevelET mLevel;
/*
    union number //error type
    {
	SdifErrorEnum num;
	SdifErrorTagET tag;
    }
*/

/*
    SdifErrorEnum errorenum()
	{}
    SdifErrorTagET errortag()
	{}
*/

};


/****************** FILE ERRORS  *************************/
class SDIFFileError : public SDIFException
{
};

/* Opening Errors */
class SDIFOpeningError : public SDIFFileError
{
};

class SDIFReadWriteOnSameFile : public SDIFOpeningError
{
};

class SDIFBadStdFile : public SDIFOpeningError
{
};

class SDIFBadMode  : public SDIFOpeningError
{
};

/* Header Errors */
class SDIFHeaderError : public SDIFFileError
{
};

class SDIFFrameHeaderSizeError : public SDIFFileError
{
};

class SDIFBadFormatVersion : public SDIFHeaderError
{
};

class SDIFBadHeader : public SDIFHeaderError
{
};

/* ASCII Chunk Errors */
class SDIFAsciiChunkError : public SDIFFileError
{
};

class SDIFReDefined : public SDIFAsciiChunkError
{
};

class SDIFUnDefined  : public SDIFAsciiChunkError
{
/*
  public:
  SDIFUnDefined(){ mError = eUnDefined};
*/
};

/* Reading */
class SDIFReading : public SDIFFileError
{
};

class SDIFEof : public SDIFReading
{
/*
  public:
  SDIFEof(){ mError = eEof};
*/
};

/****************** DESCRIPTION TYPE CHECKING ************/
class SDIFDescriptionTypeError : public SDIFException{};
/* Reading */
/*
class SDIFReading : public SDIFDescriptionTypeError
{};
*/
class SDIFArrayPosition  : public SDIFReading
{
/*
  public:
  SDIFArrayPosition(){ mError = eArrayPosition };
*/
};

/****************** MATRIX DATA ELEMENT TYPE CHECKING ****/
class SDIFMatrixDataError : public SDIFException{};

class SDIFNotInDataTypeUnion  : public SDIFMatrixDataError
{
/*
  public:
  SDIFNotInDataTypeUnion(){ mError = eNotInDataTypeUnion };
*/
};

/****************** MEMORY MANAGEMENT ********************/
class SDIFMemoryError : public SDIFException{};

class SDIFFreeNull : public SDIFMemoryError
{
/*
  public:
  SDIFFreeNull(){ mError = eFreeNull };
*/
};

class SDIFAllocFail : public SDIFMemoryError
{
/*
  public:
  SDIFAllocFail(){ mError = eAllocFail };
*/
};

/****************** SDIF-Text Reading ********************/
class SDIFTextReadingError : public SDIFException{};

class SDIFTokenLength : public SDIFTextReadingError
{
/*
  public:
  SDIFTokenLength(){ mError = eTokenLength };
*/
};

extern "C"
{
void
ExceptionThrower(int errnum, 
		 SdifErrorLevelET errlev, 
		 char* msg, 
		 SdifFileT* file, 
		 SdifErrorT* error, 
		 char* sourcefilename, 
		 int sourcefileline);
};

} // end of namespace Easdif
#endif
