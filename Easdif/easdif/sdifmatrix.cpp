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
 * @file   sdifmatrix.cpp
 * @author Fabien Tisserand
 * @date   Thu Apr 11 18:36:39 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrix.cpp,v 1.11 2003-07-17 18:09:35 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.10  2003/07/09 21:06:55  roebel
 * Added support for eUInt4.
 *
 * Revision 1.9  2003/07/07 10:29:46  roebel
 * Added support for eInt1 and eUInt1 data types, resize of matrix now reinitializes all elements to 0
 *
 * Revision 1.8  2003/05/22 17:58:12  roebel
 * Improved checks in GetColName
 *
 * Revision 1.7  2003/05/19 14:00:20  roebel
 * Include new easdif_config.h.
 *
 * Revision 1.6  2003/05/18 20:46:46  roebel
 * Added method to read column names of matrix. For this the current sdiffile is stored in the matrix.
 * Improved documentation.
 * Added support for sdif data types eChar and eInt2.
 *
 * Revision 1.5  2003/05/01 19:01:39  roebel
 * Renamed CreateMatrixData to Init.
 * Reorganized Init functions to use only a single argument to specify the matrix signature.
 * Removed redundant m_Signature from class.
 * Added Resize method.
 *
 * Revision 1.4  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.3  2003/04/29 15:18:04  schwarz
 * CreateMatrixData accepts SdifSignature or std::string,
 * throws exception when unimplemented data type requested.
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.8  2003/02/10 14:14:49  roebel
 * Comment changed.
 *
 * Revision 1.7  2002/11/07 21:06:52  roebel
 * Allow value type to be specified during Matrix construction.
 * Do not resize before writing because size will be already correct.
 * Replaced Copy Constructor to use clone method.
 * Moved Get/Set methods to header to have the possibility to use
 * real templates.
 *
 * Revision 1.6  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.5  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
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


#include <iostream>
#include "easdif/easdif_config.h"
#include "easdif/sdifmatrix.h"

namespace Easdif {

SDIFMatrix::SDIFMatrix(const SdifDataTypeET _type):
  mInter(0),mType(_type)
{

    /* default signature */
    //mSig = SdifSignatureConst('1','T','R','C');
    // signifies uninitialized matrix
    mSig = 0;
    mFile = 0;
    Init(mSig, 1, 1, mType);
}

SDIFMatrix::SDIFMatrix(const SDIFMatrix& aMatrix):mInter(0)
{
    // Check     
    mSig = aMatrix.mSig;
    mType = aMatrix.mType;
    bytesread = aMatrix.bytesread;
    mFile   = aMatrix.mFile;

    mInter =   aMatrix.mInter->clone(); 
}


SDIFMatrix & SDIFMatrix::operator=(const SDIFMatrix& aMatrix) {
    
  delete mInter;
  
  mSig = aMatrix.mSig;
  mType = aMatrix.mType;
  bytesread = aMatrix.bytesread;
  mFile   = aMatrix.mFile;
  mInter =   aMatrix.mInter->clone(); 

  return *this;
}


void SDIFMatrix::Init(SdifSignature sig, int nrows, int ncols, SdifDataTypeET type)
{
    if(mInter) {
      if(mType == type) {
	mInter->Resize(nrows,ncols);
      }
      else{
	delete mInter;
	mInter =0;
      }
    }

    if (!mInter) {
      switch (type){
      case eInt1:
      case eChar:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<char>(nrows,ncols));
	break;
      case eUInt1:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<unsigned char>(nrows,ncols));
	break;
      case eUInt2:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<unsigned short int>(nrows,ncols));
	break;
      case eInt2:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<short int>(nrows,ncols));
	break;
      case eInt4:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<int>(nrows,ncols));
	break;
      case eUInt4:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<unsigned int>(nrows,ncols));
	break;
      case eFloat4:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<float>(nrows,ncols));
	break;	
      case eFloat8:
	mInter=static_cast<SDIFMatrixDataInterface*>(new SDIFMatrixData<double>(nrows,ncols));
	break;
	
      default:
	std::cerr  << std::endl << "!!! matrix type " << type << " not yet implemented !!!" << std::endl;
	SDIFMatrixDataError exc;
	exc.initException(eError,
			  "Error in  SDIFMatrix::Init!!! unimplemented matrix type used !!!",
			  0,0,0,0);      
	
	throw exc; // to be implemented
      }
      mType = type;
    }
    mSig  = sig;
}


void SDIFMatrix::Init(const std::string &sig, int nrows, int ncols, SdifDataTypeET type)
{
    Init(SdifStringToSignature(sig.c_str()), nrows, ncols, type);
}



bool SDIFMatrix::Resize(int _rows,int _ncols) {
  if(mInter){
    mInter->Resize(_rows,_ncols);
    return true;
  }
  return false;
}

bool SDIFMatrix::Clear() {
  if(mInter){
    mInter->Clear();
    return true;
  }
  return false;
}

int SDIFMatrix::Write(SdifFileT* file)
{
    int SizeFrameW = 0;
    int nrows=mInter->GetNbRow();
    int ncols=mInter->GetNbCol();

    // SdifDataTypeET  type  = SdifFCurrDataType(file);
   
    // we do not ned to resize
    //    mInter->Resize(nrows, ncols);
    SizeFrameW = 0;

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
  // remember file that we read from
  mFile = file;

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
    int nrows = SdifFCurrNbRow(file);
    int ncols = SdifFCurrNbCol(file);
    mType  = SdifFCurrDataType (file);
	
    Init(mSig, nrows, ncols, mType);
    /* add bytesread */
    bytesread += mInter->read(file);	    
    return bytesread;
}

/* to see the matrix */
void SDIFMatrix::Print()
{
   std::cout << " Signature : " << SdifSignatureToString(mSig) << std::endl;
   mInter->print();
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

/* to get the size of matrix in sdif file including padding */
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
    return std::string(SdifSignatureToString(mSig));
}

/* Get name of column of current matrix */
std::string SDIFMatrix::GetColName(int col) const {

  if(mFile && mFile->MatrixTypesTable) {
   SdifMatrixTypeT *tt= SdifGetMatrixType(mFile->MatrixTypesTable, 
					 mSig);
   if(tt) {
     SdifColumnDefT*tt2 =  SdifMatrixTypeGetNthColumnDef (tt, col);
     if(tt2 && tt2->Name) {
       return std::string(tt2->Name);
     }
   }
  }
  return std::string("");
}

} // end of namespace Easdif
