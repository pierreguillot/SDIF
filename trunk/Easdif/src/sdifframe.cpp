/**
 * @file   sdifframe.cpp
 * @author Fabien Tisserand
 * @date   Thu Apr 11 18:44:52 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifframe.cpp,v 1.6 2002-10-10 10:49:09 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
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
int SDIFFrame::Read(SdifFileT* file,bool &eof)
{
    mFrameBytesRead = 0;

    mFrameBytesRead += ReadInfo(file);
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
    return Read(file,entity.eof());

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
int SDIFFrame::ReadInfo(SdifFileT* file)
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
int SDIFFrame::ReadInfo(const SDIFEntity& entity)
{
    SdifFileT* file = entity.GetFile();
    return ReadInfo(file);
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
int SDIFFrame::Write(const SDIFEntity& entity)
{ 
    return Write(entity.GetFile());    
}

/* writing informations */
int SDIFFrame::WriteInfo(SdifFileT* file)
{
    mSize += SdifSizeOfFrameHeader();
    SdifFSetCurrFrameHeader(file, mSig, mSize, mNbMatrix, mStreamID, mTime);	           SdifFWriteFrameHeader(file);    

    return mSize;
}

/* writing informations with SDIFEntity*/
int SDIFFrame::WriteInfo(const SDIFEntity& entity)
{
   return WriteInfo(entity.GetFile());
}

/* to see */
void SDIFFrame::ViewInfo()
{
    std::cout << "\nFrame signature : " << SdifSignatureToString(mSig)
	      << std::endl;
    std::cout << "StreamID: " << mStreamID;
    std::cout << " Time: " << mTime;
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
SDIFMatrix& SDIFFrame::GetMatrixIfSelected(SdifFileT* file, unsigned int index)
{
    SdifSignature sig, sigm;
    sig = GetMatrixSelection(file);
    sigm = mv_Matrix[index].GetSignature();
    if( (sig != 0 && sig == sigm) || sig == 0 )
    {	
	return GetMatrix(index);
    }
    else
    {
	std::cerr << " No such matrix selected " << std::endl;	   
	return mv_Matrix[0];
    }

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


SdifSignature SDIFFrame::GetMatrixSelection(SdifFileT* file)
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

SdifSignature SDIFFrame::GetMatrixSelection(const SDIFEntity& entity)
{
    return GetMatrixSelection(entity.GetFile());
}


} // end of namespace Easdif

