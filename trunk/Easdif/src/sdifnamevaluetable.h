/**
 * @file   sdifnamevaluetable.h
 * @author Fabien Tisserand
 * @date   Tue Jun 18 19:38:54 2002
 * 
 * @brief  
 *
 * 
 *
 * 
 * $Id: sdifnamevaluetable.h,v 1.1 2002-06-18 17:56:06 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $ 
 * 
 */

#ifndef SDIFNAMEVALUETABLE_H_
#define SDIFNAMEVALUETABLE_H_ 1

#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <sdif.h>

class SDIFNameValueTable
{
 private:

    SdifUInt4 mStreamID;

    /* temporarily not used */
    SdifUInt4	     FormatVersion;	/* version of the SDIF format itself */
    SdifUInt4	     TypesVersion;	/* version of the description 
					   type collection */

    // std::map<std::string,std::string> map_NameValues;

    int mNbNV;// number of NameValues in the NameValuesTable

 public:

    /* construct a NameValueTable*/
    SDIFNameValueTable():mNbNV(0)
	{};
    ~SDIFNameValueTable(){};

    /* to have a map of NameValues */
    std::map<std::string,std::string> map_NameValues;

    int AddNameValue(std::string, std::string);

    int GetNbNameValue();
    SdifUInt4 GetStreamID();
    std::string GetNameValue(std::string name);

    int SetNbNameValue(int nb);
    SdifUInt4 SetStreamID(SdifUInt4 streamid);

    void ViewNameValue(std::string name);
    void ViewNameValueTable();
};

#endif

