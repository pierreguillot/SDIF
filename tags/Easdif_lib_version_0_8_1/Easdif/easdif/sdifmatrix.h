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
 * @file   sdifmatrix.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:45:33 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrix.h,v 1.17 2003-11-25 10:55:42 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.16  2003/11/18 18:21:21  roebel
 * Impemented Get(std::string) for text matrices.
 * declared exceptions for Init and Get methods.
 *
 * Revision 1.15  2003/07/18 20:42:22  roebel
 * Moved constructor implementations to .cpp, fixed bug in constructor with allocation
 *
 * Revision 1.14  2003/07/18 19:31:13  roebel
 * Improved documentation.
 * Changed constructor to use  const std::string as argument.
 * Fixed warning due to comparison between int and unsigned int.
 *
 * Revision 1.13  2003/07/17 18:09:35  roebel
 * Improved Resize method, added Clear method and proper assigment operator
 *
 * Revision 1.12  2003/07/10 16:14:03  roebel
 * Documentation of constructors.
 *
 * Revision 1.11  2003/07/07 10:29:46  roebel
 * Added support for eInt1 and eUInt1 data types, resize of matrix now reinitializes all elements to 0
 *
 * Revision 1.10  2003/06/09 21:13:36  schwarz
 * Implemented Set(const std::string& str), redoes
 * Init(mSig, str.length(), 1, eText) and sets data.
 * Better: check if matrix was declared eText?
 *
 * Revision 1.9  2003/05/19 14:00:00  roebel
 * swig rename moved to swig  interface desription.
 *
 * Revision 1.8  2003/05/18 20:46:46  roebel
 * Added method to read column names of matrix. For this the current sdiffile is stored in the matrix.
 * Improved documentation.
 * Added support for sdif data types eChar and eInt2.
 *
 * Revision 1.7  2003/05/01 19:02:25  roebel
 * Renamed CreateMatrixData to Init.
 * Reorganized Init functions to use only a single argument to specify the matrix signature.
 * Removed redundant m_Signature from class.
 * Added Resize method.
 *
 * Revision 1.6  2003/04/29 15:54:07  schwarz
 * Use SWIG_RENAME_EASDIF to control class renaming.
 *
 * Revision 1.5  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.4  2003/04/29 15:19:51  schwarz
 * Constructor with space initialisation, default singleton float matrix.
 * CreateMatrixData accepts SdifSignature or std::string.
 * Sketch string matrix access functions to be implemented,
 * throw exception when used.
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
 * Revision 1.7  2003/02/10 14:15:03  roebel
 * Comment changed.
 *
 * Revision 1.6  2002/11/07 21:07:24  roebel
 * Allow value type to be specified during Matrix construction.
 * Moved Get/Set methods to header to have the possibility to use
 * real templates.
 *
 * Revision 1.5  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.4  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.3  2002/07/12 10:25:14  ftissera
 * Add comments
 *
 * Revision 1.2  2002/06/18 14:48:30  ftissera
 * add GetSignature(), GetStringSignature() and GetType()
 *
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFMATRIX_H_
#define SDIFMATRIX_H_ 1



#include <string>
#include <sdif.h>
#include "easdif/sdifexception.h"
#include "easdif/sdifmatrixdatainterface.h"
#include "easdif/sdifmatrixdata.h"


namespace Easdif {

/** 
 * @brief class which can be associated with a matrix
 *
 * SDIFMatrix is composed of different methods which permits to manipulate a
 * matrix.
 */


class SDIFMatrix
{
private:
  SDIFMatrixDataInterface* mInter;
  
  int bytesread;
  SdifSignature mSig;
  SdifDataTypeET mType;
  // file matrix was read from
  SdifFileT * mFile;

public:
  /**
   * \defgroup create SDIFMatrix - Construction
   */

  /** 
   * \ingroup create
   * \brief default constructor
   * 
   * Constructs matrix without allocating memory. However, the type
   * for internal data representation is fixed.
   *
   * @param _type data type for internal representation
   *  defaults to: eFloat <br>
   *  supported types: eChar, eInt2, eUInt2, eInt4, eUInt4, eFloat4 , eFloat8.<br>
   *  type extensions: eInt1, eUInt1.<br> Attention the types eInt1 and eUInt1
   *  are not yet
   *  in the sdif standard and if you use them only the IRCAM sdif library will
   *  be able to read your data.
   */
    SDIFMatrix(const SdifDataTypeET _type=eFloat4);


  /** 
   * \ingroup create
   * \brief assignment operator
   *
   * does a deep copy of the data
   * 
   *\param aMatrix matrix to copy
   *
   */
  SDIFMatrix & operator=(const SDIFMatrix& aMatrix);


  /** 
   * \ingroup create
   * \brief copy constrctor
   *
   * does a deep copy of the data
   * 
   *\param aMatrix
   *
   */
  SDIFMatrix(const SDIFMatrix& aMatrix);


  /** 
   * \ingroup create
   * \brief constructor that allocates internal memory
   * 
   * Constructs matrix with given signature
   * to hold given number of rows and columns. The type
   * for internal data representation is fixed.
   *
   * \param sig     Matrix signature
   * \param nrows   number of rows allocated
   * \param ncols   number of cols allocated
   * \param type data type for internal representation
   *  defaults to: eFloat <br>
   *  supported types: eChar, eInt2, eUInt2, eInt4, eUInt4, eFloat4 , eFloat8.<br>
   *  type extensions: eInt1, eUInt1.<br> Attention the types eInt1 and eUInt1
   *  are not yet
   *  in the sdif standard and if you use them only the IRCAM sdif library will
   *  be able to read your data.
   */
  SDIFMatrix(SdifSignature sig, int nrows = 1, int ncols = 1, 
	     SdifDataTypeET type = eFloat4);

  /** 
   * \ingroup create
   * \brief constructor that allocates internal memory
   * 
   * Constructs matrix with given signature
   * to hold given number of rows and columns. The type
   * for internal data representation is fixed.
   *
   * \param sig     4 element string specifying Matrix signature
   * \param nrows   number of rows allocated
   * \param ncols   number of cols allocated
   * \param type data type for internal representation
   *  defaults to: eFloat <br>
   *  supported types: eChar, eInt2, eUInt2, eInt4, eUInt4, eFloat4 , eFloat8.<br>
   *  type extensions: eInt1, eUInt1.<br> Attention the types eInt1 and eUInt1
   *  are not yet
   *  in the sdif standard and if you use them only the IRCAM sdif library will
   *  be able to read your data.
   */
  SDIFMatrix(const std::string& sig, int nrows = 1, int ncols = 1, 
	     SdifDataTypeET type = eFloat4);

    ~SDIFMatrix(){
	if(mInter) {
	    delete mInter;
	    mInter =0;
	}
    };



/*************************************************************************/
/* To see a SDIFMatrix */
/**
* \defgroup print SDIFMatrix - Print
*/

/** 
 * \ingroup print
 * print matrix values
 */
    void Print();

/*************************************************************************/
/* Read and write a SDIFMatrix */
/**
* \defgroup rwmat SDIFMatrix - Read and write and create a SDIFMatrix
*/

/** 
 * \ingroup rwmat
 * write the matrix in the file
 * @return the size in bytes of the matrix writing
 */
    int Write(SdifFileT* file);

/**
 * \ingroup rwmat 
 * read a matrix
 * @return the count of bytes and create a matrix which keep the values
 */
    int Read(SdifFileT* file);

/** 
 * \ingroup rwmat
 * \brief Re-Initialize matrix to hold data
 * 
 * @param sig    SDIFMatrix Signature
 * @param nrows  Number of rows fo the matrix
 * @param ncols  Number of columns of the matrix
 * @param type   SDIFDataType = type of internal representation of the matrix
 */
  void Init(SdifSignature sig, 
	      int nrows, int ncols, SdifDataTypeET  type)   
    throw(SDIFMatrixDataError,std::bad_alloc);

/** 
 * \ingroup rwmat
 * \brief Re-Initialize matrix to hold data
 * 
 * @param sig    SDIFMatrix Signature
 * @param nrows  Number of rows fo the matrix
 * @param ncols  Number of columns of the matrix
 * @param type   SDIFDataType = type of internal representation of the matrix
 */
  void Init(const std::string &sig, 
	      int nrows, int ncols, SdifDataTypeET  type)
    throw(SDIFMatrixDataError,std::bad_alloc);


/** 
 * \ingroup rwmat
 * \brief Resize matrix to hold rowsxcolumns
 * 
 *  Existing data is preserved in the correct locations
 *  newly initilized data is set to zero
 *
 * @param nrows  Number of rows fo the matrix
 * @param ncols  Number of columns of the matrix
 *
 * \return true if successful/false if matrix has not yet been initialized to a signature/data type
 *       
 */
  bool Resize(int nrows, int ncols);

  /** 
   * \ingroup rwmat
   * \brief clear matrix 
   *
   *  all data entries are set to zero.
   * \return true if successful/false if matrix has not yet been initialized to a signature/data type
   *       
   */
  bool  Clear();

/*************************************************************************/
/* Get the members of the matrix */
/**
* \defgroup membmat SDIFMatrix - Get members of the SDIFMatrix 
*/

/** 
 * \ingroup membmat
 * get the size of matrix occupied in SdifFile (including padding)!!
 */
    int GetSize() const;

/**
 * \ingroup membmat 
 * get the number of rows of the matrix
 */
    int GetNbRows() const;

/** 
 * \ingroup membmat
 * get the number of columns of the matrix
 */
    int GetNbCols() const;

/**
 * \ingroup membmat 
 * get the matrix SdifSignature
 */
    SdifSignature GetSignature() const;

/** 
 * \ingroup membmat
 * get the matrix string Signature
 */
    std::string GetStringSignature() const;

/** 
 * \ingroup membmat
 * get the matrix type of data
 */
    SdifDataTypeET GetType() const;

/** 
 * \ingroup membmat
 * get name of column or empty string if unknown
 */
 std::string GetColName(int i) const;

/*************************************************************************/
/* Get the values of the matrix */
/**
* \defgroup valmat SDIFMatrix - Get values of the SDIFMatrix 
*/

/**
 * \ingroup valmat 
 * get a value in : int
 * @param i row index
 * @param j column index
 * 
 * @return the value
 */
    int GetInt(int i, int j);

/**
 * \ingroup valmat  
 * get a value in : float
 * @param i row index
 * @param j column index
 * 
 * @return the value
 */
    float GetFloat(int i, int j);

/**
 * \ingroup valmat  
 * get the value in double
 * 
 * @param i row index
 * @param j column index
 * 
 * @return the value
 */
    double GetDouble(int i, int j);

  /** 
   * \ingroup valmat 
   * \brief get a value in form of given type
   * 
   * @param i row    index
   * @param j column index
   * @param value reference of variable to store value in 
   * 
   */
  template<typename Tout>
  void Get(int i, int j, Tout& value)
  {
    value = static_cast<Tout>(mInter->GetDouble(i, j));
  }

  // specialization for float that does not use cast
  void Get(int i, int j, float& value)
  {
    value = mInter->GetFloat(i, j);
  }
  
  // specialization for int that does not use cast
  void Get(int i, int j, int& value)
  {
    value = mInter->GetInt(i, j);
  }

  // std::string Get() ??? exception when not string matrix?
  void Get(std::string& value)
    throw(SDIFMatrixDataError)
  {

    if (mType != eText)
      throw SDIFMatrixDataError(eError,
				"Error in  SDIFMatrix::!!! string matrix access to matrix containing binary data !!!",
				0,eUnknown,0,0); 

    for(int ii=0;ii<GetNbRows();++ii)
      value.append(1,static_cast<char>(GetInt(ii,0)));

  }


/*************************************************************************
 * Set the values of the matrix  
 */

/**
 * \defgroup setmat SDIFMatrix - Set values of the SDIFMatrix 
 */

  /** 
   * \ingroup setmat
   * \brief set a value using arbitrary input type
   * 
   * @param i    row    index (C-notation: first row has index 0)
   * @param j    column index (C-notation: first col has index 0)
   * @param value 
   * 
   */
  template<typename Tin>
  void Set(int i, int j, const Tin& value)
  {
    mInter->Set(i, j, static_cast<double>(value) );
  }
  
  void Set(int i, int j, const float value)
  {
    mInter->Set(i, j, value);
  }
  
  void Set(int i, int j, const int value)
  {
    mInter->Set(i, j, value);
  }

  /** 
   * Set matrix type to eText, change matrix size to num. of bytes in
   * string and set string data
   */
  void Set(const std::string& str)
  {
      int i;

      Init(mSig, str.length(), 1, eText);

      for (i = 0; i < static_cast<int>(str.length()); i++)
	  mInter->Set(i, 0, str[i]);
  }

};

} // end of namespace Easdif

#endif