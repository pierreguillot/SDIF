/**
 * @file   sdifframe.cpp
 * @author Fabien Tisserand
 * @date   Thu Apr 11 18:44:52 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifframe.cpp,v 1.3 2002-06-18 14:50:19 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2002/05/17 20:20:56  ftissera
 * Add doxygen header
 * 
 * 
 */

#include "sdifframe.h"
#include "sdifentity.h"

/* for the frame selection */
int SDIFFrame::Selected()
{
   return  mSelected = 1;
}

/* for reading */
void SDIFFrame::Read(SdifFileT* file)
{
    ReadInfo(file);   
    Resize(file);
    ReadData(file);    
}

/* for reading with SDIFEntity */
void SDIFFrame::Read(const SDIFEntity& entity)
{
    SdifFileT* file = entity.GetFile();
    ReadInfo(file);   
    Resize(file);
    ReadData(file);    
}

/* reading the data */
void SDIFFrame::ReadData(SdifFileT* file)
{
    for (mIndex = 0; mIndex < mNbMatrix; mIndex++)
	mFrameBytesRead += mv_Matrix[mIndex].Read(file);
}

/* reading the data with SDIFEntity*/
void SDIFFrame::ReadData(const SDIFEntity& entity)
{
    SdifFileT* file = entity.GetFile();
    for (mIndex = 0; mIndex < mNbMatrix; mIndex++)
	mFrameBytesRead += mv_Matrix[mIndex].Read(file);
}

/* reading the informations */
void SDIFFrame::ReadInfo(SdifFileT* file)
{
    mFrameBytesRead = 0;
    mFrameBytesRead += SdifFReadFrameHeader(file);
    
    mTime    = SdifFCurrTime(file);
    mSig      = SdifFCurrFrameSignature(file);
    mStreamID = SdifFCurrID(file);
    mNbMatrix  = SdifFCurrNbMatrix(file);
}


/* reading the informations with SDIFEntity */
void SDIFFrame::ReadInfo(const SDIFEntity& entity)
{
    SdifFileT* file = entity.GetFile();
    mFrameBytesRead = 0;
    mFrameBytesRead += SdifFReadFrameHeader(file);
    
    mTime    = SdifFCurrTime(file);
    mSig      = SdifFCurrFrameSignature(file);
    mStreamID = SdifFCurrID(file);
    mNbMatrix  = SdifFCurrNbMatrix(file);
}


/* for writing */
void SDIFFrame::Write(SdifFileT* file)
{       
    WriteInfo(file);
    for (mIndex = 0; mIndex < mNbMatrix; mIndex++)	    
	mv_Matrix[mIndex].Write(file);      
}

/* for writing with SDIFEntity */
void SDIFFrame::Write(const SDIFEntity& entity)
{       
    SdifFileT* file = entity.GetFile();
    WriteInfo(file);
    for (mIndex = 0; mIndex < mNbMatrix; mIndex++)	    
	mv_Matrix[mIndex].Write(file);  
    // entity.SetFile(file);
}

/* writing informations */
void SDIFFrame::WriteInfo(SdifFileT* file)
{
    mSize += SdifSizeOfFrameHeader();
    SdifFSetCurrFrameHeader(file,mSig, mSize,mNbMatrix,mStreamID,mTime);	           SdifFWriteFrameHeader(file);    
}

/* writing informations with SDIFEntity*/
void SDIFFrame::WriteInfo(const SDIFEntity& entity)
{
    WriteInfo(entity.GetFile());
}

/* to see */
void SDIFFrame::ViewInfo()
{
    std::cout << "\nFrame signature : " << SdifSignatureToString(mSig)
	      << std::endl;
    std::cout << "StreamID: " << mStreamID;
    std::cout << " Time=" << mTime;
    std::cout << " number of Matrix in current Frame : " << mNbMatrix << std::endl; 
}

void SDIFFrame::View()
{
    ViewInfo();
    for (mIndex = 0; mIndex < mNbMatrix; mIndex++)
	mv_Matrix[mIndex].View();		  
}

/* to Set */
void SDIFFrame::SetInfo(SdifSignature sig, SdifUInt4 streamID, float time)//, SdifUInt4 nbMatrix)
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

void SDIFFrame::SetStringSignature(const std::string& signature)
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

SdifUInt4 SDIFFrame::GetNbMatrix()
{
    return mNbMatrix;
}

SdifSignature SDIFFrame::GetSignature()
{
    return mSig;
}

SdifUInt4 SDIFFrame::GetStreamID()
{
    return mStreamID;
}

SdifFloat8 SDIFFrame::GetTime()
{
    return mTime;
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
void SDIFFrame::AddMatrix(const SDIFMatrix& aMatrix)
{
    mv_Matrix.insert(mv_Matrix.end(), aMatrix);
    mSize += aMatrix.GetSize();
    mNbMatrix++;
}

/* resize */
void SDIFFrame::Resize(SdifFileT* file)
{
    mv_Matrix.resize(mNbMatrix);    
}

/* resize with SDIFEntity */
void SDIFFrame::Resize(const SDIFEntity& entity)
{
    Resize(entity.GetFile());   
}













