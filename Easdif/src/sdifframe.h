/* -*-c++-*- */
/**
 * @file   sdifframe.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:29:25 2002
 * 
 * @brief  
 * 
 * sdifframe.h is composed of the different methods which are using to 
 * manipulate the frame.
 * 
 * $Id: sdifframe.h,v 1.1.1.1 2002-04-11 16:13:31 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $ 
 * 
 */

#ifndef SDIFFRAME_H_
#define SDIFFRAME_H_ 1

#include <string>
#include <sdif.h>
#include "sdifmatrix.h"

class SDIFFrame
{
private:
    //SDIFMatrix matrix;
    std::vector<SDIFMatrix> mv_Matrix;
    size_t  mBytesRead;
    
    int mSelected;// if is selected = 1 else = 0

    SdifFloat8      mTime;
    SdifSignature   mSig;
    SdifUInt4       mStreamID;
    SdifUInt4 mSize;
    SdifUInt4 mNbMatrix;

public: 
    SDIFFrame(): mStreamID(0), mSize(0), mNbMatrix(0)
	{};

/** 
 * Read : permit to read completly a frame : the header and the data
 * ReadInfo : permit to read the frame header (used by Read)
 * ReadData : permit to read only the data of a frame (used by Read)
 * Write : permit to write a frame
 * WriteInfo : permit to write the frame heder and the data (used by Write)
 * View : permit to see the content of a frame
 * ViewInfo : permit to see the frame header
 * Resize : permit to resize the vector of the frame that stock the reading
 * ClearData : permit to clear the data of a frame to reused
 *
 * @param file 
 */
    
    void Read(SdifFileT* file);
    void ReadData(SdifFileT* file);
    void ReadInfo(SdifFileT* file);
    void Write(SdifFileT* file);
    void WriteInfo(SdifFileT* file);
    void View();
    void ViewInfo();

    void AddMatrix(const SDIFMatrix& aMatrix);    
    void ClearData();
    void Resize(SdifFileT* file);
    int Selected();// return 1 if selected

    SDIFMatrix& GetMatrix(unsigned int index);
    SdifUInt4 GetNbMatrix();
    SdifSignature GetSignature();
    SdifUInt4 GetStreamID();
    SdifFloat8 GetTime();

    void SetInfo(SdifSignature sig, SdifUInt4 streamID, float time);//, SdifUInt4 nbMatrix);
    void SetNbMatrix(SdifUInt4 nbMatrix);
    void SetSignature(SdifSignature sig);
    void SetStreamID(SdifUInt4 streamID);
    void SetStringSignature(const std::string& signature);
    void SetTime(float time);
};

#endif
