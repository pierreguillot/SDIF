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
 * @file   sdifframe.cpp
 * @author Fabien Tisserand
 * @date   Thu Apr 11 18:44:52 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifframe.cpp,v 1.5 2003-05-18 21:08:37 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
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
 * Revision 1.7  2003/02/10 14:14:35  roebel
 * Fixed problem with size indicated in frameheader when the frame is
 * written more than one time withut calling cleardata().
 *
 * Revision 1.6  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.5  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.4  2002/07/12 10:18:17  ftissera
 * Read has changed for selection
 *
 * Revision 1.3  2002/06/18 14:50:19  ftissera
 * add methods with SDIFEntity for reading and writing
 *
 * Revision 1.2  2002/05/17 20:20:56  ftissera
 * Add doxygen header
 * 
 * 
 */

#include "easdif/sdifframe.h"
#include "easdif/sdifentity.h"

namespace Easdif {

/* for reading */
int SDIFFrame::Read(SdifFileT* file, bool &eof)
{
    mFrameBytesRead = 0;

    mFrameBytesRead += ReadHeader(file);
    /* for selection */
    if (mFrameBytesRead == 0)
    {
	SdifFSkipFrameData (file);
	//to have exception
	eof = (SdifFGetSignature (file, &mFrameBytesRead) == eEof);
	return 0;
    }
    Resize();
    mFrameBytesRead += ReadData(file);

    /* to have exception */
    eof = (SdifFGetSignature (file, &mFrameBytesRead) == eEof);
    return mFrameBytesRead;    
}

/* for reading with SDIFEntity */
//void SDIFFrame::Read(const SDIFEntity& entity)
//int SDIFFrame::Read(const SDIFEntity& entity)
int SDIFFrame::Read(SDIFEntity& entity)
{
    mFrameBytesRead = 0;
    SdifFileT* file = entity.GetFile();

    if(entity.eof()) {
      // return -1;
      Easdif::SDIFEof exc;
      exc.initException(eError,"Error in SDIFFrame::Read -- Eof reached",
			file,0,0,0);      
      throw exc;
    }
    return Read(file, entity.mEof);

/*
    SdifFileT* file = entity.GetFile();
    ReadHeader(file);   
    Resize(file);
    ReadData(file);
*/
}

/* reading the data */
int SDIFFrame::ReadData(SdifFileT* file)
{
    int BytesRead = 0;
    SdifUInt4 index;
    for (index = 0; index < mNbMatrix; ){
	BytesRead += mv_Matrix[index].Read(file);
	if(mv_Matrix[index].GetSignature() == 0)
	    mNbMatrix --;
	else
	    index++;
    }
    Resize();
    return BytesRead;
}

/* reading the data with SDIFEntity*/
int SDIFFrame::ReadData(const SDIFEntity& entity)
{

    SdifFileT* file = entity.GetFile();
    return ReadData(file);
}

/* reading the informations */
int SDIFFrame::ReadHeader(SdifFileT* file)
{
    int BytesRead = 0;  
    //size_t BytesRead = 0;
    //int eof = 0;

    BytesRead += SdifFReadFrameHeader(file);

    /* for selection*/
       if (!SdifFCurrFrameIsSelected (file))
	  {
	      //SdifFSkipFrameData (file);
	      //eof = SdifFGetSignature (file, &bytesread) == eEof;
	 return 0;
	  }

    mTime    = SdifFCurrTime(file);
    mSig      = SdifFCurrFrameSignature(file);
    mStreamID = SdifFCurrID(file);
    mNbMatrix  = SdifFCurrNbMatrix(file);

    return BytesRead;
}


/* reading the informations with SDIFEntity */
int SDIFFrame::ReadHeader(const SDIFEntity& entity)
{
    SdifFileT* file = entity.GetFile();
    return ReadHeader(file);
}


/* for writing */
int SDIFFrame::Write(SdifFileT* file)
{        
    SdifUInt4 index;
    
    WriteHeader(file);
    SdifUInt4 _size    = 0;
    for (index = 0; index < mNbMatrix; index++)	    
	_size += mv_Matrix[index].Write(file);     

    if(_size != mSize){
      Easdif::SDIFFrameHeaderSizeError exc;
      exc.initException(eError,"Error in SDIFFrame::Write -- FrameSize in Header does not match size of matrices in header",file,0,0,0);      
      throw exc;
    }
    return mSize; 
}

/* for writing with SDIFEntity */
int SDIFFrame::Write(const SDIFEntity& entity)
{ 
    return Write(entity.GetFile());    
}

/* writing informations */
int SDIFFrame::WriteHeader(SdifFileT* file)
{
  int _frsize = SdifSizeOfFrameHeader();
  SdifFSetCurrFrameHeader(file, mSig, mSize+SdifSizeOfFrameHeader(),
			  mNbMatrix, mStreamID, mTime); 
  SdifFWriteFrameHeader(file);    
  
  return _frsize;
}

/* writing informations with SDIFEntity*/
int SDIFFrame::WriteHeader(const SDIFEntity& entity)
{
   return WriteHeader(entity.GetFile());
}

/* to see */
void SDIFFrame::PrintHeader()
{
    std::cout << "\nFrame signature : " << SdifSignatureToString(mSig)
	      << std::endl;
    std::cout << "StreamID: " << mStreamID;
    std::cout << " Time: " << mTime;
    std::cout << " number of Matrix in current Frame : " << mNbMatrix << std::endl; 
}

void SDIFFrame::Print()
{
    SdifUInt4 index;
    PrintHeader();
    for (index = 0; index < mNbMatrix; index++)
	mv_Matrix[index].Print();		  
}

/* to Set */
void SDIFFrame::SetHeader(SdifSignature sig, SdifUInt4 streamID, float time)//, SdifUInt4 nbMatrix)
{
    mSig = sig;
    mStreamID = streamID;
    mTime = time; //and  mNbMatrix = nbMatrix;
}

void SDIFFrame::SetNbMatrix(SdifUInt4 nbMatrix)
{
    mNbMatrix = nbMatrix;
}

void SDIFFrame::SetSignature(SdifSignature sig)
{
    mSig = sig;
}

void SDIFFrame::SetStreamID(SdifUInt4 streamID)
{
    mStreamID = streamID;
}

void SDIFFrame::SetSignature(const std::string& signature)
{
    mSig = SdifStringToSignature(const_cast<char*>(signature.c_str()));
}

void SDIFFrame::SetTime(float time)
{
    mTime = time;
}

/* to Get */
SDIFMatrix& SDIFFrame::GetMatrix(unsigned int index) 
{
    // Check index
    if (index > (mv_Matrix.size()-1))
    {
	std::cerr << " No such matrix " << std::endl;
	return mv_Matrix[0];
    }
    return mv_Matrix[index];
}

/* to Get */
const SDIFMatrix& SDIFFrame::GetMatrix(unsigned int index) const
{
    // Check index
    if (index > (mv_Matrix.size()-1))
    {
	std::cerr << " No such matrix " << std::endl;
	return mv_Matrix[0];
    }
    return mv_Matrix[index];
}


SdifUInt4 SDIFFrame::GetNbMatrix() const
{
    return mNbMatrix;
}

SdifSignature SDIFFrame::GetSignature() const
{
    return mSig;
}

SdifUInt4 SDIFFrame::GetStreamID() const
{
    return mStreamID;
}

SdifFloat8 SDIFFrame::GetTime() const
{
    return mTime;
}

SdifUInt4 SDIFFrame::GetSize() const
{
    return mSize;
}

/* clean */
void SDIFFrame::ClearData()
{
    mStreamID = 0;
    mSize = 0;
    mNbMatrix = 0;
    mv_Matrix.clear();
}

/* add a matrix */
int SDIFFrame::AddMatrix(const SDIFMatrix& aMatrix)
{
    mv_Matrix.insert(mv_Matrix.end(), aMatrix);
    mSize += aMatrix.GetSize();
    mNbMatrix++;
    return mNbMatrix;
}

int SDIFFrame::AddMatrixSelected(SdifFileT* file, const SDIFMatrix& aMatrix)
{
    SdifSignature sig, sigm;
    sig = GetMatrixSelection(file);
    sigm = aMatrix.GetSignature();
    if(sig != 0)
    {
	if(sig == sigm)
	{
	    return AddMatrix(aMatrix);
	}
	else
	    return 0;
    }
    else/* no selection on matrix : all matrix are selected*/
	return AddMatrix(aMatrix);
}

/* resize */
void SDIFFrame::Resize()
{
    mv_Matrix.resize(mNbMatrix);    
}



bool SDIFFrame::MatrixExists(const SdifSignature& sig) const
{ 
    bool test = false;
    SdifUInt4 index = 0;    

    while (!test && (index < mNbMatrix))
    {
	test = (mv_Matrix[index].GetSignature() == sig);
	++index;
    }
    return test;
}

bool SDIFFrame::MatrixExists(const std::string& signature) const
{
    SdifSignature sig = SdifStringToSignature(const_cast<char*>
					      (signature.c_str()));
    return MatrixExists(sig);
}


SDIFMatrix& SDIFFrame::GetMatrixWithSig(const SdifSignature sig)
{
    bool test = false;    
    SdifUInt4 index = 0;

    while (!test && (index < mNbMatrix))
    {
	test = (mv_Matrix[index].GetSignature() == sig);
	++index;
    }

    if (test)
	return mv_Matrix[index-1];
    else
    {
	std::cerr << " No such matrix " << std::endl;
	return mv_Matrix[0];
    }
}

const SDIFMatrix& SDIFFrame::GetMatrixWithSig(const SdifSignature sig) const
{
    bool test = false;    
    SdifUInt4 index = 0;

    while (!test && (index < mNbMatrix))
    {
	test = (mv_Matrix[index].GetSignature() == sig);
	++index;
    }

    if (test)
	return mv_Matrix[index-1];
    else
    {
	std::cerr << " No such matrix " << std::endl;
	return mv_Matrix[0];
    }
}



SDIFMatrix& SDIFFrame::GetMatrix(const std::string& signature)
{   
    /*
      bool test = false;    
      SdifUInt4 index = 0;
    */

    SdifSignature sig = SdifStringToSignature(const_cast<char*>
					      (signature.c_str()));
    /* with GetMatrixWithSig(sig) */
    return GetMatrixWithSig(sig);

}


const SDIFMatrix& SDIFFrame::GetMatrix(const std::string& signature) const
{   
    /*
      bool test = false;    
      SdifUInt4 index = 0;
    */

    SdifSignature sig = SdifStringToSignature(signature.c_str());
    /* with GetMatrixWithSig(sig) */
    return GetMatrixWithSig(sig);

}


SdifSignature SDIFFrame::GetMatrixSelection(SdifFileT* file) const
{
    SdifListT* listsel;
    SdifSignature sig = 0;
    listsel = file->Selection->matrix;
    SdifListInitLoop (listsel);
    while (SdifListIsNext (listsel))
    {
	sig = ((SdifSelectElementT *) SdifListGetNext 
	       (listsel))->value.signature;
    }
    return sig;
}

SdifSignature SDIFFrame::GetMatrixSelection(const SDIFEntity& entity) const
{
    return GetMatrixSelection(entity.GetFile());
}


} // end of namespace Easdif

