/* -*-c++-*- */
/**
 * @file   sdifmatrixdata.h
 * @author Fabien Tisserand
 * @date   Mon Apr  8 17:34:25 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifmatrixdata.h,v 1.3 2002-07-12 10:27:34 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2002/06/18 14:47:45  ftissera
 * add comments
 * 
 *
 * Revision 1.1.1.1  2002/04/11 16:13:31  ftissera
 * Project for new SDIF API	
 * 
 * 
 */

#ifndef SDIFMATRIXDATA_H_
#define SDIFMATRIXDATA_H_ 1

#include <vector>
#include <sdif.h>
#include "sdifmatrixdatainterface.h"

template <class T>
class SDIFMatrixData : public SdifMatrixDataInterface
{

private:

    int m_Nrows;
    int m_Ncols;
    std::vector<T> m_Data;

public:
    SDIFMatrixData(int nrows, int ncols)
	{
	    if (nrows<0|| ncols<0 )
	    {
		m_Nrows=0;
		m_Ncols=0;
		m_Data.resize(0);
	    }
	    m_Nrows = nrows;
	    m_Ncols = ncols;
	    m_Data.resize(m_Nrows*m_Ncols);
	};

    ~SDIFMatrixData()
	{
	};
	

    template <class TT>
    TT Get(int i, int j)
	{
	    if (i > m_Nrows || j > m_Ncols)
	    {
		std::cerr<<"Error in Get : cannot have i>m_Nrows or j>m_Ncols"<<std::endl;
		return 1;
	    }
	   return  static_cast<TT>(m_Data[i*m_Ncols+j]);
	}

    double GetDouble(int i, int j)
	{
	    return Get<double>(i, j);   
	}

    float GetFloat(int i, int j)
	{
	    return Get<float>(i, j);   
	}

    int GetInt(int i, int j)
	{	    
	    return Get<int>(i, j);   
	}

    void Resize(int nrows, int ncols)
	{
	    m_Nrows = nrows;
	    m_Ncols = ncols;
	    m_Data.resize(m_Nrows*m_Ncols);
	}

    int read(SdifFileT* file)
	{
	    int bytesread = 0;
	    /*Read matrix data*/
	    for (int row = 0; row < m_Nrows; row++)
	    {
		bytesread += SdifFReadOneRow(file);
		T* p= static_cast<T*>(SdifFCurrOneRowData(file));
		
		std::copy(p,p+m_Ncols,&(m_Data[row*m_Ncols]));
	    }
	    /*Read matrix padding*/
	    bytesread += SdifFReadPadding(file, SdifFPaddingCalculate(file->Stream, bytesread));
	    return bytesread; 
	}

    int write(SdifFileT* file)
	{
	    int SizeFrameW = 0;
	    /* Write matrix data */
	    for (int row = 0; row < m_Nrows; row++)
	    {
		for (int col = 1; col <= m_Ncols; col++)
		{
		    T value = static_cast<T>(m_Data[row*m_Ncols+col-1]);
		    SdifFSetCurrOneRowCol (file, col, value);	   	 
		}
		SizeFrameW += SdifFWriteOneRow (file);
	    }

	    /* Write matrix padding */
	    SizeFrameW += SdifFWritePadding(file, SdifFPaddingCalculate (file->Stream, SizeFrameW));

	    return SizeFrameW;
	}

    void view()
	{
	    int row;
            int col;
	    for (row = 0; row < m_Nrows; row++)
	    {
	        for (col = 0; col < m_Ncols; col++)
		{		    
		    std::cout <<m_Data[row*m_Ncols+col] << " ";
		}
		std::cout << std::endl;
	    }

	}


   inline  int GetNbRow()
	{
	    return m_Nrows;
	}
    inline int GetNbCol() 
	{
	    return m_Ncols;
	}

    inline std::vector<T>& GetData()
	{
	    return m_Data;
	}

    inline int CopyData(std::vector<T>& data)
	{
	    data.resize(m_Data.size());
	    std::copy(m_Data.begin(), m_Data.end(), data.begin());
	}

    inline int Set(int i, int j, const int& value)
	{
	    m_Data[i*m_Ncols+j] = static_cast<T>(value);
	    return 1;
	}

    inline int Set(int i, int j, const float& value)
	{
	    m_Data[i*m_Ncols+j] = static_cast<T>(value);
	    return 1;
	}

    inline int Set(int i, int j, const double& value)
	{
	    m_Data[i*m_Ncols+j] = static_cast<T>(value);
	    return 1;
	}

};


#endif
