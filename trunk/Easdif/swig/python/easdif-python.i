// $Id: easdif-python.i,v 1.9 2012-01-05 01:18:07 roebel Exp $ -*-c-*-
//
// easdif-python.i		30.04.2003		Patrice Tisserand
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
// Revision 1.8  2012/01/04 12:19:09  roebel
// Create derived class to render Frame and Entity into python iterable.
//
// Revision 1.7  2006/12/10 17:37:30  roebel
// SelectionSet now renamed to SelectionSetI to prevent
// name clash in swig.
//
// Revision 1.6  2006/12/08 18:02:20  roebel
// Extended python support for SelectionSet.
//
// Revision 1.5  2006/11/26 20:35:41  roebel
// Unified by means of ../easdif-common-init.i
//
// Revision 1.4  2006/11/26 16:15:08  roebel
// Improved platform support by means of using automake more consistently.
// Added new example script.
// Reorganized to work with swig 1.3.31!
//
// Revision 1.3  2003/06/05 11:15:09  roebel
// Moved stlmap conditional to easdif-common.i.in
//
// Revision 1.2  2003/05/28 16:57:03  roebel
// Made configure switching map support according to the swig version installed.
//
// Revision 1.1  2003/04/30 11:45:48  tisseran
// Added swig python stuff
// Started autoconfiscation of swig
//
// include common init
%include ../easdif-common-init.i

// include typemaps
%include sdiftypemap-python.i

// define only the needed basic SDIF stuff from sdif.h
%include ../easdif-defines.i

%{
#include "easdif/easdif.h"

// proxy class for frame that provides Frame iterator with storage location for iterator position
struct
FrameIt  {
  const Easdif::SDIFFrame *mpOriFrame;
  int mIterPos;
  FrameIt(const Easdif::SDIFFrame& in ) : mpOriFrame(&in), mIterPos(0) {}
  ~FrameIt() {}
  const Easdif::SDIFMatrix&  
  next() {
    if(mIterPos < static_cast<int>(mpOriFrame->GetNbMatrix()))
      return mpOriFrame->GetMatrix(mIterPos++);
    throw std::runtime_error("StopIteration");
  }
};


%}

// include common module directive
%include ../easdif-common.i

// include typemap for std::string from SWIG library
%include std_set.i

// provide access to std::set as eaSDIF.vector
namespace std {
   %template(ISet) set<unsigned int>;
};


%template(SelectionSetI) Easdif::SelectionSet<unsigned int>;


// make entity iterable
// but attention next method does not provide a reference
// but a pointer to a new Frame so we cannot modify the entity by means of the 
// frames in a loop
%exception Easdif::SDIFEntity::next() {
  try {
    $action
      }
  catch(const Easdif::SDIFEof& e) {
      PyErr_SetNone(PyExc_StopIteration);
      return NULL;
  }
 }


%extend Easdif::SDIFEntity{
  Easdif::SDIFEntity&
  __iter__() {
    // ensure that the complete file will be read
    $self->Rewind();
    return *$self;
  }

   // prefer to work with pointers in python
   Easdif::SDIFFrame*
   next() {
     //allocate new frame
     Easdif::SDIFFrame *localFrame = new(Easdif::SDIFFrame);
     try {
       $self->ReadNextSelectedFrame(*localFrame);
     } catch(...) {
       delete localFrame;
       throw;
     }
     return localFrame;
   }
 }

// make entity iterable
// but attention next method does not provide a reference
// but a pointer to a new Matrix.  This avoids problems with the missing reference counting
// that would arise if teh original frame disappears before the matrix
%exception Easdif::SDIFFrame::next() {
  try {
    $action
      }
  catch(const std::runtime_error& e) {
      PyErr_SetNone(PyExc_StopIteration);
      return NULL;
  }
 }


%extend Easdif::SDIFFrame {
  Easdif::SDIFFrame*
  __iter__() {
    return reinterpret_cast<Easdif::SDIFFrame*>(new FrameIt(*$self));
  }
  // prefer to work with pointers in python
  Easdif::SDIFMatrix*
  next() {
    FrameIt * it = reinterpret_cast<FrameIt*>($self);
    Easdif::SDIFMatrix *pMat = new(Easdif::SDIFMatrix);
    try {
      *pMat = it->next();
    }
    catch(...) {
      delete  pMat;
      throw;
    }
    return pMat; 
  }
}

