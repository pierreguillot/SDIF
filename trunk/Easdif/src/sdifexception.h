/* g++ ex_excep.cpp -Wall -o ex_excep */
#ifndef ERR_EXCEP_H_
#define ERR_EXCEP_H_ 1

#include <iostream>
#include <string>
#include <sdif.h>
//#include "SdifErrMess.h"
//#include "SdifError.h"

/*
in this class, we are interested just in errors (not warnings)
*/
class SDIFException
{
public:
    SDIFException(): mIsInit(false) {};

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
		mSourceFileName = std::string(sourceFileName);
		mMessage = std::string(message);
		mLevel = level;
		mSdifFile = sdifFile;
		mError = error;
		mIsInit = true;
	    }
	    return mIsInit;
	}

    void ErrorMessage()
	{
	    std::cerr << "Error in file : "
		      << mSourceFileName 
		      << "\nat line : "
		      << mSourceFileLine
		      << std::endl
		      << mMessage 
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
#endif
