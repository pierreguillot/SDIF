/**
 * @file   sdifentity.h
 * @author Fabien Tisserand
 * @date   Tue Jun 18 20:12:40 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifentity.h,v 1.3 2002-08-28 16:46:53 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2002/07/12 10:33:56  ftissera
 * *** empty log message ***
 *
 * Revision 1.1  2002/06/18 18:44:14  ftissera
 * Project for new SDIF API
 *  ----------------------------------------------------------------------
 * 
 * 
 */

#ifndef SDIFENTITY_H_
#define SDIFENTITY_H_ 1

#include <string>
#include <vector>
#include <sdif.h>


#include "easdif/sdifframe.h"
#include "easdif/sdifnamevaluetable.h"

/* for initialise SDIF library and install exception*/
#include "easdif/sdifinit.h"

/** 
 * @brief class which can be associated with a file
 *
 * SDIFEntity is composed of different methods which permits the gestion of a 
 * file. 
 * 
 */
class SDIFEntity
{

private:

/** 
 * contain the SDIFNameValueTable of the entity
 */
    std::vector<SDIFNameValueTable> mv_NVT;
    /* for taking a nvt from a file */
    SDIFNameValueTable NVT;

    SdifFileT* efile;
    SdifStringT* mDescription;

    SdifUInt4 mNbFrame;
    SdifUInt4 mSize;

    int mEof;
    int mNbNVT;
    int mOpen;
    size_t generalHeader;
    size_t asciiChunks;
    size_t bytesread;

    //  char* string;
    //  char* filename;


public: 
    /// Default constructor
    SDIFEntity();
    ~SDIFEntity()
	{
	    SdifStringFree(mDescription);
	    SdifFClose(efile);
	};


/*************************************************************************/
/* Description type */
/**
* \defgroup  description SDIFEntity - Description type
*/

/** 
 * \ingroup description
 * add the description type to the frame "1TYP". (for internal used in the
 * Opening)
 */
 private:
    int WriteTypes();
    //int AddDescriptionType();

 public:
/** 
 * \ingroup description
 * add a frame type in the SdifStringT* of the entity
 *
 * the description is added to the file with WriteTypes()
 * in the Opening
 * must be used after AddMatrixType()
 *
 * @param frametype string for define a new frame type 
 * @param matrix string to define the differents matrix which are in 
 * the frame
 * the matrix type
 */
    int AddFrameType(const std::string& frametype, 
		     const std::string& matrix);

/** 
 * \ingroup description
 * defined or redefined a matrix type in the SdifStringT* of the entity.
 * An example of used :AddMatrixType("TYPE", "namecolumn1, namecolumn2, 
 * namecolumn3"). You can have the number of column that you will
 *
 * the description is added to the file with WriteTypes() 
 * in the Opening
 * must be used before AddFrameType()
 *
 * @param matrixtype string for define a new matrix type 
 * or redefined a matrix type 
 * @param colnames string to defined the differents parameters of 
 * the matrix type
 */
    int AddMatrixType(const std::string& matrixtype, 
		      const std::string& colnames);

/** 
 * \ingroup description
 * View the SdifStringT* which have the types definitions of the frames
 * and matrix
 */
    int ViewTypes();

/** 
 * \ingroup description
 * return the SdifStringT* which describe the differents frames of the file
 * in the frame "1TYP".
 */
    SdifStringT* GetTypeString();

/** 
 * \ingroup description
 * Set a SdifStringT* for hte description of the differents frames of the file
 * in the frame "1TYP".
 */
    int SetTypeString(SdifStringT* String);

/*************************************************************************/
/* Change the selection */
/**
 * \defgroup selection SDIFEntity - Selection
 */

 /** 
 * \ingroup selection
  * Replace current selection by new one given in argument.
  * The selection specification may contain all the parts of a filename
  * based selection after the  selection indicator :: .
  */
    int ChangeSelection(const std::string& selection);


/*************************************************************************/
/* Operation with a file */
/**
* \defgroup  file  SDIFEntity - Operations with file
*/

/** 
 * \ingroup  file
 * open the file of the entity in reading or writing mode
 * 
 * @param filename 
 * @param Mode can be "eReadFile" or "eWriteFile"
 */
    int Open(const char* filename, SdifFileModeET Mode);

/** 
 * \ingroup  file
 * open a file in reading mode
 */
    int OpenRead(const char* filename);

/** 
 * \ingroup  file
 * open a file in writing mode
 */
    int OpenWrite(const char* filename);
    
/** 
 * \ingroup  file
 * close a file 
 */
    int Close();
    
/** 
 * \ingroup file
 * get the SdifFileT* file
 */
    SdifFileT* GetFile() const;


    /*temporary SetFile*/
    int SetFile(SdifFileT* SdifFile);
    SdifErrorT* LastError();

/*************************************************************************/
/* Name Value Table */
/**
* \defgroup  nvt  SDIFEntity - Name Value Table
*/

/** 
 * \ingroup nvt
 * to see all the Name Values Tables
 */
    void ViewAllNVTs();

/**
 * \ingroup nvt 
 * take a Name Values Table of a file. (for internal used in opening)
 * @return SDIFNameValueTable of the file
 */
 private:
    SDIFNameValueTable TakeNVT();

 public:
/** 
 * \ingroup nvt
 * insert a SDIFNameValueTable in the vector of Name Value Table
 * 
 * @param nvt Name Value Table to insert in the vector
 * @param StreamID StreamId of the Name Value Table 
 */
    int AddNVT(const SDIFNameValueTable &nvt, SdifUInt4 StreamID=0);


/** 
 * \ingroup nvt
 * write the vector of Name Value Table in the file in the opening in 
 * writing mode
 */
    int WriteNVTs();

/**
 * \ingroup nvt 
 * get the number of Name Value Tables
 */
    int GetNbNVT() const;

/** 
 * \ingroup nvt
 * get the i-th Name Value Table
 */
    SDIFNameValueTable& GetNVT(unsigned int i);




/******************/
/*************************************************************************/
/* Read and Write */
/**
* \defgroup  rnwentity  SDIFEntity - Read and Write
*/

/**
 * \ingroup rnwentity
 * read the next frame of the file
 * return the number of bytes read
 */
    int ReadNextFrame(SDIFFrame& frame);

    int& eof();

/** 
 * \ingroup rnwentity
 * write the next frame of the file
 * return the size of the frame write
 */
    int WriteFrame(SDIFFrame& frame);
    

/*
  TODO
  int GetColumnIndex(SdifSignature matrixsig, std::string columnname);	// do this after Open
*/


};

#endif


