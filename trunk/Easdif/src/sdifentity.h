/**
 * @file   sdifentity.h
 * @author Fabien Tisserand
 * @date   Tue Jun 18 20:12:40 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifentity.h,v 1.1 2002-06-18 18:44:14 ftissera Exp $ 
 * 
 * $Log: not supported by cvs2svn $ 
 * 
 */

#ifndef SDIFENTITY_H_
#define SDIFENTITY_H_ 1

#include <string>
#include <vector>
#include <sdif.h>

#include "SdifFPut.h"

#include "sdifframe.h"
#include "sdifnamevaluetable.h"

class SDIFEntity
{

private:
    std::vector<SDIFNameValueTable> mv_NVT;
    /* for taking a nvt from a file */
    SDIFNameValueTable NVT;

    SdifFileT* efile;
    SdifStringT* mDescription;

    SdifUInt4 mNbFrame;
    SdifUInt4 mSize;

    int mNbNVT;
    int mOpen;
    size_t generalHeader;
    size_t asciiChunks;
    size_t bytesread;

    //  char* string;
    //  char* filename;

public: 

    SDIFEntity();
    ~SDIFEntity()
	{
	    SdifStringFree(mDescription);
	    SdifFClose(efile);
	};
    /*temporary*/
    //  SdifFileT* file;

    int AddDescriptionType();
    int AddFrameType(const std::string& frametype, 
		     const std::string& matrix);
    int AddMatrixType(const std::string& matrixtype, 
		      const std::string& colnames);

    int SetSdifString(SdifStringT* String);
    SdifStringT* GetSdifString();
    int ViewString();

    //  int InitString();

    int GetNbNVT() const;
    SDIFNameValueTable& GetNVT(unsigned int i);

    SdifFileT* GetFile() const;
    /*temporary*/
    int SetFile(SdifFileT* SdifFile);

    int Open(const char* filename, SdifFileModeET Mode);
    int OpenRead(const char* filename);
    int OpenWrite(const char* filename);
    
    int Close();
    
    /* operations with NVTs : */
    void ViewAllNVTs();
    SDIFNameValueTable TakeNVT();
    //  int TakeAndAddNVT(SdifUInt4 StreamID);
    /* to do before OpenWrite */
    int AddNVT(const SDIFNameValueTable &nvt, SdifUInt4 StreamID);
    /* module which is used in "OpenWrite(const char* filename)" */
    int WriteNVTs();

    bool ReadNextFrame(SDIFFrame& frame);

    /* **  */
    bool WriteNextFrame(SDIFFrame& frame);
    int GetColumnIndex(SdifSignature matrixsig, std::string columnname);

    SdifErrorT* LastError();

    //   int Close(char* filename);


    
/*
  static Init(std::string sdiftypes = NULL);  // called as SdifEntity::Init(), does SdifGenInit, installs exception callbacks

  //for viewing ?
  std::string LookupAllNVTs(std::string key);

  // a preciser
  int AddDescriptionType(std::string sdiftypedef);       // do this before OpenWrite

  bool ReadNextFrame(SdifFrame &frame);

  int GetColumnIndex(SdifSignature matrixsig, std::string columnname);	// do this after Open

*/


};

#endif





