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


/**
 * @file   sdifexception.h
 * @author Axel Roebel
 * @date   Wed May 21 22:03:04 2003
 * 
 * @brief   SDIFException - SDIF Error handling via exceptions
 * 
 * 
 * 
 * $Id: sdifexception.h,v 1.3 2003-05-21 20:40:30 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $ 
 * 
 */


#ifndef SDIFEXCEPTION_H_
#define SDIFEXCEPTION_H_ 1

#include <iostream>
#include <string>
#include <sdif.h>

namespace Easdif {

  /*************************************************************************/
  /* Exception Handling  */

  /**
   * \defgroup exception SDIFException - SDIF Error handling via exceptions
   *
   *
   */

  /** 
   * \brief SDIFException - SDIF Error handling via exceptions
   * 
   *
   * In Easdif all SDIF errors are mapped to corresponding exceptions
   * the base class is SDIFException. All other exception 
   * classes are derived from here.
   *
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


    bool initException(SdifErrorLevelET level,
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

    /** 
     * \ingroup exception 
     *  The member function ErrorMessage prints the SDIF error message
     *  via  cerr.
     */
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

    

    /** 
     * \ingroup exception 
     *
     * \brief get sdiffile pointer for file that causec exception
     * 
     * @return  sdiffile pointer
     */
    const SdifFileT* sdifFile() const { return mSdifFile;}


    /** 
     * \ingroup exception 
     * \brief get  error message
     * 
     * @return returns mMessage string
     */

    const std::string& getmessage() const { return mMessage;}


    /** 
     * \ingroup exception 
     * \brief get  source file name that emitted error
     * 
     * @return returns Source File name as a string
     */
    const std::string& getsourcename() const { return mSourceFileName;}

     /** 
     * \ingroup exception 
     * \brief get SDIF error tag 
     * 
     * @return error tag
     */
     SdifErrorTagET getetag() const { 
       if (mError)return mError->Tag;
       return eUnknow;
     }
   
protected:
    bool mIsInit;
    int mSourceFileLine;
    std::string mSourceFileName;
    std::string mMessage;
    SdifFileT* mSdifFile;
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
