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
 * $Id: sdifexception.h,v 1.6 2003-12-05 13:53:14 ellis Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.5  2003/11/25 10:57:05  roebel
 * Added missing namespace qualifier for exceptions.
 * Added missing exception specification to exception deconstructor.
 *
 * Revision 1.4  2003/11/18 18:18:40  roebel
 * Reorganized exception handling and interface such that
 * sdifexcpetions are now derived from standard exception and
 * sdif error tags and error numbers are treated equivalently.
 * Improved documentation.
 *
 * Revision 1.3  2003/05/21 20:40:30  roebel
 * Added documentation. Removed unused member "error".
 * Added member functions to access message and errortag.
 * 
 * 
 */


#ifndef SDIFEXCEPTION_H_
#define SDIFEXCEPTION_H_ 1

#include <iostream>
#include <string>
#include <sdif.h>
#include <stdexcept>

namespace Easdif {

  /*************************************************************************/
  /* Exception Handling  */

  /**
   * \defgroup exception SDIFException - SDIF Error handling via exceptions
   *
   *
   */

  /** 
   * \ingroup exception 
   * \brief SDIFException - Basic exception from which all other Easdif exceptions derive 
   * 
   *
   * In Easdif all SDIF errors are mapped to corresponding exceptions
   * the base class is SDIFException. All other exception 
   * classes are derived from here.
   *
   */
  class SDIFException : public std::exception
  {
  public:    

    /** 
     * \ingroup exception 
     *  \brief Initialize exception
     *
     *  
     */
    SDIFException(SdifErrorLevelET level,
		  const char* message,
		  SdifFileT* sdifFile,	
		  int error, // either SdifErrorE or SdifErrorTagE
		  const char* sourceFileName,
		  int sourceFileLine) {

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
    };

    ~SDIFException() throw() {	};

    /** 
     * \ingroup exception 
     *  \brief return message
     *
     *  The member function what returns the error message
     */
    const char *what() {      
      return mMessage.c_str();
    }


    /** 
     * \ingroup exception 
     *  \brief pretty print error message
     *
     *  The member function ErrorMessage pretty-prints the SDIF error message
     *  via  cerr.
     */
    void ErrorMessage()
	{

	  std::cerr << "Exception: " << what() << std::endl;
	    
	  if (mSourceFileName != "")
	    std::cerr << "Source file : "
		      << mSourceFileName 
		      << "\nat line : "
		      << mSourceFileLine
		      << std::endl;
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
     * \brief get SDIF error tag / SdifErrorNumber
     * 
     * @return error number
     */
     int getenum() const { 
       return mError;
     }
   
protected:
    int mSourceFileLine;
    std::string mSourceFileName;
    std::string mMessage;
    SdifFileT* mSdifFile;
    int  mError;

    SdifErrorLevelET mLevel;

};


#define constructor(class1,class2) class1(SdifErrorLevelET level,  const char* message, \
					 SdifFileT* sdifFile,   int error, \
					 const char* sourceFileName, int sourceFileLine) \
  : class2(level,message,sdifFile,error,sourceFileName,sourceFileLine){}

/****************** FILE ERRORS  *************************/

/// \ingroup exception 
/// \brief SDIFFileError  error during file operations
class SDIFFileError : public SDIFException
{
public:
  constructor(SDIFFileError,SDIFException)
};

/// \ingroup exception 
/// \brief SDIFOpeningError error when opening a file 
class SDIFOpeningError : public SDIFFileError
{
public:
  constructor(SDIFOpeningError,SDIFFileError)
};

/// \ingroup exception 
/// \brief ReadWriteOnSameFile
class SDIFReadWriteOnSameFile : public SDIFOpeningError
{
public:
  constructor(SDIFReadWriteOnSameFile,SDIFOpeningError)
};

/// \ingroup exception 
/// \brief SDIFBadStdFile
class SDIFBadStdFile : public SDIFOpeningError
{
public:
  constructor(SDIFBadStdFile,SDIFOpeningError)
};

/// \ingroup exception 
/// \brief SDIFBadMode bad mode when opening file
class SDIFBadMode  : public SDIFOpeningError
{
public:
  constructor(SDIFBadMode,SDIFOpeningError)
};

/// \ingroup exception 
/// \brief SDIFHeaderError Frame/Matrix/File Header Errors 
class SDIFHeaderError : public SDIFFileError
{
public:
  constructor(SDIFHeaderError,SDIFFileError)
};

/// \ingroup exception 
/// \brief SDIFFrameHeaderSizeError 
class SDIFFrameHeaderSizeError : public SDIFFileError
{
public:
  constructor(SDIFFrameHeaderSizeError,SDIFFileError)
};

/// \ingroup exception 
/// \brief SDIFBadFormatVersion
class SDIFBadFormatVersion : public SDIFHeaderError
{
public:
  constructor(SDIFBadFormatVersion,SDIFHeaderError)
};

/// \ingroup exception 
/// \brief SDIFBadHeader
class SDIFBadHeader : public SDIFHeaderError
{
public:
  constructor(SDIFBadHeader,SDIFHeaderError)
};

/// \ingroup exception 
/// \brief ASCII Chunk Errors 
class SDIFAsciiChunkError : public SDIFFileError
{
public:
  constructor(SDIFAsciiChunkError,SDIFFileError)
};

/// \ingroup exception 
/// \brief SDIFReDefined
class SDIFReDefined : public SDIFAsciiChunkError
{
public:
  constructor(SDIFReDefined,SDIFAsciiChunkError)
};

/// \ingroup exception 
/// \brief SDIFUnDefined
class SDIFUnDefined  : public SDIFAsciiChunkError
{
public:
  constructor(SDIFUnDefined,SDIFAsciiChunkError)
};

/// \ingroup exception 
/// \brief SDIFReading error during read
class SDIFReading : public SDIFFileError
{
public:
  constructor(SDIFReading,SDIFFileError)
};

/// \ingroup exception 
/// \brief SDIFEof: eof of sdif file reached
class SDIFEof : public SDIFReading
{
public:
  constructor(SDIFEof,SDIFReading)
};

/****************** DESCRIPTION TYPE CHECKING ************/

/// \ingroup exception 
/// \brief SDIFDescriptionTypeError 
class SDIFDescriptionTypeError : public SDIFException{
public:
  constructor(SDIFDescriptionTypeError,SDIFException)
};

/// \ingroup exception 
/// \brief SDIFArrayPosition
class SDIFArrayPosition  : public SDIFReading
{
public:
  constructor(SDIFArrayPosition,SDIFReading)
};

/****************** MATRIX DATA ELEMENT TYPE CHECKING ****/
/// \ingroup exception 
/// \brief SDIFMatrixDataError
class SDIFMatrixDataError : public SDIFException{
public:
  constructor(SDIFMatrixDataError,SDIFException)
};

/// \ingroup exception 
/// \brief SDIFNotInDataTypeUnion
class SDIFNotInDataTypeUnion  : public SDIFMatrixDataError
{
public:
  constructor(SDIFNotInDataTypeUnion,SDIFMatrixDataError)
};

/****************** MEMORY MANAGEMENT ********************/
/// \ingroup exception 
/// \brief SDIFMemoryError memory problems
class SDIFMemoryError : public SDIFException{
public:
  constructor(SDIFMemoryError,SDIFException)
};

/// \ingroup exception 
/// \brief SDIFFreeNull  mem pointer is zero
class SDIFFreeNull : public SDIFMemoryError
{
public:
  constructor(SDIFFreeNull,SDIFMemoryError)
};

/// \ingroup exception  
/// \brief SDIFAllocFail memory allocation failed
class SDIFAllocFail : public SDIFMemoryError
{
public:
  constructor(SDIFAllocFail,SDIFMemoryError)
};

/****************** SDIF-Text Reading ********************/
/// \ingroup exception  
/// \brief SDIFTextReadingError  error reading text section
class SDIFTextReadingError : public SDIFException{
public:
  constructor(SDIFTextReadingError,SDIFException)
};

/// \ingroup exception    
/// \brief SDIFTokenLength
class SDIFTokenLength : public SDIFTextReadingError
{
public:
  constructor(SDIFTokenLength,SDIFTextReadingError)
};

/****************** Frame Contents ****/
/// \ingroup exception    
/// \brief SDIFMatrixNotAvailable requested matrix not in frame
class SDIFMatrixNotAvailable : public SDIFException{  
public:
  constructor(SDIFMatrixNotAvailable,SDIFException)
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

}
} // end of namespace Easdif
#endif
