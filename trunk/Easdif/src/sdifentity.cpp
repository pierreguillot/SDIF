/**
 * @file   sdifentity.cpp
 * @author Fabien Tisserand
 * @date   Tue Jun 18 20:12:24 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifentity.cpp,v 1.5 2002-10-10 10:49:09 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
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

SDIFEntity::SDIFEntity(): efile(0), mSize(0), mEof(0), mNbNVT(0), 
    mOpen(0), generalHeader(0), asciiChunks(0), bytesread(0)
{
    mDescription = SdifStringNew();
};

/* to open a file in mode Read  */
int SDIFEntity::OpenRead(const char* filename)
{
    int n;
    efile = SdifFOpen (filename, eReadFile);
    generalHeader = SdifFReadGeneralHeader(efile);
    asciiChunks = SdifFReadAllASCIIChunks(efile);

    /* to put matrix and frame's types in the SdifString : mDescription */
    SdifFAllMatrixTypeToSdifString(efile, mDescription);
    SdifFAllFrameTypeToSdifString(efile, mDescription);
    n = SdifFNameValueNum(efile);
    if( n != 0)
    {
	/* initialisation of the vector */
	mv_NVT.clear();
	/* used in the loop for getting the next NVT  */	
	SdifNameValuesLT* NVlist;
	NVlist = efile->NameValues;

	for (int i = 1 ; i <= n ; i++)
	{	 
	    if(i == n)
		NVlist->CurrNVT = (SdifNameValueTableT*)
		    SdifListGetNext(NVlist->NVTList);
	    else
		SdifNameValuesLSetCurrNVT(NVlist, i);

	    NVT = TakeNVT();
	    AddNVT(NVT, _SdifNVTStreamID);
	}
    }  
    return  mOpen = 2;
}

/* to open a file in mode Write  */
int SDIFEntity::OpenWrite(const char* filename)
{
    efile = SdifFOpen ( filename, eWriteFile);
    /* write on the file 'SDIF' and 4 bytes chunk size */
    generalHeader = SdifFWriteGeneralHeader(efile);

    /* to add member data (NVTs) to sdif file */
    if (mNbNVT != 0)	
	    WriteNVTs();

    /* to add Descriptions types to sdif file  */
    WriteTypes();
	
    asciiChunks = SdifFWriteAllASCIIChunks(efile);
    return  mOpen = 1;
}

/* to open completely (with Header and ASCII Chunks)  a file in mode : Mode  */
int SDIFEntity::Open(const char* filename, SdifFileModeET Mode)
{
    efile = SdifFOpen (filename, Mode);
    switch(Mode)
    {
    case eReadFile:
    {
	OpenRead(filename);
    }
    break;
    case eWriteFile:
    {	
	OpenWrite(filename);
    }
    break;

    /* the others cases are for administrator : 
     *case ePredefinedTypes, case eModeMask, case eParseSelection:
     */
    default:
	std::cerr << "Default in Opening" << std::endl;
	//_SdifFError(file, eBadMode, "Default in Opening");
	break;
    }
    return mOpen = Mode;
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
    return mNbNVT;
}

int SDIFEntity::AddNVT(const SDIFNameValueTable& nvt, 
			       SdifUInt4 StreamId)
{
    mv_NVT.insert(mv_NVT.end(), nvt);
    mv_NVT[mNbNVT].SetStreamID(StreamId);
    //mSize += mv_NVT.GetSize(); like in SDIFFrame
    mNbNVT++;
    return 1;
}

int SDIFEntity::WriteNVTs()
{
    SdifNameValuesLT* NVlist;
    const char* name;
    const char* value;
    // size_t asciiChunks;    
    typedef std::map<std::string, std::string>::const_iterator CI;

    for (int i = 0 ; i < mNbNVT ; i++)
    {
	NVlist = efile->NameValues;
	SdifNameValuesLNewTable( NVlist, mv_NVT[i].GetStreamID());
    
	//NVlist = SdifNameValuesLNewTable( NVlist, StreamID);
	// NVlist->CurrNVT->NumTable = mNbNVT + 1;

	for (CI p = mv_NVT[i].map_NameValues.begin();
	     p != mv_NVT[i].map_NameValues.end() ; ++p)
	{
	    name = const_cast<char*>(p->first.c_str());
	    value = const_cast<char*>(p->second.c_str());
	    SdifNameValuesLPutCurrNVT   (NVlist, name, value);	
	}
    }
    /* for writing the Name Value Table in the file */
    //  asciiChunks = SdifFWriteAllASCIIChunks(efile);
    return 1;
}

/* to get a NVT of a file */
SDIFNameValueTable SDIFEntity::TakeNVT()
{
    SdifUInt4       iNV;
    SdifHashNT     *pNV;
    SdifHashTableT *HTable;
    SDIFNameValueTable nvt;
    int nbNV = 0;

    HTable = efile->NameValues->CurrNVT->NVHT;  
    // a loop to put the NameValues in a SDIFNameValuesTable 
    for(iNV=0; iNV<HTable->HashSize; iNV++)
    {
	for (pNV = HTable->Table[iNV]; pNV; pNV = pNV->Next)
	{
	    nvt.map_NameValues[((SdifNameValueT *)pNV->Data)->Name] =
		((SdifNameValueT *)pNV->Data)->Value;
	    nbNV ++;
	}
    }
    nvt.SetNbNameValue(nbNV);
    return nvt;
}

/* to get a NVT of an entity */
SDIFNameValueTable& SDIFEntity::GetNVT(unsigned int i)
{
    /* Check the index */
    if (i > (mv_NVT.size()-1))
    {
	std::cerr << " No such NameValueTable " << std::endl;
	return mv_NVT[0];
    }
    return mv_NVT[i];
}

int SDIFEntity::Close()
{
    if (0 != efile)
    {
	SdifFClose(efile);
	efile = 0;
    }
    return 1;
}

/* temporary  */
SdifErrorT* SDIFEntity::LastError()
{
    return SdifFLastError (efile);
}


bool& SDIFEntity::eof() {
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
    for (int n = 0; n < mNbNVT; ++n)
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
