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
 * $Id: sdifentity.h,v 1.22 2004-09-08 09:15:57 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.21  2004/08/25 09:22:11  roebel
 * SDIFEntity::Rewind(time) no longer private.
 *
 * Revision 1.20  2004/07/28 13:03:27  roebel
 * Fixed position type to be SdiffPosT
 *
 * Revision 1.19  2004/07/27 17:40:19  roebel
 * Changed include directive to use user path and not system path for sdif.h
 *
 * Revision 1.18  2004/07/21 13:20:24  roebel
 * Added support to hold a frameDirectory in the entity and
 * the possibility to read frames from a given time position.
 *
 * Revision 1.17  2004/07/13 15:01:42  roebel
 * Removed unused variable mNbFrames.
 *
 * Revision 1.16  2004/05/04 12:53:35  roebel
 * Fixed documentation.
 *
 * Revision 1.15  2004/02/11 19:28:17  roebel
 * Added inline function to test state of entity. Added function to access NVTs that indicates existance of name.
 *
 * Revision 1.14  2004/01/19 15:49:55  bogaards
 * Added Rewind and ReadNextSelectedFrame methods to Entity
 *
 * Revision 1.13  2003/08/06 18:00:33  roebel
 * Fixed documentation
 *
 * Revision 1.12  2003/07/18 21:19:36  roebel
 * Made WriteNVTs private. It is for internal use only.
 *
 * Revision 1.11  2003/07/18 20:39:35  roebel
 * Improved documentation
 *
 * Revision 1.10  2003/05/24 00:27:38  roebel
 * Internal representation of types now using std::string.
 * Parameters for type handling routines completely moved
 * to std::string
 *
 * SDIFEntity::SetTypeString no longer appends but sets
 * the internal string. In fact there was now way to reset
 * the type string before.
 *
 * Revision 1.9  2003/05/19 13:59:40  roebel
 * swig rename moved to swig  interface desription.
 *
 * Revision 1.8  2003/05/01 18:59:12  roebel
 * Added missing class keyword for friend declaration.
 *
 * Revision 1.7  2003/04/29 15:54:04  schwarz
 * Use SWIG_RENAME_EASDIF to control class renaming.
 *
 * Revision 1.6  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.5  2003/04/18 17:43:22  schwarz
 * eof() const method
 *
 * Revision 1.4  2003/04/18 16:44:00  schwarz
 * Small changes to make easdif swiggable:
 * - name change for swig-generated classes
 * - eof() returns bool, not bool&
 * - Matrix::Set takes int/float, not int&/float&
 *
 * Revision 1.3  2003/04/08 17:26:04  roebel
 * Updated doc.
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
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
#include <list>
#include "sdif.h"


#include "easdif/sdifframe.h"
#include "easdif/sdifnamevaluetable.h"

/* for initialise SDIF library and install exception*/
#include "easdif/sdifinit.h"

namespace Easdif {

/** 
 * @brief class holding all information concerned with a singe sdif file.
 *
 * SDIFEntity is composed of different methods that allow the handling of an 
 * sdif-file. 
 * 
 */


class SDIFEntity
{
    friend class SDIFFrame;

private:

/** 
 * contain the SDIFNameValueTable of the entity
 */
  std::vector<SDIFNameValueTable> mv_NVT;
  
  SdifFileT* efile;
  std::string mDescription;
  
  SdifUInt4 mSize;
  SdiffPosT mFirstFramePos;	// file position after reading the header
  
  bool mEof;
  
  int mOpen;
  size_t generalHeader;
  size_t asciiChunks;

  //  pairs of time and file position serve as directory
  struct SDIFLocation {
    std::pair<SdifFloat8,SdifUInt4> loc;
    SDIFLocation():loc(-1.,0){}
    SDIFLocation(SdifFloat8 _time,SdifUInt4 _pos):loc(_time,_pos){}
    SdifFloat8  GetTime() const { return loc.first;}
    SdifUInt4   GetPos()  const { return loc.second;}
  };

  std::list<SDIFLocation> mFrameDirectory;
  std::list<SDIFLocation>::iterator mCurrDirPos;
  bool isFrameDirEnabled;

public: 
    /// Default constructor
    SDIFEntity();
    ~SDIFEntity()
	{	    
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
 * \brief write type info
 *
 * add the description type to the frame "1TYP". (for internal use when
 * opening the file )
 */
 private:
    bool WriteTypes();
    //int AddDescriptionType();

 public:
/** 
 * \ingroup description
 *
 * \brief add new frame type  or change existing  frame type  of the entity
 *
 * the description is added to the file with WriteTypes()
 * when the entity is opened for wrinting.Therefore, 
 * type descriptors  have to be added
 * to the entity before the file is opened.
 *
 * AddFrameType() must be used after Easdif::SDIFEntity::AddMatrixType()
 *
 * @param frametype string that identifies the  new frame type,
 *  the identifier will use  at most 4 characters
 * @param matrix string that defines the matrix elements that may be part  
 * of the frame
 *
 * Example: 
 * 
 *  entity.AddFrameType("1NEW", "1NEW NewMatrix; 1FQ0 New1FQ0");
 *
 */
    bool AddFrameType(const std::string& frametype, 
		      const std::string& matrix);

/** 
 * \ingroup description
 * 
 * \brief define a new or redefine an existing matrix type for  the entity.
 *
 * The type description is added to the entity when the entity is
 * opened for writing. Therefore, type descriptors  have to be added
 * to the entity before the file is opened.
 *
 * AddMatrixType() must be used before Easdif::SDIFEntity::AddFrameType()
 *
 * @param matrixtype string for define a new matrix type 
 * or redefined a matrix type 
 * @param colnames string to defined the differents parameters of 
 * the matrix type
 *
 * Example: 
 * 
 *  entity.AddMatrixType("1NEW", "amplitude, phase");
 *
 */
    bool AddMatrixType(const std::string& matrixtype, 
		       const std::string& colnames);

/** 
 * \ingroup description
 * Print the SdifStringT* which have the types definitions of the frames
 * and matrix
 */
    int PrintTypes();

/** 
 * \ingroup description
 * return a string  containing the user defined types of the
 * file that is stored in the frame "1TYP".
 *
 *  return Type string
 */
  const std::string& GetTypeString() const;

/** 
 * \ingroup description
 * Set the user defined frame types in the frame "1TYP" for the current file to
 * the argument string 
 * 
 * \param TypeString
 * \return return true if success
 */
  bool SetTypeString(const std::string& TypeString);

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
 * \ingroup  file
 * rewind a file to first non-ascii frame after the file header
 * @return true if positioning was successful
 */
    
    bool Rewind();


  /** 
   * \ingroup  file
   * rewind a file to a frame that has a time position
   * that is guaranteed to be before the time specified in timePos
   * if the directory information is incomplete (due to selection)
   * the frame may be well before the time required.
   *
   * Postion will be on end of file if there  exists
   * no Frame after timePos
   *
   *
   * @return true if positioning was successful
   */
    
    bool Rewind(SdifFloat8 timePos);

  /** 
   * \ingroup file
   * get the SdifFileT* file
   * \return SdifFile pointer related to Entity
   */
  SdifFileT* GetFile() const;

  /** 
   * \ingroup file
   *
   * test file state
   *
   * \return true if file is opened and not at eof
   */
  bool good() const {
    return (!eof() && mOpen && efile);
  }

/**
 * \ingroup file
 * true if file is at eof
 */
  bool eof() const {return mEof;}


  /**
   * \ingroup file
   * enable Frame Directory:
   *
   * If enabled the entity will internally keep track of the positions of all
   * frames and thereby allow to quickly reposition to a desired frame
   * by specifying the requested Frame time.
   *
   * enableFrameDir can be called at any time, however, is most effectively
   * set before any frames are read. If it is enabled later the 
   * all frames up to the current frame are reread to obtain a complete
   * and consistent directory. 
   */
  void EnableFrameDir();

  /**
   * \ingroup file
   * true if frame Directory is enabled
   */
  bool IsFrameDir() {   return isFrameDirEnabled;}


  /**
   * \ingroup file
   * dump content of Frame Directory to stderr
   */
  void PrintFrameDir() const;

private:

  /**
   * \ingroup file
   *
   * add a new time/position point to the directory
   * used only internally to maintain directory from 
   * the frame read interface.
   *
   */
  void AddFramePos(SdifFloat8 time,SdifUInt4 pos);

private:
  // this appears to be a remaining of the initial development 
  // will be removed in the future
    /*temporary SetFile*/
    int SetFile(SdifFileT* SdifFile);

public:
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
    void PrintAllNVTs();

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
 * write the vector of Name Value Table in the file when opening a file 
 * for writing (for internal use only)
 *
 * \return true if ok/false if error.
 */
private:
    bool WriteNVTs();
public:

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
 * read the next frame of the file that is located after time timePos 
 * return the number of bytes read
 *
 * Calling this function will automatically enable the internal
 * FrameDirectory.
 *
 * \see EnableFrameDir()
 */
    int ReadNextFrame(SDIFFrame& frame, SdifFloat8 timePos);

/**
 * \ingroup rnwentity
 * read the next selected frame of the file
 * return the number of bytes read
 */

  int ReadNextSelectedFrame(SDIFFrame& frame);

/**
 * \ingroup rnwentity
 * read the next selected frame of the file starting from position timePos
 * return the number of bytes read
 *
 * Calling this function will automatically enable the internal
 * FrameDirectory.
 *
 * \see EnableFrameDir()
 */
  int ReadNextSelectedFrame(SDIFFrame& frame,SdifFloat8 timePos);


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


