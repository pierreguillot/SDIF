/* -*-c++-*- */
/**
 * @file   sdifmatrix.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:45:33 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrix.h,v 1.2 2002-06-18 14:48:30 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFMATRIX_H_
#define SDIFMATRIX_H_ 1



#include <string>
#include <sdif.h>
#include "sdifmatrixdatainterface.h"
#include "sdifmatrixdata.h"


class SDIFMatrix
{
private:
    SdifMatrixDataInterface* mInter;

    int bytesread;
    std::string m_Signature;
    SdifSignature mSig;
    SdifDataTypeET mType;

public:
    SDIFMatrix();
    SDIFMatrix(const SDIFMatrix& aMatrix);

    ~SDIFMatrix(){
	if(mInter) {
	    delete mInter;
	    mInter =0;
	}
    };

    void CreateMatrixData(SdifSignature sig, 
			  int nrows, int ncols, SdifDataTypeET  type=eFloat4);
    int Write(SdifFileT* file);
    int Read(SdifFileT* file);
    void View();

    int GetSize() const;
    int GetNbRows();
    int GetNbCols();

    /* adding for selection */
    SdifSignature GetSignature();
    std::string GetStringSignature();
    SdifDataTypeET GetType();

    int GetInt(int i, int j);
    float GetFloat(int i, int j);
    double GetDouble(int i, int j);

    int Get(int i, int j, int& value);
    float Get(int i, int j, float& value);
    double Get(int i, int j, double& value);

    int Set(int i, int j, const int& value);
    int Set(int i, int j, const float& value);
    int Set(int i, int j, const double& value);
};

#endif
