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
 * $Id: sdifnamevaluetable.h,v 1.5 2002-11-28 21:16:56 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.4  2002/11/27 20:10:39  roebel
 * Fixed NVT to keep ordering as defined by the sequence of additions to the table.
 * Added begin/end iterator.
 * Added global functions to get name and value from SDIFNameValueTable iterators.
 * Removed member that redundantly indicated size of map.
 *
 * Revision 1.3  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
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
  
  /** 
   * @brief map of string which are containing the Name Values
   *
   * we use our own less then comparator such that the NVT
   * is always scanned in the same order that has been
   * used to  add the entries.
   */
  std::map<std::string,std::string> map_NameValues;


public:

    /* construct a NameValueTable*/
    SDIFNameValueTable() {};

    ~SDIFNameValueTable(){};

  /** 
   *  \brief add a Name Value in the map
   * 
   * @param name 
   * @param value 
   * 
   * @return  the number of Name Values in the map
   */
  int AddNameValue(const std::string& name, const std::string& value);

  /** 
   * \brief clear  all Name Values from the  table 
   */
  void clear() { map_NameValues.clear();};


  /*************************************************************************/
  /*
    // FUNCTION GROUP:	set the members
  */
  
  /**  
   * @brief set the streamID
   * 
   * @param streamid 
   * 
   * @return 
   */
  SdifUInt4 SetStreamID(const SdifUInt4& streamid);

  /*************************************************************************/
  /*
    // FUNCTION GROUP:	to see
  */
  /** 
   * @brief view a Name Value
   * 
   * 
   * @param name 
   */
  void ViewNameValue(const std::string& name)const;

  /** 
   * @brief view a Name Value Table
   */
  void ViewNameValueTable() const;


  /*************************************************************************/
  /*
    // FUNCTION GROUP:   Iterator
  */

  
  typedef std::map<std::string,std::string>::iterator iterator;
  typedef std::map<std::string,std::string>::const_iterator const_iterator;


  /** 
   * /brief iterator over name value table
   * 
   * 
   * @return iterator pointing to first name value entry
   */
  iterator  begin() { return map_NameValues.begin();  }
  const_iterator  begin() const { return map_NameValues.begin();  }

  /** 
   * /brief iterator over name value table
   * 
   * 
   * @return iterator pointing to end of name value table
   */
  iterator  end() { return map_NameValues.end();  }
  const_iterator  end() const { return map_NameValues.end();  }

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
  std::string GetValue(const std::string& name) const;


};


  /** 
   * @brief get the Value
   * @param nvt iterator
   * @return value string
   */
  std::string GetValueFromSDIFNVTIt(SDIFNameValueTable::const_iterator& it);


  /** 
   * @brief get the name
   * @param nvt iterator
   * @return name string
   */
  std::string GetNameFromSDIFNVTIt(SDIFNameValueTable::const_iterator& it);



} // end of namespace Easdif

#endif

