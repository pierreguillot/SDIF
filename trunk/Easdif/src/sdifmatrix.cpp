/**
 * @file   sdifmatrix.cpp
 * @author Fabien Tisserand
 * @date   Thu Apr 11 18:36:39 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrix.cpp,v 1.2 2002-04-11 16:41:55 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $ 
 * 
 */

#include "sdifmatrix.h"
SDIFMatrix::SDIFMatrix():mInter(0)
{
    //m_Signature =...;
    mType = eFloat4;
    /*default signature*/
    mSig = SdifSignatureConst('1','T','R','C');
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

void SDIFMatrix::CreateMatrixData(SdifSignature sig, int nrows, int ncols, SdifDataTypeET type=eFloat4)
{
    if(mInter) {
	delete mInter;
	mInter =0;
    }

    mSig = sig;
    //update m_Signature;
    mType = type;

    switch (mType){
    case eInt4:
	mInter=static_cast<SdifMatrixDataInterface*>(new SDIFMatrixData<int>(nrows,ncols));
	break;
    case eFloat4:
	mInter=static_cast<SdifMatrixDataInterface*>(new SDIFMatrixData<float>(nrows,ncols));
	break;	
    case eFloat8:
	mInter=static_cast<SdifMatrixDataInterface*>(new SDIFMatrixData<double>(nrows,ncols));
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

    /*Write matrix header*/
    mSig = SdifStringToSignature(const_cast<char*>(m_Signature.c_str()));
/*
SdifFSetCurrMatrixHeader(file, mSig, mType, nrows, ncols);
*/
    SdifFSetCurrMatrixHeader(file, mSig, eFloat4, nrows, ncols);
    SizeFrameW += SdifFWriteMatrixHeader(file);
    SizeFrameW +=mInter->write(file);

    return SizeFrameW;
}

/* when reading a matrix, this return the count of bytes and create a matrix which keep the values  */
int SDIFMatrix::Read(SdifFileT* file)
{
    int bytesread = 0;
    bytesread += SdifFReadMatrixHeader(file);

    mSig = SdifFCurrMatrixSignature(file);
    m_Signature = SdifSignatureToString(mSig);
    int nrows = SdifFCurrNbRow(file);
    int ncols = SdifFCurrNbCol(file);
    mType  = SdifFCurrDataType (file);
	
    CreateMatrixData(mSig, nrows, ncols, mType);
    mInter->read(file);	    
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
int SDIFMatrix::GetNbCols()
{
    return mInter->GetNbCol();
}

int SDIFMatrix::GetNbRows()
{
    return mInter->GetNbRow();
}

/* to get the size of matrix */
int SDIFMatrix::GetSize() const
{
    return SdifSizeOfMatrix(mType, mInter->GetNbCol(), mInter->GetNbRow());
}



