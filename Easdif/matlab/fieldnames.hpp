/**
 * @file   fieldnames.hpp
 * @author Axel Roebel
 * @date   Mon Jan 21 23:43:44 2008
 * 
 * @brief  field names for matlab structs
 * 
 * 
 */

#ifndef FIELDNAMES_HPP
#define FIELDNAMES_HPP

namespace {
  // frame description struct
  const char *frameFrameSig_fieldString  = "fsig";
  const char *frameStreamID_fieldString  = "stream";
  const char *frameTime_fieldString      = "time";
  const char *frameMatrixSig_fieldString = "msig";
  const char *frameData_fieldString      = "data";
  
  // header description struct
  const char *headNVT_fieldString =  "NVT";
  const char *headTYP_fieldString =  "TYP";
  const char *headIDS_fieldString =  "IDS";
  
  // TYPE  sub struct
  const char *headMatrixTypeD_fieldString =  "MTD";
  const char *headFrameTypeD_fieldString  =  "FTD";

  // Matrix type definitions
  const char* mtdMatrixSig_fieldString    = "msig"; 
  const char* mtdColName_fieldString      = "cname"; 

  // Frame type definitions
  const char* ftdFrameSig_fieldString     = "fsig"; 
  const char* ftdMatrixSig_fieldString    = "msig";
  const char* ftdMatrixName_fieldString   = "mname"; 
}

#endif
