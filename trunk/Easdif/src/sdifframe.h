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
 * $Id: sdifframe.h,v 1.3 2002-07-12 10:19:03 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2002/06/18 14:51:13  ftissera
 * add methods with SDIFEntity for reading and writing
 *
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFFRAME_H_
#define SDIFFRAME_H_ 1

#include <string>
#include <sdif.h>
#include "sdifmatrix.h"

class SDIFEntity;

class SDIFFrame
{
private:
    //SDIFMatrix matrix;
    std::vector<SDIFMatrix> mv_Matrix;
/*for SDIFStream file's operations, it become public.
size_t  mBytesRead;
*/
    //int mSelected;// if is selected = 1 else = 0

    SdifFloat8      mTime;
    SdifSignature   mSig;
    SdifUInt4       mStreamID;
    SdifUInt4 mSize;// keep the size of the frame
    SdifUInt4 mNbMatrix;
    SdifUInt4 mSelected;
    //  SdifUInt4 mIndex;

public: 
    SDIFFrame(): mStreamID(0), mSize(0), mNbMatrix(0), mSelected(0)
	{};
    size_t  mFrameBytesRead;

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
    
    int Read(SdifFileT* file);
    int ReadData(SdifFileT* file);
    int ReadInfo(SdifFileT* file);
    int Write(SdifFileT* file);
    int WriteInfo(SdifFileT* file);
    void View();
    void ViewInfo();

    /* for SDIFEntity*/
    //  void Read(const SDIFEntity& entity);
    int Read(const SDIFEntity& entity);
    void ReadData(const SDIFEntity& entity);
    void ReadInfo(const SDIFEntity& entity);
    void Write(const SDIFEntity& entity);
    void WriteInfo(const SDIFEntity& entity);
    void Resize(const SDIFEntity& entity);


    void AddMatrix(const SDIFMatrix& aMatrix);    
    void ClearData();
    void Resize(SdifFileT* file);

    /* for the selection */
    int Select();
    int DeSelect();
    int IsSelected();

    /* to get a matrix */
    SDIFMatrix& GetMatrix(unsigned int index);
    /*ambiguity between signature and int  -> other name : GetMatrixwithSig()*/
    SDIFMatrix& GetMatrixWithSig(const SdifSignature& sig); 
    SDIFMatrix& GetMatrix(const std::string& signature);

    /* check if a matrix type exist in the frame  */
    bool MatrixExists(const SdifSignature& sig);
    bool MatrixExists(const std::string& signature);

    /* to get the informations of the frame */
    SdifUInt4 GetNbMatrix();
    SdifSignature GetSignature();
    SdifUInt4 GetStreamID();
    SdifFloat8 GetTime();
    SdifUInt4 GetSize();

    /* to set the informations of the frames  */
    void SetInfo(SdifSignature sig, SdifUInt4 streamID, float time);//, SdifUInt4 nbMatrix);
    void SetNbMatrix(SdifUInt4 nbMatrix);
    void SetSignature(SdifSignature sig);
    void SetStreamID(SdifUInt4 streamID);
    void SetStringSignature(const std::string& signature);
    void SetTime(float time);
};

#endif
