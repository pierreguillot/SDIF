/*
 * This file is part of the IRCAM EASDIF Library (http://www.ircam.fr/sdif)
 *
 * Copyright (C) 2002-2003 by IRCAM-Centre Georges Pompidou, Paris, France.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * See file COPYING for further informations on licensing terms.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *
 * The EASDIF library provides an Easy Api for IRCAMs SDIF library. 
 *
 */
/**
 * @file   sdifnamevaluetable.cpp
 * @author Fabien Tisserand
 * @date   Tue Jun 18 19:41:01 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifnamevaluetable.cpp,v 1.2 2003-04-06 16:31:08 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.5  2002/11/28 21:16:56  roebel
 * Back to simple map which sorts alphabetically. In any case
 * in SDIF the name value tables are mixed arbitrarily.
 *
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
 * Revision 1.1  2002/06/18 17:56:40  ftissera
 * Project for new SDIF API
 * 
 * 
 */

#include <iostream>
#include "easdif/sdifnamevaluetable.h"

namespace Easdif {

int SDIFNameValueTable::AddNameValue(const std::string& name,
				     const std::string& value) 
{
    map_NameValues[name] = value;
    return GetNbNameValue();
}

int SDIFNameValueTable::GetNbNameValue()
{
    return map_NameValues.size();
}

SdifUInt4 SDIFNameValueTable::GetStreamID()
{
    return mStreamID;
}

std::string SDIFNameValueTable::GetValue(const std::string& name) const
{
  const_iterator it;
  if((it= map_NameValues.find(name)) != map_NameValues.end())
    return it->second;
  else {
    return std::string("UNKNOWN NAME");
  }
}


SdifUInt4 SDIFNameValueTable::SetStreamID(const SdifUInt4& streamid)
{
    return mStreamID = streamid;
}


void SDIFNameValueTable::ViewNameValue(const std::string& name) const
{
  std::cout << name << "\t\t"
	    << GetValue(name) << std::endl;
}




void SDIFNameValueTable::ViewNameValueTable() const
{

    for (const_iterator p = begin(); 
	 p != end() ; ++p)
    {
      std::cout << GetNameFromSDIFNVTIt(p) << "\t\t" 
		<< GetValueFromSDIFNVTIt(p) << "\n";
    }
}


std::string GetNameFromSDIFNVTIt(SDIFNameValueTable::const_iterator& it) 
{
    return it->first;
}

std::string GetValueFromSDIFNVTIt(SDIFNameValueTable::const_iterator& it)
{
    return it->second;
}



} // end of namespace Easdif

