/**
 * @file   sdifnamevaluetable.cpp
 * @author Fabien Tisserand
 * @date   Tue Jun 18 19:41:01 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifnamevaluetable.cpp,v 1.2 2002-08-28 16:46:53 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.1  2002/06/18 17:56:40  ftissera
 * Project for new SDIF API
 * 
 * 
 */

#include "easdif/sdifnamevaluetable.h"


int SDIFNameValueTable::AddNameValue(std::string name,std::string value)
{
    map_NameValues[name] = value;
    return mNbNV ++;
}

int SDIFNameValueTable::GetNbNameValue()
{
    return mNbNV;
}

SdifUInt4 SDIFNameValueTable::GetStreamID()
{
    return mStreamID;
}

std::string SDIFNameValueTable::GetValue(std::string name)
{
    return map_NameValues[name];
}

int SDIFNameValueTable::SetNbNameValue(int nb)
{
    return mNbNV = nb;
}

SdifUInt4 SDIFNameValueTable::SetStreamID(SdifUInt4 streamid)
{
    return mStreamID = streamid;
}


void SDIFNameValueTable::ViewNameValue(std::string name)
{
    std::cout << name << "\t\t"
	      << map_NameValues[name] << std::endl;
}


void SDIFNameValueTable::ViewNameValueTable()
{
   typedef map<std::string, std::string>::const_iterator CI;

    for (CI p = map_NameValues.begin(); p != map_NameValues.end() ; ++p)
    {
	std::cout << p->first << "\t\t" 
		  << p->second << std::endl;
    }
}












