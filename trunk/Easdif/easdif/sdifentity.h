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
 * @file   sdifentity.h
 * @author Fabien Tisserand
 * @date   Tue Jun 18 20:12:40 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifentity.h,v 1.2 2003-04-06 16:31:08 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.7  2002/11/27 20:13:04  roebel
 * Removed member that redundantly indicated size of NVT vector.
 * Make use of improved SDIFNameValueTable interface.
 *
 * Revision 1.6  2002/10/30 15:27:32  roebel
 * Changed return type from int to bool.
 * Changed error checking in openroutines.
 *
 * Revision 1.5  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.4  2002/10/03 11:26:56  tisseran
 * Check if efile is not null before trying to close it.
 * Initialize efile to 0 by sdifentity::sdifentity()
 *
 * Revision 1.3  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
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

namespace Easdif {

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

    SdifFileT* efile;
    SdifStringT* mDescription;

    SdifUInt4 mNbFrame;
    SdifUInt4 mSize;

    bool mEof;

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
	    if (0 != efile)
	    {
		SdifFClose(efile);
		efile = 0;
	    }
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
 *
 * @return true if opened/false if error
 */
    bool Open(const char* filename, SdifFileModeET Mode);

/** 
 * \ingroup  file
 * open a file in reading mode
 * @param filename 
 * @return true if opened/false if error
 */
    bool OpenRead(const char* filename);

/** 
 * \ingroup  file
 * open a file in writing mode
 * @param filename 
 * @return true if opened/false if error
*/
    bool OpenWrite(const char* filename);
    
/** 
 * \ingroup  file
 * close a file 
 * @return true if closed / false if file was not opened
 */
    bool Close();
    
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
 *
 * \return true if ok/false if error.
 */

  bool AddNVT(const SDIFNameValueTable &nvt, SdifUInt4 StreamID=0);


/** 
 * \ingroup nvt
 * write the vector of Name Value Table in the file in the opening in 
 * writing mode
 *
 * \return true if ok/false if error.
 */
    bool WriteNVTs();

/**
 * \ingroup nvt 
 * get the number of Name Value Tables  stored in entity
 * 
 * \return number of Name Value Tables
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


/**
 * \ingroup rnwentity
 * true if file is at eof
 */
    bool& eof();

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

} // end of namespace Easdif

#endif


