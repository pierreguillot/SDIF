/* -*-c++-*- */
/**
 * @file   sdifmatrixdatainterface.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:46:08 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrixdatainterface.h,v 1.2 2002-07-12 10:29:54 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFMATRIXDATAINTERFACE_H_
#define SDIFMATRIXDATAINTERFACE_H_ 1

#include <sdif.h>
//#include "sdifmatrixdata.h"

class SdifMatrixDataInterface
{ 
public:
//  virtual <class T> Get(int i, int j)=0;
    virtual double GetDouble(int i, int j)=0;
    virtual float GetFloat(int i, int j)=0;
    virtual int GetInt(int i, int j)=0;
    virtual int GetNbRow()=0;
    virtual int GetNbCol()=0;
    virtual void view()=0;
    virtual int read(SdifFileT* file)=0;
    virtual void Resize(int nrows, int ncols)=0;
    virtual int write(SdifFileT* file)=0;

    virtual int Set(int row, int col, const int& value)=0;
    virtual int Set(int row, int col, const float& value)=0;
    virtual int Set(int row, int col, const double& value)=0;

    virtual ~SdifMatrixDataInterface() {};
};

#endif
