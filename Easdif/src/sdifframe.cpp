/**
 * @file   sdifframe.cpp
 * @author Fabien Tisserand
 * @date   Thu Apr 11 18:44:52 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifframe.cpp,v 1.4 2002-07-12 10:18:17 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.3  2002/06/18 14:50:19  ftissera
 * add methods with SDIFEntity for reading and writing
 *
 * Revision 1.2  2002/05/17 20:20:56  ftissera
 * Add doxygen header
 * 
 * 
 */

#include "sdifframe.h"
#include "sdifentity.h"

/* for the frame selection */
int SDIFFrame::Select()
{
   return  mSelected = 1;
}

int SDIFFrame::DeSelect()
{
    return mSelected = 0;
}

int SDIFFrame::IsSelected()
{
    return mSelected;
}

/* for reading */
int SDIFFrame::Read(SdifFileT* file)
{
    mFrameBytesRead = 0;
    // int eof = 0;//

    mFrameBytesRead += ReadInfo(file);
    /* for selection */
    if (mFrameBytesRead == 0)
    {
	SdifFSkipFrameData (file);
	//eof = SdifFGetSignature (file, &mFrameBytesRead) == eEof;
	SdifFGetSignature (file, &mFrameBytesRead);
	return 0;
    }

    Resize(file);
    mFrameBytesRead += ReadData(file);

    return mFrameBytesRead;    
}

/* for reading with SDIFEntity */
//void SDIFFrame::Read(const SDIFEntity& entity)
int SDIFFrame::Read(const SDIFEntity& entity)
{
    mFrameBytesRead = 0;
    SdifFileT* file = entity.GetFile();
    return Read(file);

/*
    SdifFileT* file = entity.GetFile();
    ReadInfo(file);   
    Resize(file);
    ReadData(file);
*/
}

/* reading the data */
int SDIFFrame::ReadData(SdifFileT* file)
{
    int BytesRead = 0;
    SdifUInt4 index;
    for (index = 0; index < mNbMatrix; index++)
	BytesRead += mv_Matrix[index].Read(file);

    return BytesRead;
}

/* reading the data with SDIFEntity*/
void SDIFFrame::ReadData(const SDIFEntity& entity)
{
    SdifUInt4 index;
    SdifFileT* file = entity.GetFile();
    for (index = 0; index < mNbMatrix; index++)
	mFrameBytesRead += mv_Matrix[index].Read(file);
}

/* reading the informations */
int SDIFFrame::ReadInfo(SdifFileT* file)
{
    int BytesRead = 0;  

    BytesRead += SdifFReadFrameHeader(file);

    /* for selection*/
       if (!SdifFCurrFrameIsSelected (file))
	  {
	//SdifFSkipFrameData (file);
	    //    eof = SdifFGetSignature (file, &bytesread) == eEof;
	 return 0;
	  }

    mTime    = SdifFCurrTime(file);
    mSig      = SdifFCurrFrameSignature(file);
    mStreamID = SdifFCurrID(file);
    mNbMatrix  = SdifFCurrNbMatrix(file);

    return BytesRead;
//    return 0;
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
int SDIFFrame::Write(SdifFileT* file)
{        
    SdifUInt4 index;
    WriteInfo(file);
    for (index = 0; index < mNbMatrix; index++)	    
	mSize += mv_Matrix[index].Write(file);     

    return mSize; 
}

/* for writing with SDIFEntity */
void SDIFFrame::Write(const SDIFEntity& entity)
{       
    SdifUInt4 index;
    SdifFileT* file = entity.GetFile();
    WriteInfo(file);
    for (index = 0; index < mNbMatrix; index++)	    
	mv_Matrix[index].Write(file);  
    // entity.SetFile(file);
}

/* writing informations */
int SDIFFrame::WriteInfo(SdifFileT* file)
{
    mSize += SdifSizeOfFrameHeader();
    SdifFSetCurrFrameHeader(file, mSig, mSize, mNbMatrix, mStreamID, mTime);	           SdifFWriteFrameHeader(file);    

    return mSize;
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
    SdifUInt4 index;
    ViewInfo();
    for (index = 0; index < mNbMatrix; index++)
	mv_Matrix[index].View();		  
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

SdifUInt4 SDIFFrame::GetSize()
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


bool SDIFFrame::MatrixExists(const SdifSignature& sig)
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

bool SDIFFrame::MatrixExists(const std::string& signature)
{
    SdifSignature sig = SdifStringToSignature(const_cast<char*>
					      (signature.c_str()));
    return MatrixExists(sig);
}


SDIFMatrix& SDIFFrame::GetMatrixWithSig(const SdifSignature& sig)
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

/*
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
*/
}









