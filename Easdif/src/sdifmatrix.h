/* -*-c++-*- */
/**
 * @file   sdifmatrix.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:45:33 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrix.h,v 1.7 2003-02-10 14:15:03 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
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
    std::string m_Signature;
    SdifSignature mSig;
    SdifDataTypeET mType;

public:
    SDIFMatrix(const SdifDataTypeET &_type=eFloat4);
    SDIFMatrix(const SDIFMatrix& aMatrix);

    ~SDIFMatrix(){
	if(mInter) {
	    delete mInter;
	    mInter =0;
	}
    };



/*************************************************************************/
/* To see a SDIFMatrix */
/**
* \defgroup viewm SDIFMatrix - View
*/

/** 
 * \ingroup viewm
 * to see the matrix
 */
    void View();

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
 * create a vector of data for a matrix
 */
    void CreateMatrixData(SdifSignature sig, 
			  int nrows, int ncols, SdifDataTypeET  type);
//type default = eFloat4

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


/*************************************************************************/
/* Get the values of the matrix */
/**
* \defgroup valmat SDIFMatrix - Get values of the SDIFMatrix 
*/

/**
 * \ingroup valmat 
 * get a value in : int
 */
    int GetInt(int i, int j);

/**
 * \ingroup valmat  
 * get a value in : float
 */
    float GetFloat(int i, int j);

/**
 * \ingroup valmat  
 * get a value in : double
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
  void SDIFMatrix::Get(int i, int j, Tout& value)
  {
    value = static_cast<Tout>(mInter->GetDouble(i, j));
  }
  // specialization for float that does not use cast
  void SDIFMatrix::Get(int i, int j, float& value)
  {
    value = mInter->GetFloat(i, j);
  }
  
  // specialization for int that does not use cast
  void SDIFMatrix::Get(int i, int j, int& value)
  {
    value = mInter->GetInt(i, j);
  }


/*************************************************************************/
/* Set the values of the matrix */
/**
* \defgroup setmat SDIFMatrix - Set values of the SDIFMatrix 
*/

  /** 
   * \ingroup setmat
   * \brief set a value using arbitrary input type
   * 
   * @param i    row    index
   * @param j    column index
   * @param value 
   * 
   */
  template<typename Tin>
  void Set(int i, int j, const Tin& value)
  {
    mInter->Set(i, j, static_cast<double>(value) );
  }
  
  void
  Set(int i, int j, const float& value)
  {
    mInter->Set(i, j, value);
  }
  
  void Set(int i, int j, const int& value)
  {
    mInter->Set(i, j, value);
  }

};

} // end of namespace Easdif

#endif


