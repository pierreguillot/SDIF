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
 * @file   sdifentity.cpp
 * @author Fabien Tisserand
 * @date   Tue Jun 18 20:12:24 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifentity.cpp,v 1.4 2003-04-18 17:43:22 schwarz Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.3  2003/04/18 16:44:00  schwarz
 * Small changes to make easdif swiggable:
 * - name change for swig-generated classes
 * - eof() returns bool, not bool&
 * - Matrix::Set takes int/float, not int&/float&
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
 *
 * Revision 1.8  2003/02/07 18:37:58  roebel
 * Empty sdif files (only header information) will be correctly handled
 * now.
 *
 * Revision 1.7  2002/11/27 20:13:04  roebel
 * Removed member that redundantly indicated size of NVT vector.
 * Make use of improved SDIFNameValueTable interface.
 *
 * Revision 1.6  2002/10/30 15:27:32  roebel
 * Changed return type from int to bool.
 * Changed error checking in openroutines.
 *
 * Revision 1.5  2002/10/10 10:49:09  roebel
 * Now using namespace Easdif.
 * Fixed handling of zero pointer arguments in initException.
 * Reading past end of file now throws an exception.
 *
 * Revision 1.4  2002/10/03 11:26:56  tisseran
 * Check if efile is not null before trying to close it.
 * Initialize efile to 0 by sdifentity::sdifentity()
 *
 * Revision 1.3  2002/08/28 16:46:53  roebel
 * Internal reorganization and name changes.
 *
 * Revision 1.2  2002/07/12 10:34:08  ftissera
 * *** empty log message ***
 *
 * Revision 1.1  2002/06/18 18:43:08  ftissera
 * Project for new SDIF API
 * 
 * 
 */

#include "easdif/sdifentity.h"

#include "SdifFPut.h"


//inline int EasdifInit(std::string PredefinedType)
//inline int EasdifInit(char* PredefinedType)
//{
/*
  char* Predefined;    
  Predefined = const_cast<char*>(PredefinedType.c_str());
*/
//  SdifGenInit(PredefinedType);
//  SdifSetErrorFunc(ExceptionThrower);
//}

namespace Easdif {

SDIFEntity::SDIFEntity(): efile(0), mSize(0), mEof(0), 
    mOpen(0), generalHeader(0), asciiChunks(0), bytesread(0)
{
    mDescription = SdifStringNew();
};

/* to open a file in mode Read  */
bool SDIFEntity::OpenRead(const char* filename)
{
    int n;
    efile = SdifFOpen (filename, eReadFile);

    if(!efile)
      return false;

    generalHeader = SdifFReadGeneralHeader(efile);
    if(!generalHeader){
      SdifFClose(efile);
      efile = 0;
      return false;
    }

    asciiChunks = SdifFReadAllASCIIChunks(efile);

    /* to put matrix and frame's types in the SdifString : mDescription */
    SdifFAllMatrixTypeToSdifString(efile, mDescription);
    SdifFAllFrameTypeToSdifString(efile, mDescription);
    n = SdifFNameValueNum(efile);
    /* initialisation of the vector */
    mv_NVT.clear();

    if( n != 0)
    {
	/* used in the loop for getting the next NVT  */	
	SdifNameValuesLT* NVlist;
	NVlist = efile->NameValues;

	for (int i = 1 ; i <= n ; i++)
	{	 
	  SdifNameValuesLSetCurrNVT(NVlist, i);
	  AddNVT(TakeNVT(), _SdifNVTStreamID);
	}
    }  

    // empty sdif file
    if(feof(efile->Stream))
      mEof = true;

    mOpen = 2;
    return true;
}

/* to open a file in mode Write  */
bool SDIFEntity::OpenWrite(const char* filename)
{
    efile = SdifFOpen ( filename, eWriteFile);

    if(!efile)
      return false;

   /* write on the file 'SDIF' and 4 bytes chunk size */
    if(!(generalHeader = SdifFWriteGeneralHeader(efile))){
      SdifFClose(efile);
      efile = 0;
      return false;
    }

    /* to add member data (NVTs) to sdif file */
    if (mv_NVT.size() != 0)	
      WriteNVTs();

    /* to add Descriptions types to sdif file  */
    WriteTypes();
	
    asciiChunks = SdifFWriteAllASCIIChunks(efile);
    mOpen = 1; 
    return true; 
}

/* to open completely (with Header and ASCII Chunks)  a file in mode : Mode  */
bool SDIFEntity::Open(const char* filename, SdifFileModeET Mode)
{
    efile = SdifFOpen (filename, Mode);
    switch(Mode)
    {
    case eReadFile:
    {
	return OpenRead(filename);
    }
    break;
    case eWriteFile:
    {	
	return OpenWrite(filename);
    }
    break;

    /* the others cases are for administrator : 
     *case ePredefinedTypes, case eModeMask, case eParseSelection:
     */
    default:
	std::cerr << " SDIFEntity::Open:: Mode specification error" << std::endl;
	//_SdifFError(file, eBadMode, "Default in Opening");
	break;
    }
    return false;
}

SdifFileT* SDIFEntity::GetFile() const
{
    return efile;
}

int SDIFEntity::SetFile(SdifFileT* SdifFile)
{
    efile = SdifFile;
    return 1;
}

int SDIFEntity::GetNbNVT()const
{
    return mv_NVT.size();
}

bool SDIFEntity::AddNVT(const SDIFNameValueTable& nvt, 
			       SdifUInt4 StreamId)
{
    mv_NVT.push_back(nvt);
    mv_NVT.back().SetStreamID(StreamId);

    return true;
}

bool SDIFEntity::WriteNVTs()
{
    SdifNameValuesLT* NVlist;

    int nnvts = mv_NVT.size();
    for (int i = 0 ; i < nnvts ; i++)
    {
	NVlist = efile->NameValues;
	SdifNameValuesLNewTable( NVlist, mv_NVT[i].GetStreamID());
    
	//NVlist = SdifNameValuesLNewTable( NVlist, StreamID);
	// NVlist->CurrNVT->NumTable = mNbNVT + 1;

	for (SDIFNameValueTable::const_iterator p = mv_NVT[i].begin();
	     p != mv_NVT[i].end() ; ++p)
	{
	    SdifNameValuesLPutCurrNVT(NVlist, 
				      GetNameFromSDIFNVTIt(p).c_str(), 
				      GetValueFromSDIFNVTIt(p).c_str());	
	}
    }
    /* for writing the Name Value Table in the file */
    //  asciiChunks = SdifFWriteAllASCIIChunks(efile);
    return true;
}

/* to get a NVT of a file */
SDIFNameValueTable SDIFEntity::TakeNVT()
{
    SdifUInt4       iNV;
    SdifHashNT     *pNV;
    SdifHashTableT *HTable;
    SDIFNameValueTable nvt;

    HTable = efile->NameValues->CurrNVT->NVHT;  
    // a loop to put the NameValues in a SDIFNameValuesTable 
    for(iNV=0; iNV<HTable->HashSize; iNV++)
    {
	for (pNV = HTable->Table[iNV]; pNV; pNV = pNV->Next)
	{

	  nvt.AddNameValue(((SdifNameValueT *)pNV->Data)->Name,
			   ((SdifNameValueT *)pNV->Data)->Value);
	}
    }

    return nvt;
}

/* to get a NVT of an entity */
SDIFNameValueTable& SDIFEntity::GetNVT(unsigned int i)
{
    /* Check the index */
    if (i > (mv_NVT.size()-1))
    {
	std::cerr << " No such NameValueTable " << std::endl;
        mv_NVT.push_back(SDIFNameValueTable());
	return mv_NVT[0];
    }
    return mv_NVT[i];
}

bool SDIFEntity::Close()
{
    if (0 != efile)
    {
	SdifFClose(efile);
	efile = 0;
	return true;
    }
    return false;
}

/* temporary  */
SdifErrorT* SDIFEntity::LastError()
{
    return SdifFLastError (efile);
}


bool SDIFEntity::eof() const 
{
    return mEof;
}


int SDIFEntity::ReadNextFrame(SDIFFrame& frame)
{
    int bytesread = 0;

    if(eof()) {
      // return -1;
      SDIFEof exc;
      exc.initException(eError,
			"Error in SDIFEntity::ReadNextFrame -- Eof reached",
			efile,0,0,0);      
      throw exc;
    }

    //bytesread = frame.Read(efile, eof());
    bytesread = frame.Read(efile, mEof);
    return bytesread;    
/*
  int eof = 0;
  size_t bytesread = 0;
  if (!eof  &&  SdifFLastError(efile) == NULL)
  {
  bytesread = frame.Read(efile);
  eof = SdifFGetSignature(efile, &bytesread) == eEof;
  return bytesread;
  }
  else
  return -1;
*/
}

int SDIFEntity::WriteFrame(SDIFFrame& frame)
{
    int size_frame = 0;
    
    frame.Write(efile);
    return size_frame;
}

void SDIFEntity::ViewAllNVTs()
{
  int nnvts = mv_NVT.size();
    for (int n = 0; n < nnvts; ++n)
    {
	std::cout << std::endl <<"NameValueTable number : " << n+1 
		  << std::endl;
	mv_NVT[n].ViewNameValueTable();
    }
}

/*******************************/

int SDIFEntity::SetTypeString(SdifStringT* String)
{
    SdifStringAppend(mDescription , String->str);
    return 1;
}

SdifStringT* SDIFEntity::GetTypeString()
{
    return mDescription;
}

int SDIFEntity::ViewTypes()
{
    std::cout << std::endl <<  mDescription->str << std::endl;
    return 1;
}

/* for adding a frame type */
int SDIFEntity::AddFrameType(const std::string& frametype, 
			     const std::string& matrix)
{
    SdifStringAppend(mDescription , "1FTD");
    SdifStringAppend(mDescription ,const_cast<char*>(frametype.c_str()));
    SdifStringAppend(mDescription ," { ");
    SdifStringAppend(mDescription ,const_cast<char*>(matrix.c_str()));
    SdifStringAppend(mDescription ,";  }\n");
    return 1;
}

/* for adding a matrix type */
int SDIFEntity::AddMatrixType(const std::string& matrixtype, 
			      const std::string& colnames)
{
    SdifStringAppend(mDescription , "1MTD");
    SdifStringAppend(mDescription ,const_cast<char*>(matrixtype.c_str()));
    SdifStringAppend(mDescription ," { ");
    SdifStringAppend(mDescription ,const_cast<char*>(colnames.c_str()));
    SdifStringAppend(mDescription ," }\n");
    return 1;
}

/* for adding the description types when opening in mode "eWriteFile" */
int SDIFEntity::WriteTypes()
{
  if (mDescription->TotalSize > 0)
  {
      SdifFGetAllTypefromSdifString(efile, mDescription);
      return 1;
  }
  else
  return 0;
}


int SDIFEntity::ChangeSelection(const std::string& selection)
{
    const char* sel = const_cast<const char*>(selection.c_str());
    SdifReplaceSelection(sel, efile->Selection);
    return 1;
}

/*****************************************************/

} // end of namespace Easdif
