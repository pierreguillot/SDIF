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
 * $Id: sdifmatrix.h,v 1.4 2002-08-28 16:46:53 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
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
    SDIFMatrix();
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
 * get the size of matrix
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
 * get a value in : int
 */
    int Get(int i, int j, int& value);

/** 
 * \ingroup valmat 
 * get a value in : float
 */
    float Get(int i, int j, float& value);

/** 
 * \ingroup valmat 
 * get a value in : double
 */
    double Get(int i, int j, double& value);

/*************************************************************************/
/* Set the values of the matrix */
/**
* \defgroup setmat SDIFMatrix - Set values of the SDIFMatrix 
*/

/** 
 * \ingroup setmat
 * set a value in : int
 */
    int Set(int i, int j, const int& value);

/**
 * \ingroup setmat 
 * set a value in : float
 */
    int Set(int i, int j, const float& value);

/**
 * \ingroup setmat 
 * set a value in : double
 */
    int Set(int i, int j, const double& value);
};

#endif
