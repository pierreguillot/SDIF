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
 * $Id: sdifnamevaluetable.h,v 1.3 2002-10-10 10:49:09 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.1  2002/06/18 17:56:06  ftissera
 * Project for new SDIF API
 * 
 * 
 */

#ifndef SDIFNAMEVALUETABLE_H_
#define SDIFNAMEVALUETABLE_H_ 1

#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <sdif.h>


namespace Easdif {

/** 
 * @brief class which can be associated with a Name Value Table
 *
 * SDIFNameValueTable is composed of different methods which permits to
 * manipulate a Name Value Table.
 */
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

/** 
 * @brief map of string which are containing the Name Values
 */
    std::map<std::string,std::string> map_NameValues;



/** 
 * add a Name Value in the map
 * @return the number of Name Values in the map
 */
    int AddNameValue(std::string name, std::string value);


/*************************************************************************/
/*
// FUNCTION GROUP:	get the members
*/
/** 
 * @brief get the number of Name Value
 */
    int GetNbNameValue();

/** 
 * @brief get the StreamID
 */
    SdifUInt4 GetStreamID();

/** 
 * @brief get the Value
 * @param name string
 * @return value string
 */
    std::string GetValue(std::string name);


/*************************************************************************/
/*
// FUNCTION GROUP:	set the members
*/
/** 
 * @brief set the number of name value
 */
    int SetNbNameValue(int nb);

/**  
 * @brief set the streamID
 */
    SdifUInt4 SetStreamID(SdifUInt4 streamid);

/*************************************************************************/
/*
// FUNCTION GROUP:	to see
*/
/** 
 * @brief view a Name Value
 */
    void ViewNameValue(std::string name);

/** 
 * @brief view a Name Value Table
 */
    void ViewNameValueTable();
};


} // end of namespace Easdif

#endif

