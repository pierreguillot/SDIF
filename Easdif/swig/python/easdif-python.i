// $Id: easdif-python.i,v 1.13 2012-09-02 01:19:03 roebel Exp $ -*-c-*-
//
// easdif-python.i		30.04.2003		Patrice Tisserand
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
// Revision 1.12  2012/08/28 22:08:14  roebel
// Fixed next iterator for sdifentity.
//
// Revision 1.11  2012/08/19 18:28:57  roebel
// Added copy method to Frame.
//
// Revision 1.10  2012/01/05 11:11:21  roebel
// Added documentation and fixed memory leak.
//
// Revision 1.9  2012/01/05 01:18:07  roebel
// Make Entity and Frame iterable and take car that iteration is efficient by means of
// returning pointers to Frames and matrices.
//
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
%feature("autodoc", "1");

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

%newobject Easdif::SDIFEntity::next;
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
       // force to run into eof exception if unselected frames are located at the end of the file
       while(!$self->ReadNextSelectedFrame(*localFrame)){};
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

%newobject Easdif::SDIFFrame::next;
%newobject Easdif::SDIFFrame::copy;
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

  Easdif::SDIFFrame*
  copy() {
    return new Easdif::SDIFFrame(*$self);
  }

}


#ifdef USE_NUMPY
 

%extend Easdif::SDIFMatrix {

%apply (double* INPLACE_ARRAY1, int DIM1) {(double *outarr, int outarrsize)}
%apply (double* IN_ARRAY1, int DIM1) {(double *inarr, int inarrsize)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(float *outarr, int outarrsize)}
%apply (float* IN_ARRAY1, int DIM1) {(float *inarr, int inarrsize)}
%apply (int* INPLACE_ARRAY1, int DIM1) {(int *outarr, int outarrsize)}
%apply (int* IN_ARRAY1, int DIM1) {(int *inarr, int inarrsize)}

  void
    GetColA(double * outarr, int outarrsize, int col) {
    if ($self->GetNbRows() != outarrsize ) {
      throw std::runtime_error("GetColA::output array does not match column size");
    }
    $self->GetCol(outarr, col);
    return; 
  }
  void
    GetRowA(double * outarr, int outarrsize, int row) {
    if ($self->GetNbCols() != outarrsize ) {
      throw std::runtime_error("GetRowA::output array does not match row size");
    }
    $self->GetRow(outarr, row);
    return; 
  }
  void
    SetColA(double * inarr, int inarrsize, int col) {
    if ($self->GetNbRows() != inarrsize ) {
      throw std::runtime_error("SetColA::input array does not match column size");
    }
    $self->SetCol(const_cast<const double *>(inarr), col);
    return; 
  }
  void
    SetRowA(double * inarr, int inarrsize, int row) {
    if ($self->GetNbCols() != inarrsize ) {
      throw std::runtime_error("SetRowA::output array does not match row size");
    }
    $self->SetRow(inarr, row);
    return; 
  }

  void
    GetColA(float * outarr, int outarrsize, int col) {
    if ($self->GetNbRows() != outarrsize ) {
      throw std::runtime_error("GetColA::output array does not match column size");
    }
    $self->GetCol(outarr, col);
    return; 
  }
  void
    GetRowA(float * outarr, int outarrsize, int row) {
    if ($self->GetNbCols() != outarrsize ) {
      throw std::runtime_error("GetRowA::output array does not match row size");
    }
    $self->GetRow(outarr, row);
    return; 
  }
  void
    SetColA(float * inarr, int inarrsize, int col) {
    if ($self->GetNbRows() != inarrsize ) {
      throw std::runtime_error("SetColA::input array does not match column size");
    }
    $self->SetCol(inarr, col);
    return; 
  }
  void
    SetRowA(float * inarr, int inarrsize, int row) {
    if ($self->GetNbCols() != inarrsize ) {
      throw std::runtime_error("SetRowA::output array does not match row size");
    }
    $self->SetRow(inarr, row);
    return; 
  }

  void
    GetColA(int * outarr, int outarrsize, int col) {
    if ($self->GetNbRows() != outarrsize ) {
      throw std::runtime_error("GetColA::output array does not match column size");
    }
    if (sizeof(int) != sizeof(SdifInt4))
      throw std::runtime_error("SetColA::output array int type is not comptaible to SdifInt4");
    $self->GetCol(outarr, col);
    return; 
  }
  void
    GetRowA(int * outarr, int outarrsize, int row) {
    if ($self->GetNbCols() != outarrsize ) {
      throw std::runtime_error("GetRowA::output array does not match row size");
    }
    if (sizeof(int) != sizeof(SdifInt4))
      throw std::runtime_error("SetRowA::output array int type is not comptaible to SdifInt4");

    $self->GetRow(outarr, row);
    return; 
  }
  void
    SetColA(int * inarr, int inarrsize, int col) {
    if ($self->GetNbRows() != inarrsize ) {
      throw std::runtime_error("SetColA::input array does not match column size");
    }
    if (sizeof(int) != sizeof(SdifInt4))
      throw std::runtime_error("SetColA::input array int type is not comptaible to SdifInt4");
    $self->SetCol(inarr, col);
    return; 
  }
  void
    SetRowA(int * inarr, int inarrsize, int row) {
    if ($self->GetNbCols() != inarrsize ) {
      throw std::runtime_error("SetRowA::output array does not match row size");
    }
    if (sizeof(int) != sizeof(SdifInt4))
      throw std::runtime_error("SetRowA::input array int type is not comptaible to SdifInt4");
    $self->SetRow(inarr, row);
    return; 
  }
}
#endif
