#include "sdifframe.h"

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

void SDIFFrame::ReadData(SdifFileT* file)
{
    for (int m = 0; m < mNbMatrix; m++)
	mBytesRead += mv_Matrix[m].Read(file);
}

void SDIFFrame::ReadInfo(SdifFileT* file)
{
    mBytesRead = 0;
    mBytesRead += SdifFReadFrameHeader(file);
    
    mTime    = SdifFCurrTime(file);
    mSig      = SdifFCurrFrameSignature(file);
    mStreamID = SdifFCurrID(file);
    mNbMatrix  = SdifFCurrNbMatrix(file);
}

/* for writing */
void SDIFFrame::Write(SdifFileT* file)
{       
    WriteInfo(file);
    for (int m = 0; m < mNbMatrix; m++)	    
	mv_Matrix[m].Write(file);      
}

void SDIFFrame::WriteInfo(SdifFileT* file)
{
    mSize += SdifSizeOfFrameHeader();
    SdifFSetCurrFrameHeader(file,mSig, mSize,mNbMatrix,mStreamID,mTime);	           SdifFWriteFrameHeader(file);    
}

/* to see */
void SDIFFrame::ViewInfo()
{
    std::cout << "\n Signature de la frame : " << SdifSignatureToString(mSig)
	      << std::endl;
    std::cout << "sID: " << mStreamID;
    std::cout << " Tps=" << mTime;
    std::cout << " nbreMat ds Frame : " << mNbMatrix << std::endl; 
}

void SDIFFrame::View()
{
    ViewInfo();
    for (int m = 0; m < mNbMatrix; m++)
	mv_Matrix[m].View();		  
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















