/* -*-c++-*- */
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
 * @file   sdifframe.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:29:25 2002
 * 
 * @brief  
 * 
 * sdifframe.h is composed of the different methods which are using to 
 * manipulate the frame.
 * 
 * $Id: sdifframe.h,v 1.15 2004-08-26 09:33:12 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.14  2004/08/25 18:23:56  roebel
 * Version 0.11.0
 * removed SetNbMatrix which was confusing because people used it
 * to resize a Frame. Added a real resize function.
 * Added size(), resize() and clear() functions to
 * have more consistent interface compared to stl containers.
 *
 * Revision 1.13  2004/07/27 17:40:19  roebel
 * Changed include directive to use user path and not system path for sdif.h
 *
 * Revision 1.12  2004/07/21 13:20:38  roebel
 * Added support to hold a frameDirectory in the entity and
 * the possibility to read frames from a given time position.
 *
 * Revision 1.11  2004/02/02 18:07:27  roebel
 * Version 0.8.1: bug fix in sdifframe establishing the possibility to
 * resize matrices within the frame.
 *
 * Revision 1.10  2003/11/18 18:27:04  roebel
 * Added exceptions when accessing non existing matrices in a frame.
 *
 * Revision 1.9  2003/07/21 09:59:43  roebel
 * Fixed Error in Documentation
 *
 * Revision 1.8  2003/07/18 19:32:41  roebel
 * Improved documentation.
 * Added SetHeader with const std::string instead of SdifSignature.
 *
 * Revision 1.7  2003/05/19 14:00:00  roebel
 * swig rename moved to swig  interface desription.
 *
 * Revision 1.6  2003/05/18 21:08:37  roebel
 * Added appropriate const methods.
 * For proper swig access GetMatrixWithSignature does no longer take
 * a reference as argument.
 *
 * Revision 1.5  2003/04/29 15:54:06  schwarz
 * Use SWIG_RENAME_EASDIF to control class renaming.
 *
 * Revision 1.4  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.3  2003/04/18 16:44:00  schwarz
 * Small changes to make easdif swiggable:
 * - name change for swig-generated classes
 * - eof() returns bool, not bool&
 * - Matrix::Set takes int/float, not int&/float&
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.5  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.4  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.3  2002/07/12 10:19:03  ftissera
 * *** empty log message ***
 *
 * Revision 1.2  2002/06/18 14:51:13  ftissera
 * add methods with SDIFEntity for reading and writing
 *
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFFRAME_H_
#define SDIFFRAME_H_ 1

#include <string>
#include "sdif.h"
#include "easdif/sdifmatrix.h"
#include "easdif/sdifexception.h"

namespace Easdif {

class SDIFEntity;

/** 
 * @brief class which can be associated with a frame
 *
 * SDIFFrame is composed of different methods which permits to manipulate a
 * frame. All the methods with a SdifFileT* parameter are surcharged with
 * SDIFEntity parameter.
 */


class SDIFFrame
{
private:
    std::vector<SDIFMatrix> mv_Matrix;
    //SDIFMatrix matrix;

    size_t  mFrameBytesRead;
    //int mSelected;// if is selected = 1 else = 0

    SdifFloat8      mTime;
    SdifSignature   mSig;
    SdifUInt4       mStreamID;
    SdifUInt4       mNbMatrix;
    //SdifUInt4       mSelected;


public: 

  
  /**
   * \defgroup frconstr SDIFFrame - Constructor
   */

  /** 
   * \ingroup frconstr
   * \brief default constructor
   *
   *  Constructs a frame 
   */
  SDIFFrame()	{ClearData();}

/** 
 * Read : permit to read entirely a frame : the header and the data
 * ReadHeader : read the frame header (used by Read)
 * ReadData : read only the data of a frame (used by Read)
 * Write : write a frame
 * WriteHeader : write the frame header and the data (used by Write)
 * Print : see the content of a frame
 * PrintHeader : see the frame header
 * Resize : resize the vector of the frame that stock the reading
 * ClearData : reinitialize the frame, will erase all matrices and all header infos
 *
 * @param file 
 */
   
/*************************************************************************/
/* Read and Write file */
/**
* \defgroup rnw SDIFFrame - Read and Write 
*/

/** 
 * \ingroup rnw
 * read entirely a frame : the header and the data
 * @return number of bytes read
 *
 * Attention: using this function bypasses the update of the FrameDirectory
 * in the SDIFEntity! 
 */
    int  Read(SdifFileT* file, bool &eof);

/**
 * \ingroup rnw 
 * read the frame header (used by Read)
 * @return number of bytes read
 */
    int  ReadData(SdifFileT* file);

/** 
 * \ingroup rnw
 * read only the data of a frame (used by Read)
 * @return number of bytes read
 */
    int  ReadHeader(SdifFileT* file);

/** 
 * \ingroup rnw
 * write entirely a frame
 * @return number of bytes written
 */
    int  Write(SdifFileT* file);

/** 
 * \ingroup rnw
 * write the frame header (used by Write)
 * @return number of bytes written
 */
    int  WriteHeader(SdifFileT* file);

    /* for SDIFEntity*/
    //int  Read(const SDIFEntity& entity);
    int  Read(SDIFEntity& entity);
    int  ReadData(const SDIFEntity& entity);
    int  ReadHeader(const SDIFEntity& entity);
    int  Write(const SDIFEntity& entity);
    int  WriteHeader(const SDIFEntity& entity);


/*************************************************************************/
/* Add a SDIFMatrix in the frame */
/**
* \defgroup addframe SDIFFrame - Add SDIFMatrix 
*/

/**
 * \ingroup addframe 
 * add a SDIFMatrix in the matrix vector of the frame
 * @return the number of SDIFMatrix contained in the vector
 */
    int  AddMatrix(const SDIFMatrix& aMatrix);

/** 
 * \ingroup addframe
 * add a matrix in the matrix vector of the frame if this matrix is
 * selected
 * @return the number of SDIFMatrix contained in the vector
 */
    int  AddMatrixSelected(SdifFileT* file, const SDIFMatrix& aMatrix);


/*************************************************************************/
/* Other */
/**
* \defgroup otherframe SDIFFrame - Other 
*/

/** 
 * \ingroup otherframe
 * @brief see the content of a frame
 */
    void Print();

/**
 * \ingroup otherframe 
 * @brief see the frame header
 */
    void PrintHeader();

/**
 * \ingroup otherframe
 * @brief reinitialize the frame: remove all matrices and set all headers to initial default values
 *
 *  use Resize(0) to clear only the matrices.
 */
    void ClearData();

/**
 * \ingroup otherframe
 * @brief reinitialize the frame: remove all matrices and set all headers to initial default values
 */
    void clear(){ClearData();};

/**
 * \ingroup otherframe
 * @brief resize the vector to hold _size SDIFMatrices 
 * \param _size number of matrices that should be in the frame
 */
  void Resize(int _size);

/**
 * \ingroup otherframe
 * @brief resize the vector to hold _size SDIFMatrices 
 * \param _size number of matrices that should be in the frame
 */
  void resize(int _size){Resize(_size);}


/*************************************************************************/
/* Get Informations */
/**
* \defgroup infoframe SDIFFrame - Get Informations 
*/

    /* get the matrix signature selected */
/** 
 * \ingroup infoframe
 * get the matrix signature selected which is stored in file
 * @return SdifSignature of SDIFMatrix selection
 */
    SdifSignature GetMatrixSelection(SdifFileT* file) const;

    SdifSignature GetMatrixSelection(const SDIFEntity& entity)  const;

/**
 * \ingroup infoframe 
 * @brief check if a matrix type exist in the frame with a SdifSignature
 */
    bool MatrixExists(const SdifSignature& sig) const;

/** 
 * \ingroup infoframe
 * @brief check if a matrix type exist in the frame with a string signature
 */
    bool MatrixExists(const std::string& signature) const;


/*************************************************************************/
/* Get SDIFMatrix */
/**
* \defgroup mat SDIFFrame - Get a SDIFMatrix 
*/

/**
 * \ingroup mat 
 * \brief get the matrix number i which is stored in the vector of matrix 
 *
 * \param index  index to matrix (C-notation: first matrix at index 0)
 *
 * @return SDIFMatrix number i
 */
    SDIFMatrix& GetMatrix(unsigned int index) throw(Easdif::SDIFMatrixNotAvailable);
    const SDIFMatrix& GetMatrix(unsigned int index) const throw(Easdif::SDIFMatrixNotAvailable);


/** 
 * \ingroup mat
 * get the matrix of SdifSignature : sig in the vector of matrix
 * @return SDIFMatrix
 */
    SDIFMatrix& GetMatrixWithSig(const SdifSignature sig) throw(SDIFMatrixNotAvailable);/*ambiguity 
					   between signature and int
					   -> other name : GetMatrixwithSig()*/
    const SDIFMatrix& GetMatrixWithSig(const SdifSignature sig) const throw(SDIFMatrixNotAvailable);

/**
 * \ingroup mat 
 * get the matrix with the signature in the vector of matrix
 * @param signature string
 * @return SDIFMatrix
 */
  SDIFMatrix& GetMatrix(const std::string& signature) throw(SDIFMatrixNotAvailable);
  const SDIFMatrix& GetMatrix(const std::string& signature) const throw(SDIFMatrixNotAvailable);


  /** 
   *  \ingroup mat 
   *   SDIFFrame iterators iterate over the matrices conatined in the
   *   frame. These are STL compliant random access iterators and they  
   *   exist as iterator and const_iterator
   */
  typedef std::vector<SDIFMatrix>::iterator iterator;
  typedef std::vector<SDIFMatrix>::const_iterator const_iterator;

/**
 * \ingroup mat 
 * get an the iterator pointing to the first matrix in the frame
 * 
 * @return SDIFFRame::iterator
 * const Frames return const_iterators
 */
  iterator begin(){return mv_Matrix.begin();}
  const_iterator begin() const {return mv_Matrix.begin();}

/**
 * \ingroup mat 
 * get an the iterator pointing after the last matrix in the frame
 * 
 * @return SDIFFRame::iterator
 * const Frames return const_iterators
 */
  iterator end(){return mv_Matrix.end();}
  const_iterator end() const {return mv_Matrix.end();}


  /*************************************************************************/
  /* Get members of the frame */
  /**
   * \defgroup  getmframe SDIFFrame - Get members
   */
  
  /**
   * \ingroup getmframe 
   * @brief get the number of matrix in the frame
   */
  SdifUInt4 GetNbMatrix() const  {    return mv_Matrix.size();}

  /**
   * \defgroup  getmframe SDIFFrame - Get members
   */
  
  /**
   * \ingroup getmframe 
   * @brief get the number of matrix in the frame
   */
  SdifUInt4 size() const  {    return mv_Matrix.size();}
  
  /** 
   * \ingroup getmframe
   * @brief get the signature of the frame
   */
  SdifSignature GetSignature() const{    return mSig;}
  
  
  /** 
   * \ingroup getmframe
   * @brief get the signature of the frame
   */
  void GetSignature(std::string& sig) const
  {
    sig = SdifSignatureToString(mSig);
    return;
  }

  /** 
   * \ingroup getmframe
   * @brief get the streamID of the frame
   */
  SdifUInt4 GetStreamID() const{ return mStreamID;}

  
  /** 
   * \ingroup getmframe
   * @brief get the time of the frame
   */
  SdifFloat8 GetTime() const {  return mTime;}

  /** 
   * \ingroup getmframe
   * @brief get the size of the frame
   */
  SdifUInt4     GetSize() const;
  
  /*************************************************************************/
  /* Set the informations of the frames */
  /**
   * \defgroup  setmframe SDIFFrame - Set members
   */
  
/** 
 * \ingroup setmframe
 * @brief Set the frame header
 */
  void SetHeader(SdifSignature sig, SdifUInt4 streamID, float time);//, SdifUInt4 nbMatrix);
  
  /** 
   * \ingroup setmframe
   * @brief Set the frame header
   */
  void SetHeader(const std::string& sig, SdifUInt4 streamID, float time);
    
  /** 
   * \ingroup setmframe
   * @brief Set one element of the frame header : the signature
   */
  void SetSignature(SdifSignature sig);
  
  /** 
   * \ingroup setmframe
   * @brief Set one element of the frame header : the signature with a string
   */
  void SetSignature(const std::string& signature);
  
  /** 
   * \ingroup setmframe
   * @brief Set one element of the frame header : the streamID
   */
  void SetStreamID(SdifUInt4 streamID);
  
  /** 
   * \ingroup setmframe
   * @brief Set one element of the frame header : the time
   */
  void SetTime(float time);

};
} // end of namespace Easdif

#endif
