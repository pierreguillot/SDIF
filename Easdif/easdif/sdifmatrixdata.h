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
 * @file   sdifmatrixdata.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:34:25 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrixdata.h,v 1.6 2003-07-17 18:09:35 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.5  2003/07/07 10:29:46  roebel
 * Added support for eInt1 and eUInt1 data types, resize of matrix now reinitializes all elements to 0
 *
 * Revision 1.4  2003/05/18 23:14:10  roebel
 * Improved error message should be an exception
 *
 * Revision 1.3  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.6  2002/11/07 21:09:33  roebel
 * Provide clone method.
 * Fixed bounds checking in Get template.
 * Reorganized CopyData.
 *
 * Revision 1.5  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.4  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.3  2002/07/12 10:27:34  ftissera
 * *** empty log message ***
 *
 * Revision 1.2  2002/06/18 14:47:45  ftissera
 * add comments
 * 
 *
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFMATRIXDATA_H_
#define SDIFMATRIXDATA_H_ 1

#include <vector>
#include <sdif.h>
#include "easdif/sdifmatrixdatainterface.h"

namespace Easdif {

/** 
 * @brief class which can store the value in template type
 *  class for the data storage and the operation on data
 */
template <class T>
class SDIFMatrixData : public SDIFMatrixDataInterface//SdifMatrixDataInterface
{

private:

    int m_Nrows;
    int m_Ncols;

/** 
 * vector which store the data in natural type
 */
    std::vector<T> m_Data;

public:
    SDIFMatrixData(int nrows, int ncols)
	{
	    if (nrows<0|| ncols<0 )
	    {
		m_Nrows=0;
		m_Ncols=0;
		m_Data.resize(0);
	    }
	    m_Nrows = nrows;
	    m_Ncols = ncols;
	    m_Data.resize(m_Nrows*m_Ncols);
	};

    ~SDIFMatrixData()
	{
	};
	


/*************************************************************************/
/* Get data */
/**
* \defgroup getdata SDIFMatrixData - Get data 
*/


  /** 
   *  template method for getting data
   */
  template <class TT>
  TT Get(int i, int j)
  {
    if (i<0 || i >= m_Nrows || j >= m_Ncols || j<0)
      {
	std::cerr<<"Error in Get(" <<i << "," <<j <<") out of matrix range ("<<m_Nrows<<","<<m_Ncols<<")" <<std::endl;
	return -1;
      }
    return static_cast<TT>(m_Data[i*m_Ncols+j]);
  }

/**
 * \ingroup getdata
 * get the value in double
 * 
 * @param i row index
 * @param j column index
 * 
 * @return the value
 */
    double GetDouble(int i, int j)
	{
	    return Get<double>(i, j);   
	}

/** 
 * \ingroup getdata
 * get the value in float
 * 
 * @param i row index
 * @param j column index
 * 
 * @return the value
 */
    float GetFloat(int i, int j)
	{
	    return Get<float>(i, j);   
	}

/** 
 * \ingroup getdata
 * get the value in int
 * 
 * @param i row index
 * @param j column index
 * 
 * @return the value
 */
    int GetInt(int i, int j)
	{	    
	    return Get<int>(i, j);   
	}

/** 
 * \ingroup getdata
 * get the vector of data
 * @return vector of template type
 */
    inline std::vector<T>& GetData()
	{
	    return m_Data;
	}


/*************************************************************************/
/* Get members*/
/**
* \defgroup getmemb SDIFMatrixData - Get members
*/
/** 
 * \ingroup getmemb
 * @brief get the row number of values
 */
   inline  int GetNbRow()
	{
	    return m_Nrows;
	}

/** 
 * \ingroup getmemb
 * @brief get the column number of values
 */
    inline int GetNbCol() 
	{
	    return m_Ncols;
	}

/*************************************************************************/
/* Read and Write data */
/**
* \defgroup rnwdata SDIFMatrixData - Read and Write data 
*/

/** 
 * \ingroup rnwdata
 * read the data in the file and store in a vector of data in template type
 * 
 * @param file 
 */
    int read(SdifFileT* file)
	{
	    int bytesread = 0;
	    /*Read matrix data*/
	    for (int row = 0; row < m_Nrows; row++)
	    {
		bytesread += SdifFReadOneRow(file);
		T* p= static_cast<T*>(SdifFCurrOneRowData(file));
		
		std::copy(p,p+m_Ncols,&(m_Data[row*m_Ncols]));
	    }
	    /*Read matrix padding*/
	    bytesread += SdifFReadPadding(file, SdifFPaddingCalculate(file->Stream, bytesread));
	    return bytesread; 
	}

/**
 * \ingroup rnwdata 
 * take the SDIFMatrixData values in the vector and write them in the
 * file
 * 
 * @param file 
 */
    int write(SdifFileT* file)
	{
	    int SizeFrameW = 0;
	    /* Write matrix data */
	    for (int row = 0; row < m_Nrows; row++)
	    {
		for (int col = 1; col <= m_Ncols; col++)
		{
		    T value = static_cast<T>(m_Data[row*m_Ncols+col-1]);
		    SdifFSetCurrOneRowCol (file, col, value);	   	 
		}
		SizeFrameW += SdifFWriteOneRow (file);
	    }

	    /* Write matrix padding */
	    SizeFrameW += SdifFWritePadding(file, SdifFPaddingCalculate (file->Stream, SizeFrameW));

	    return SizeFrameW;
	}


/*************************************************************************/
/* Other */
/**
* \defgroup otherdata SDIFMatrixData - Other
*/


/** 
 * \ingroup otherdata
 * used for printing the data 
 */
    void print()
	{
	    int row;
            int col;
	    for (row = 0; row < m_Nrows; row++)
	    {
	        for (col = 0; col < m_Ncols; col++)
		{		    
		    std::cout <<m_Data[row*m_Ncols+col] << " ";
		}
		std::cout << std::endl;
	    }

	}


  /**
   * \brief clone matrix data
   *
   */
  SDIFMatrixData *clone() {
    SDIFMatrixData<T> *tmp = 
      new SDIFMatrixData<T>(this->m_Nrows,this->m_Ncols);
    tmp->CopyData(this->m_Data);    
    return tmp;
  }


  void CopyData(std::vector<T>& data)
  {
    m_Data.resize(data.size());
    std::copy(data.begin(), data.end(), m_Data.begin());
  }

  /** 
   * \ingroup otherdata
   * \brief clear matrix 
   *
   *  all data entries are set to zero.
   */
  void Clear() {
    std::fill(&m_Data[0],&m_Data[m_Data.size()],T(0));
  }

  /** 
   * \ingroup otherdata
   * \brief resize the matrix 
   *
   *  Existing data is preserved in the correct locations
   *  newly initilized data is set to zero
   * 
   * @param nrows 
   * @param ncols 
   */
  void Resize(int nrows, int ncols)
  {

    if(nrows != m_Nrows || ncols != m_Ncols){
      int i,j;
      
      // Resize sufficiently
      if(static_cast<size_t>(nrows*ncols) > m_Data.size()) {
	m_Data.resize(nrows*ncols);
      }
      
      // reorganize data
      if (m_Ncols < ncols) {
	// start from the end
	// fill all new fields with zero
	for (i = nrows-1; 
	     i >= m_Nrows ; --i){
	  for (j = ncols-1; j >= 0; --j)
	    m_Data[i * ncols + j] = T(0);
	}
	
	for (; i >= 0; --i){
	  for (j = ncols-1; j >=m_Ncols ; --j)
	    m_Data[i * ncols + j] = T(0);
	  
	  for (; j >= 0; --j)
	    m_Data[i * ncols + j] = m_Data[i * m_Ncols + j];
	}
      }
      else{
	// Start from the beginning
	// The first row will need no change
	int prows = m_Nrows;
	if (m_Nrows > nrows) prows = nrows;
	
	for (i = 1; i < prows; ++i){
	  for (j=0; j <ncols; ++j)
	    m_Data[i * ncols + j] = m_Data[i * m_Ncols + j];
	}
	for (; i < nrows; ++i){
	  for (j=0; j <ncols; ++j)
	    m_Data[i * ncols + j] = T(0);	    
	}
      }
      
      // If size has been shrinked --> adjust properly
      if(static_cast<size_t>(ncols*nrows) < m_Data.size()) {
	m_Data.resize(ncols*nrows);
      }
      
      
      m_Ncols   = ncols;
      m_Nrows   = nrows;
    }

    return;
  }



/*************************************************************************/
/* Set the data */
/**
* \defgroup setdata SDIFMatrixData - Set data 
*/

/** 
 * \ingroup setdata
 * set a value in the matrix using double input type
 * 
 * @param i 
 * @param j 
 * @param value 
 */
  void Set(int i, int j, const double& value)
  {
    m_Data[i*m_Ncols+j] = static_cast<T>(value);
  }

/** 
 * \ingroup setdata
 * set a value in the matrix using float input type
 * 
 * @param i 
 * @param j 
 * @param value 
 */
  void Set(int i, int j, const float& value)
  {
    m_Data[i*m_Ncols+j] = static_cast<T>(value);
  }

/** 
 * \ingroup setdata
 * set a value in the matrix using int input type
 * 
 * @param i 
 * @param j 
 * @param value 
 */
  void Set(int i, int j, const int& value)
  {
    m_Data[i*m_Ncols+j] = static_cast<T>(value);
  }

};

} // end of namespace Easdif

#endif
