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
 * $Id: sdifmatrixdatainterface.h,v 1.5 2002-11-07 21:04:28 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.4  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.3  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.2  2002/07/12 10:29:54  ftissera
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFMATRIXDATAINTERFACE_H_
#define SDIFMATRIXDATAINTERFACE_H_ 1

#include <sdif.h>

namespace Easdif {

//class SdifMatrixDataInterface

/** 
 * abstract class which permits the storage in natural type in SDIFMatrixData :
 * you can get the value in different type
 * @see SDIFMatrixData
 */
class SDIFMatrixDataInterface
{ 
public:
//  virtual <class T> Get(int i, int j)=0;
    virtual double GetDouble(int i, int j)=0;
    virtual float  GetFloat(int i, int j)=0;
    virtual int    GetInt(int i, int j)=0;
    virtual int    GetNbRow()=0;
    virtual int    GetNbCol()=0;
    virtual void   view()=0;
    virtual int    read(SdifFileT* file)=0;
    virtual void   Resize(int nrows, int ncols)=0;
    virtual int    write(SdifFileT* file)=0;

    virtual void   Set(int row, int col, const int& value)=0;
    virtual void   Set(int row, int col, const float& value)=0;
    virtual void   Set(int row, int col, const double& value)=0;
    
  virtual SDIFMatrixDataInterface * clone()=0;

    virtual ~SDIFMatrixDataInterface() {};
};

} // end of namespace Easdif

#endif
