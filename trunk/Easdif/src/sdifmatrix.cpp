/**
 * @file   sdifmatrix.cpp
 * @author Fabien Tisserand
 * @date   Thu Apr 11 18:36:39 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrix.cpp,v 1.5 2002-08-28 16:46:53 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.4  2002/07/12 10:20:04  ftissera
 * Read has changed for selection
 *
 * Revision 1.3  2002/06/18 14:48:53  ftissera
 * add GetSignature(), GetStringSignature() and GetType()
 *
 * Revision 1.2  2002/04/11 16:41:55  ftissera
 * add comment for sdifmatrix.cpp
 * 
 * 
 */

#include "easdif/sdifmatrix.h"

SDIFMatrix::SDIFMatrix():mInter(0)
{
    //m_Signature =...;
    /* default type */
    mType = eFloat4;
    /* default signature */
    //mSig = SdifSignatureConst('1','T','R','C');
    // signifies uninitialized matrix
    mSig = 0;
    CreateMatrixData(mSig, 1, 1, mType);
}

SDIFMatrix::SDIFMatrix(const SDIFMatrix& aMatrix):mInter(0)
{
    // Check     
    m_Signature = aMatrix.m_Signature;
    mSig = aMatrix.mSig;
    mType = aMatrix.mType;
    bytesread = aMatrix.bytesread;
    int mNrows = aMatrix.mInter->GetNbRow();
    int mNcols = aMatrix.mInter->GetNbCol();
    
   CreateMatrixData(mSig, mNrows, mNcols, mType);
          
   switch (mType){
   case eInt4:
       for(int row = 0 ; row <mNrows ; row++)
       {
	   for(int col = 0 ; col <mNcols ; col++)
	   {
	      mInter->Set(row, col, aMatrix.mInter->GetInt(row, col));
	   }
       }
	break;
   case eFloat4:
       for(int row = 0 ; row <mNrows ; row++)
       {
	   for(int col = 0 ; col <mNcols ; col++)
	   {
	       mInter->Set(row, col, aMatrix.mInter->GetFloat(row, col));
	   }
       }       
	break;	
   case eFloat8:
       for(int row = 0 ; row <mNrows ; row++)
       {
	   for(int col = 0 ; col <mNcols ; col++)
	   {
	       mInter->Set(row, col, aMatrix.mInter->GetDouble(row, col));
	   }
       }
	break;
    }		
}

void SDIFMatrix::CreateMatrixData(SdifSignature sig, int nrows, int ncols, SdifDataTypeET type)//=eFloat4)
{
    if(mInter) {
	delete mInter;
	mInter =0;
    }

    mSig = sig;
    /* to update the m_Signature : */
    m_Signature = SdifSignatureToString(mSig);
    mType = type;

    switch (mType){
    case eInt4:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<int>(nrows,ncols));
	break;
    case eFloat4:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<float>(nrows,ncols));
	break;	
    case eFloat8:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<double>(nrows,ncols));
	break;
    }
}

int SDIFMatrix::Write(SdifFileT* file)
{
    int SizeFrameW = 0;
    int nrows=mInter->GetNbRow();
    int ncols=mInter->GetNbCol();

    // SdifDataTypeET  type  = SdifFCurrDataType(file);
   
    mInter->Resize(nrows, ncols);
    SizeFrameW = 0;

    /* convert for the signature*/
    mSig = SdifStringToSignature(const_cast<char*>(m_Signature.c_str()));

    /* Set with the current matrix header */
    SdifFSetCurrMatrixHeader(file, mSig, mType, nrows, ncols);
    //SdifFSetCurrMatrixHeader(file, mSig, eFloat4, nrows, ncols);
    SizeFrameW += SdifFWriteMatrixHeader(file);
    SizeFrameW += mInter->write(file);

    return SizeFrameW;
}

/* when reading a matrix, this return the count of bytes and create a matrix which keep the values  */
int SDIFMatrix::Read(SdifFileT* file)
{
    int bytesread = 0;
    bytesread += SdifFReadMatrixHeader(file);
    /* for selection */
    if (!SdifFCurrMatrixIsSelected (file))
	  {
	      //bytesread += SdifFSkipMatrixData(file);
	      SdifFSkipMatrixData(file);

/*
	      bytesread += SdifFReadPadding(file,
	      SdifFPaddingCalculate
	      (file->Stream, bytesread));
*/
	      //return bytesread;
	      return 0;
	  }

    mSig = SdifFCurrMatrixSignature(file);
    m_Signature = SdifSignatureToString(mSig);
    int nrows = SdifFCurrNbRow(file);
    int ncols = SdifFCurrNbCol(file);
    mType  = SdifFCurrDataType (file);
	
    CreateMatrixData(mSig, nrows, ncols, mType);
    /* add bytesread */
    bytesread += mInter->read(file);	    
    return bytesread;
}

/* to see the matrix */
void SDIFMatrix::View()
{
   std::cout << " Signature : " << m_Signature << std::endl;
   mInter->view();
}

/* to set the value of matrix with type : "int", "float" or "double"*/
int SDIFMatrix::Set(int i, int j, const int& value)
{
    return mInter->Set(i, j, value);
}

int SDIFMatrix::Set(int i, int j, const float& value)
{
    return mInter->Set(i, j, value);
}

int SDIFMatrix::Set(int i, int j, const double& value)
{
    return mInter->Set(i, j, value);
}

/* to get */
/* a method to get value with type : "int", "float" or "double": */
int SDIFMatrix::Get(int i, int j, int& value)
{
    return value = mInter->GetInt(i, j);
}

float SDIFMatrix::Get(int i, int j, float& value)
{
    return value = mInter->GetFloat(i, j);
}

double SDIFMatrix::Get(int i, int j, double& value)
{
    return value = mInter->GetDouble(i, j);
}

/* an other method to get value : */
int SDIFMatrix::GetInt(int i, int j)
{
    return mInter->GetInt(i, j);
}

float SDIFMatrix::GetFloat(int i, int j)
{
    return mInter->GetFloat(i, j);
}

double SDIFMatrix::GetDouble(int i, int j)
{
    return mInter->GetDouble(i, j);
}

/* to get number of rows and columns*/
int SDIFMatrix::GetNbCols() const
{
    return mInter->GetNbCol();
}

int SDIFMatrix::GetNbRows() const
{
    return mInter->GetNbRow();
}

/* to get the size of matrix */
int SDIFMatrix::GetSize() const
{
    return SdifSizeOfMatrix(mType, mInter->GetNbCol(), mInter->GetNbRow());
}

/* to get the SdifDataType of the matrix */
SdifDataTypeET SDIFMatrix::GetType() const
{
    return mType;
}

/* to get the SdifSignature of the matrix */
SdifSignature SDIFMatrix::GetSignature() const
{
    return mSig;
}

/* to get the string signature of the matrix*/
std::string SDIFMatrix::GetStringSignature() const
{
    return m_Signature;
}
