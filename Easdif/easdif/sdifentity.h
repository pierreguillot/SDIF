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
 * @file   sdifentity.h
 * @author Fabien Tisserand
 * @date   Tue Jun 18 20:12:40 2002
 * 
 * @brief  
 * 
 * 
 * 
 * $Id: sdifentity.h,v 1.27 2004-09-10 14:45:28 roebel Exp $ 
 * 
 * $Log: not supported by cvs2svn $
 * Revision 1.26  2004/09/10 10:58:21  roebel
 * Fixed signed unsigned compiler warnings
 *
 * Revision 1.25  2004/09/10 09:20:52  roebel
 * Extend frame directory to contain the matrix signatures for each frame.
 * No longer needs to re read the frame to decide whether frame is selected.
 *
 * Revision 1.24  2004/09/09 19:36:52  roebel
 * Made some members of the iterator private.
 *
 * Revision 1.23  2004/09/09 19:17:38  roebel
 * Version 1.0.0beta:
 * First complete version of iterator access when reading files. Frame-Iterators use the
 * internal Frame Directory that each Entity will generate and update on the fly
 * to minimize disk access during positioning.
 *
 * Revision 1.22  2004/09/08 09:15:57  roebel
 * Improved efficiency of FrameDirectory by means of
 * preventing the need to  search the complete directoy for each frame read.
 * FrameDir is now a list and there exists a current read position pointer into that list
 * that indicates where to search for insert positions.
 *
 * Revision 1.21  2004/08/25 09:22:11  roebel
 * SDIFEntity::Rewind(time) no longer private.
 *
 * Revision 1.20  2004/07/28 13:03:27  roebel
 * Fixed position type to be SdiffPosT
 *
 * Revision 1.19  2004/07/27 17:40:19  roebel
 * Changed include directive to use user path and not system path for sdif.h
 *
 * Revision 1.18  2004/07/21 13:20:24  roebel
 * Added support to hold a frameDirectory in the entity and
 * the possibility to read frames from a given time position.
 *
 * Revision 1.17  2004/07/13 15:01:42  roebel
 * Removed unused variable mNbFrames.
 *
 * Revision 1.16  2004/05/04 12:53:35  roebel
 * Fixed documentation.
 *
 * Revision 1.15  2004/02/11 19:28:17  roebel
 * Added inline function to test state of entity. Added function to access NVTs that indicates existance of name.
 *
 * Revision 1.14  2004/01/19 15:49:55  bogaards
 * Added Rewind and ReadNextSelectedFrame methods to Entity
 *
 * Revision 1.13  2003/08/06 18:00:33  roebel
 * Fixed documentation
 *
 * Revision 1.12  2003/07/18 21:19:36  roebel
 * Made WriteNVTs private. It is for internal use only.
 *
 * Revision 1.11  2003/07/18 20:39:35  roebel
 * Improved documentation
 *
 * Revision 1.10  2003/05/24 00:27:38  roebel
 * Internal representation of types now using std::string.
 * Parameters for type handling routines completely moved
 * to std::string
 *
 * SDIFEntity::SetTypeString no longer appends but sets
 * the internal string. In fact there was now way to reset
 * the type string before.
 *
 * Revision 1.9  2003/05/19 13:59:40  roebel
 * swig rename moved to swig  interface desription.
 *
 * Revision 1.8  2003/05/01 18:59:12  roebel
 * Added missing class keyword for friend declaration.
 *
 * Revision 1.7  2003/04/29 15:54:04  schwarz
 * Use SWIG_RENAME_EASDIF to control class renaming.
 *
 * Revision 1.6  2003/04/29 15:41:30  schwarz
 * Changed all names View* to Print* and *Info to *Header for consistency
 * with SDIF library.
 *
 * Revision 1.5  2003/04/18 17:43:22  schwarz
 * eof() const method
 *
 * Revision 1.4  2003/04/18 16:44:00  schwarz
 * Small changes to make easdif swiggable:
 * - name change for swig-generated classes
 * - eof() returns bool, not bool&
 * - Matrix::Set takes int/float, not int&/float&
 *
 * Revision 1.3  2003/04/08 17:26:04  roebel
 * Updated doc.
 *
 * Revision 1.2  2003/04/06 16:31:08  roebel
 * Added license info
 *
 * Revision 1.1  2003/03/03 19:00:16  roebel
 * Moved src directory to new name easdif
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
 * Revision 1.2  2002/07/12 10:33:56  ftissera
 * *** empty log message ***
 *
 * Revision 1.1  2002/06/18 18:44:14  ftissera
 * Project for new SDIF API
 *  ----------------------------------------------------------------------
 * 
 * 
 */

#ifndef SDIFENTITY_H_
#define SDIFENTITY_H_ 1

#include <string>
#include <list>
#include "sdif.h"


#include "easdif/sdifframe.h"
#include "easdif/sdifnamevaluetable.h"

/* for initialise SDIF library and install exception*/
#include "easdif/sdifinit.h"

namespace Easdif {
  //  pairs of time and file position serve as directory
  struct SDIFLocation {

    SdiffPosT        mPos;
    SdifFrameHeaderS mFrameHdr;
    SdifSignature    mMatrixSig0;
    SdifSignature    mMatrixSig1;
    SdifSignature    mMatrixSig2;
    std::vector<SdifSignature> mMatrixN;

    SDIFLocation():mPos(-1){      
      mMatrixSig0 = mMatrixSig1 = mMatrixSig2 =
        mFrameHdr.Signature = eEmptySignature;
      mFrameHdr.Size      = 0;
      mFrameHdr.NbMatrix  = 0;
      mFrameHdr.NumID     = 0;
      mFrameHdr.Time      = -1.;
    }

    SDIFLocation(SdifUInt4 _pos,SdifUInt4 _id, SdifFloat8 _time, 
                 SdifSignature _sig,  SdifUInt4  _nmatrix   )
      :mPos(_pos)  {
      mMatrixSig0 = mMatrixSig1 = mMatrixSig2 = eEmptySignature;
      mFrameHdr.Signature =   _sig;
      mFrameHdr.Size      = 0;
      mFrameHdr.NbMatrix  = _nmatrix;
      mFrameHdr.NumID     = _id;
      mFrameHdr.Time      = _time;
      if(_nmatrix >3)
        mMatrixN.resize(_nmatrix-3);
    }

    SdifUInt4      LocStreamID()  const { return mFrameHdr.NumID;}
    SdifUInt4      LocNbMatrix()  const { return mFrameHdr.NbMatrix;}
    SdifFloat8     LocTime()      const { return mFrameHdr.Time; }
    SdifSignature  LocSignature() const { return mFrameHdr.Signature;}
    SdiffPosT      LocPos()       const { return mPos;}
    SdifSignature  LocMSignature(SdifUInt4 ind)     const { 
      if(ind >= mFrameHdr.NbMatrix)
        return eEmptySignature;
      switch(ind) {
      case 0 : return mMatrixSig0; break;
      case 1 : return mMatrixSig1; break;
      case 2 : return mMatrixSig2; break;
      default: break;
      }
      return mMatrixN[ind-3];
    }
    void SetMSignature(SdifUInt4 ind,SdifSignature _sig ){
      if( ind >= mFrameHdr.NbMatrix){
        std::cerr << " SDIFLocation:: SetMSignature ind out of bounds "<< ind << "\n";
        return ;
      }
      switch(ind) {
      case 0 : mMatrixSig0 =_sig; break;
      case 1 : mMatrixSig1 =_sig; break;
      case 2 : mMatrixSig2 =_sig; break;
      default : mMatrixN[ind-3] =_sig; break;
      }
    }


  };

namespace {
  // inversion of const tag for elem_iterator classes
  template<int CONST>
  struct swap_const {
    enum {CONSTINV=1};
  };
  
  template<>
  struct swap_const<1> {
    enum {CONSTINV=0};
  };

  template<int CONST>
  struct Base_Iterator {
    typedef  std::list<Easdif::SDIFLocation>::iterator basic_iterator;
  };

  template<>
  struct Base_Iterator<1> {
    typedef std::list<Easdif::SDIFLocation>::const_iterator basic_iterator;
  };

  template<int CONST>
  struct IteratorTypes {
    typedef Easdif::SDIFFrame    value_type;
    typedef Easdif::SDIFFrame   *pointer;
    typedef Easdif::SDIFFrame&   reference;    
  };

  template<>
  struct IteratorTypes<1> {
    typedef const Easdif::SDIFFrame   value_type;
    typedef const Easdif::SDIFFrame  *pointer;
    typedef const Easdif::SDIFFrame&  reference;    
  };
}


/** 
 * @brief class holding all information concerned with a singe sdif file.
 *
 * SDIFEntity is composed of different methods that allow the handling of an 
 * sdif-file. 
 * 
 */


class SDIFEntity
{
  friend class SDIFFrame;
  typedef std::list<SDIFLocation> Directory;

private:

/** 
 * contain the SDIFNameValueTable of the entity
 */
  std::vector<SDIFNameValueTable> mv_NVT;
  
  SdifFileT* efile;
  std::string mDescription;
  
  SdifUInt4 mSize;
  SdiffPosT mFirstFramePos;	// file position after reading the header
  
  bool mEof;
  bool mEofSeen;

  int mOpen;
  size_t generalHeader;
  size_t asciiChunks;

  mutable Directory::iterator mCurrDirPos;
  bool isFrameDirEnabled;

public: 



  /**
   * @brief bidrectional  iterator 
   *
   * An iterator class that will
   * iterate over all selected frames  in the SDIFEntity 
   * Note that write access to the frame pointed to by the iterator 
   * will change a frame in memory and will not yet be written into the file
   * 
   */
  template<int CONST>
  class FRIterator {
    typename Base_Iterator<0>::basic_iterator mBase;
   //typename std::list<SDIFLocation>::iterator mBase;
    SDIFEntity   *mpEnt;
    SDIFFrame     mFrame;
    bool          mlEndUP;
    bool          mlEndDOWN;
    bool          mlFrameIsLoaded;
  private:
    
    
    friend class FRIterator<swap_const<CONST>::CONSTINV>;
    friend class SDIFEntity;
    typedef typename Base_Iterator<CONST>::basic_iterator basic_iterator;
    //typedef std::list<SDIFLocation>::iterator basic_iterator;
    
    void initIterator(bool up) {
      SdifSelectionT *sel   = mpEnt->GetFile()->Selection;
      bool matrixSelection = !SdifListIsEmpty (sel->matrix);
      //position to next selected frame

      if(mBase!=
         mpEnt->mFrameDirectory.end() ||
         (!mlEndDOWN && !up)|| (!mlEndUP && up)) {

        if(mpEnt->mFrameDirectory.size() && (mlEndUP || mlEndDOWN)
           && mBase== mpEnt->mFrameDirectory.end()) {

          if(mlEndDOWN && up)
            {mBase = mpEnt->mFrameDirectory.begin(); mlEndDOWN=false;}
          if(mlEndUP  && !up) 
            {--mBase; mlEndUP= false;}
        }

        while(mBase!=
              mpEnt->mFrameDirectory.end()){

          if(SdifFrameIsSelected(&(mBase->mFrameHdr),sel)) {
            if(matrixSelection) {              
              SdifUInt4 nb = mBase->LocNbMatrix();
              for(SdifUInt4 i  = 0; i< nb;++i){
                if(SdifSelectTestSignature (sel->matrix,mBase->LocMSignature(i) )){
                  return;
                }
              }
            }
            else {
              return;
            }
          }
          if(up)
            ++mBase;
          else{
            if(mBase == mpEnt->mFrameDirectory.begin() )
              break;
            else
              --mBase;                
          }
        }
      }

      if(up && !mpEnt->mEofSeen) {      
        if(mpEnt->ReadNextSelectedFrame(mFrame)){
          mBase = --mpEnt->mFrameDirectory.end();
          mlFrameIsLoaded = true;
          return;
        }
      }

      // signal not found for both directions is end of File      
      mBase = mpEnt->mFrameDirectory.end();
      if(up)
        mlEndUP   = true;
      else
        mlEndDOWN = true;
      mlFrameIsLoaded = true;
      return;
    }

  public:
    typedef typename basic_iterator::iterator_category  iterator_category;
    typedef SDIFFrame        value_type;
    typedef typename basic_iterator::difference_type   difference_type;
    typedef typename IteratorTypes<CONST>::pointer       pointer;
    typedef typename IteratorTypes<CONST>::reference     reference;

     
    SDIFLocation& GetLoc() {
      return *mBase;
    }

    const
    SDIFLocation& GetLoc() const {
      return *mBase;
    }

    bool GotoPos()  {
      SdiffPosT pos = mBase->LocPos();
      mpEnt->mCurrDirPos   = mBase;
      mpEnt->mEof   = false;
      if(-1==SdiffSetPos(mpEnt->GetFile()->Stream,&pos)) {
        std::cerr << "cannot seek to pos %d " <<pos<< "\n";
        exit(1);
        return false;
      }
      
      if(mpEnt->mOpen & 2) {
        size_t SizeR = 0;
        SdifFGetSignature(mpEnt->GetFile(), &SizeR);            
        if(SizeR!=sizeof(SdifSignature)){
          std::cerr << "cannot read signature to pos "<<pos <<"\n";
          exit(1);
          return false;
        }
      }
      
      return true;
    }
       
    
    FRIterator () : mpEnt(0),mlEndUP(false),mlEndDOWN(false),mlFrameIsLoaded(false)  {};

    FRIterator (const FRIterator<CONST> & in) : 
      mBase(in.mBase),mpEnt(in.mpEnt), 
      mlEndUP(in.mlEndUP),mlEndDOWN(in.mlEndDOWN),mlFrameIsLoaded(false)
    {    };

    FRIterator (const FRIterator<swap_const<CONST>::CONSTINV> & in) : 
      mBase(in.mBase), mpEnt(in.mpEnt), 
      mlEndUP(in.mlEndUP), mlEndDOWN(in.mlEndDOWN), mlFrameIsLoaded(false)     
    {    };
	
    FRIterator (const SDIFEntity * _ent, bool end = false) : 
      mBase(end ? 
            const_cast<SDIFEntity *>(_ent)->mFrameDirectory.end() :
            const_cast<SDIFEntity *>(_ent)->mCurrDirPos),
      mpEnt(const_cast<SDIFEntity *>(_ent)),mlFrameIsLoaded(false)
    {
      mlEndUP    = end || (mBase == mpEnt->mFrameDirectory.end()&&mpEnt->mEofSeen);
      mlEndDOWN  = false;
      if(!mlEndUP) {
        initIterator(true);
      }      
    }
	
    FRIterator (const SDIFEntity &_ent, bool end = false) : 
      mBase(end ?
            const_cast<SDIFEntity &>(_ent).mFrameDirectory.end():
            const_cast<SDIFEntity &>(_ent).mCurrDirPos),
      mpEnt(const_cast<SDIFEntity *>(&_ent)),mlFrameIsLoaded(false)
    {
      mlEndUP = end || (mBase == mpEnt->mFrameDirectory.end()&&mpEnt->mEofSeen);
      mlEndDOWN  = false;
      if(!mlEndUP) {
        initIterator(true);
      }
    }
	
    ~FRIterator ( ) {};
	
    FRIterator& operator ++() {
      mlFrameIsLoaded = false;
      mFrame.ClearData();

      if(!mlEndUP ) {
        if(mBase!=mpEnt->mFrameDirectory.end()) 
          ++mBase;

        initIterator(true);
      }    
      return *this;
    }
	
    FRIterator operator ++(int) {
      FRIterator tmp=*this;
      this->operator++();
      return tmp;
    }
		
    
    FRIterator& operator --() {
      mFrame.ClearData();
      mlFrameIsLoaded = false;
      if(!mlEndDOWN) {
        if(mBase==
           mpEnt->mFrameDirectory.end() && !mpEnt->mEofSeen) {
          
          while(!mpEnt->eof()) {
            mpEnt->ReadNextSelectedFrame(mFrame);
          }        
        }
        
        if(mBase!= mpEnt->mFrameDirectory.begin()){
          mlEndUP = false;
          --mBase;
        }
        initIterator(false);      
      }
      return *this;
    }
	
    FRIterator operator --(int) {
      FRIterator tmp=*this;
      this->operator--();
      return tmp;
    }
	
    reference operator*()  { 
      if(!mlFrameIsLoaded) {
        GotoPos();
        if(mpEnt->ReadNextFrame(mFrame) ) {
          mlFrameIsLoaded = true;            
        }
      }
      return mFrame;
    }
    reference operator*() const { 
      if(!mlFrameIsLoaded) {
        GotoPos();
        if(mpEnt->ReadNextFrame(mFrame) ) {
          mlFrameIsLoaded = true;            
        }
      }

      return mFrame;      
    }
    
    pointer operator->() const {      
      if(!mlFrameIsLoaded) {
        GotoPos();
        if(mpEnt->ReadNextFrame(mFrame) ) {
          mlFrameIsLoaded = true;            
        }
      }
      return &mFrame;
    }

    pointer operator->()  {      
      if(!mlFrameIsLoaded) {
        GotoPos();
        if(mpEnt->ReadNextFrame(mFrame) ) {
          mlFrameIsLoaded = true;            
        }
      }
      return &mFrame;
    }
	
    template<int OC>
    bool operator==(const FRIterator<OC>& i)const {
      return((i.mlEndUP  ||i.mlEndDOWN) == (mlEndUP || mlEndDOWN)  && i.mBase == mBase);
    }

    template<int OC>
    bool operator!=(const  FRIterator<OC>& i)const {
      return !operator==(i);
    }



  };
  typedef FRIterator<0>       iterator;
  typedef FRIterator<1> const_iterator;      
  friend class FRIterator<0>;
  friend class FRIterator<1>;

  mutable Directory           mFrameDirectory;


    /// Default constructor
    SDIFEntity();
    ~SDIFEntity()
	{	    
	    if (0 != efile)
	    {
		SdifFClose(efile);
		efile = 0;
	    }
	};



  const_iterator
  begin () const {
    return const_iterator(this,false);
  }

  iterator
  begin ()  {
    return iterator(this,false);
  }

  const_iterator
  end () const {
    return const_iterator(this,true);
  }

  iterator
  end ()  {
    return iterator(this,true);
  }

  const SDIFLocation&
  GetCurLoc() const {
    return *mCurrDirPos;
  }

  Directory::const_iterator
  GetCurItPos() const {
    return Directory::const_iterator(mCurrDirPos);
  }

  const Directory&
  GetDirectory() const {
    return mFrameDirectory;
  }

/*************************************************************************/
/* Description type */
/**
* \defgroup  description SDIFEntity - Description type
*/

/** 
 * \ingroup description
 * \brief write type info
 *
 * add the description type to the frame "1TYP". (for internal use when
 * opening the file )
 */
 private:
    bool WriteTypes();
    //int AddDescriptionType();

 public:
/** 
 * \ingroup description
 *
 * \brief add new frame type  or change existing  frame type  of the entity
 *
 * the description is added to the file with WriteTypes()
 * when the entity is opened for wrinting.Therefore, 
 * type descriptors  have to be added
 * to the entity before the file is opened.
 *
 * AddFrameType() must be used after Easdif::SDIFEntity::AddMatrixType()
 *
 * @param frametype string that identifies the  new frame type,
 *  the identifier will use  at most 4 characters
 * @param matrix string that defines the matrix elements that may be part  
 * of the frame
 *
 * Example: 
 * 
 *  entity.AddFrameType("1NEW", "1NEW NewMatrix; 1FQ0 New1FQ0");
 *
 */
    bool AddFrameType(const std::string& frametype, 
		      const std::string& matrix);

/** 
 * \ingroup description
 * 
 * \brief define a new or redefine an existing matrix type for  the entity.
 *
 * The type description is added to the entity when the entity is
 * opened for writing. Therefore, type descriptors  have to be added
 * to the entity before the file is opened.
 *
 * AddMatrixType() must be used before Easdif::SDIFEntity::AddFrameType()
 *
 * @param matrixtype string for define a new matrix type 
 * or redefined a matrix type 
 * @param colnames string to defined the differents parameters of 
 * the matrix type
 *
 * Example: 
 * 
 *  entity.AddMatrixType("1NEW", "amplitude, phase");
 *
 */
    bool AddMatrixType(const std::string& matrixtype, 
		       const std::string& colnames);

/** 
 * \ingroup description
 * Print the SdifStringT* which have the types definitions of the frames
 * and matrix
 */
    int PrintTypes();

/** 
 * \ingroup description
 * return a string  containing the user defined types of the
 * file that is stored in the frame "1TYP".
 *
 *  return Type string
 */
  const std::string& GetTypeString() const;

/** 
 * \ingroup description
 * Set the user defined frame types in the frame "1TYP" for the current file to
 * the argument string 
 * 
 * \param TypeString
 * \return return true if success
 */
  bool SetTypeString(const std::string& TypeString);

/*************************************************************************/
/* Change the selection */
/**
 * \defgroup selection SDIFEntity - Selection
 */

 /** 
 * \ingroup selection
  * Replace current selection by new one given in argument.
  * The selection specification may contain all the parts of a filename
  * based selection after the  selection indicator :: .
  */
    int ChangeSelection(const std::string& selection);


/*************************************************************************/
/* Operation with a file */
/**
* \defgroup  file  SDIFEntity - Operations with file
*/

/** 
 * \ingroup  file
 * open the file of the entity in reading or writing mode
 * 
 * @param filename 
 * @param Mode can be "eReadFile" or "eWriteFile"
 *
 * @return true if opened/false if error
 */
    bool Open(const char* filename, SdifFileModeET Mode);

/** 
 * \ingroup  file
 * open a file in reading mode
 * @param filename 
 *               
 * @return true if opened/false if error
 */
    bool OpenRead(const char* filename);

/** 
 * \ingroup  file
 * open a file in reading mode without destroying any information
 *    in the internal FrameDirectory
 * @param filename 
 *
 * In constrast to standard OpenRead the internal frame directory 
 * is kept and reused.
 *     Attention reusing a directory is only reasonable if the
 *       same file is.
 * 
 * If there is no directory information, either if the SDIFEntity
 * has just been  created or the file that has previously been opened
 * did not use a frame directory this call is equivalent to OpenRead()
 *               
 * @return true if opened/false if error
 */
    bool ReOpenRead(const char* filename);

/** 
 * \ingroup  file
 * open a file in writing mode
 * @param filename 
 * @return true if opened/false if error
*/
    bool OpenWrite(const char* filename);
    
/** 
 * \ingroup  file
 * close a file 
 * @return true if closed / false if file was not opened
 */
    bool Close();

/** 
 * \ingroup  file
 * rewind a file to first non-ascii frame after the file header
 * @return true if positioning was successful
 */
    
    bool Rewind();



  /** 
   * \ingroup file
   * get the SdifFileT* file
   * \return SdifFile pointer related to Entity
   */
  SdifFileT* GetFile() const;

  /** 
   * \ingroup file
   *
   * test file state
   *
   * \return true if file is opened and not at eof
   */
  bool good() const {
    return (!eof() && mOpen && efile);
  }

/**
 * \ingroup file
 * true if file is at eof
 */
  bool eof() const {return mEof;}


  /**
   * \ingroup file
   * enable Frame Directory:
   *
   * If enabled the entity will internally keep track of the positions of all
   * frames and thereby allow to quickly reposition to a desired frame
   * by specifying the requested Frame time.
   *
   * enableFrameDir can be called at any time, however, is most effectively
   * set before any frames are read. If it is enabled later the 
   * all frames up to the current frame are reread to obtain a complete
   * and consistent directory. 
   */
  void EnableFrameDir()  throw(SDIFDirError);

  /**
   * \ingroup file
   * true if frame Directory is enabled
   */
  bool IsFrameDir() {   return isFrameDirEnabled;}


  /**
   * \ingroup file
   * dump content of Frame Directory to stderr
   */
  void PrintFrameDir() const;

private:

  /**
   * \ingroup file
   *
   * add a new time/position point to the directory
   * used only internally to maintain directory from 
   * the frame read interface.
   * \return pointer to location that needs to be completed 
   *     with matrix signatures.
   */
  SDIFLocation* 
  AddFramePos(SdifUInt4 id, SdifSignature sig, SdifFloat8 time,
              SdifUInt4 nbMatrix, SdiffPosT pos);
private:
  // this appears to be a remaining of the initial development 
  // will be removed in the future
    /*temporary SetFile*/
    int SetFile(SdifFileT* SdifFile);

public:
    SdifErrorT* LastError();

/*************************************************************************/
/* Name Value Table */
/**
* \defgroup  nvt  SDIFEntity - Name Value Table
*/

/** 
 * \ingroup nvt
 * to see all the Name Values Tables
 */
    void PrintAllNVTs();

/**
 * \ingroup nvt 
 * take a Name Values Table of a file. (for internal used in opening)
 * @return SDIFNameValueTable of the file
 */
 private:
    SDIFNameValueTable TakeNVT();

 public:
/** 
 * \ingroup nvt
 * insert a SDIFNameValueTable in the vector of Name Value Table
 * 
 * @param nvt Name Value Table to insert in the vector
 * @param StreamID StreamId of the Name Value Table 
 *
 * \return true if ok/false if error.
 */

  bool AddNVT(const SDIFNameValueTable &nvt, SdifUInt4 StreamID=0);


/** 
 * \ingroup nvt
 * write the vector of Name Value Table in the file when opening a file 
 * for writing (for internal use only)
 *
 * \return true if ok/false if error.
 */
private:
    bool WriteNVTs();
public:

/**
 * \ingroup nvt 
 * get the number of Name Value Tables  stored in entity
 * 
 * \return number of Name Value Tables
 */
    int GetNbNVT() const;

/** 
 * \ingroup nvt
 * get the i-th Name Value Table
 */
    SDIFNameValueTable& GetNVT(unsigned int i);




/******************/
/*************************************************************************/
/* Read and Write */
/**
* \defgroup  rnwentity  SDIFEntity - Read and Write
*/

/**
 * \ingroup rnwentity
 * read the next frame of the file
 * return the number of bytes read
 */
    int ReadNextFrame(SDIFFrame& frame);

/**
 * \ingroup rnwentity
 * read the next frame of the file that is located after time timePos 
 * return the number of bytes read
 *
 * Calling this function will automatically enable the internal
 * FrameDirectory.
 *
 * \see EnableFrameDir()
 */
    int ReadNextFrame(SDIFFrame& frame, SdifFloat8 timePos);

/**
 * \ingroup rnwentity
 * read the next selected frame of the file
 * return the number of bytes read
 */

  int ReadNextSelectedFrame(SDIFFrame& frame);

/**
 * \ingroup rnwentity
 * read the next selected frame of the file starting from position timePos
 * return the number of bytes read
 *
 * Calling this function will automatically enable the internal
 * FrameDirectory.
 *
 * \see EnableFrameDir()
 */
  int ReadNextSelectedFrame(SDIFFrame& frame,SdifFloat8 timePos);


/** 
 * \ingroup rnwentity
 * write the next frame of the file
 * return the size of the frame write
 */
    int WriteFrame(SDIFFrame& frame);
    

/*
  TODO
  int GetColumnIndex(SdifSignature matrixsig, std::string columnname);	// do this after Open
*/


};

} // end of namespace Easdif

#endif


