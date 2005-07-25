// $Id: easdif-java.i,v 1.2 2005-07-25 13:29:24 roebel Exp $ -*-c-*-
//
// easdif-java.i		05.05.2003		Patrice Tisserand
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
// Revision 1.1  2003/05/05 16:05:32  tisseran
// Added swig java binding.
// TODO: change configure and java/Makefile.am for jni include path
//
//

// generate python module named 'eaSDIF'
%module eaSDIF




// swig + java doesn't like a lot method overloaded only by const
%ignore Easdif::SDIFNameValueTable::begin() const;
%ignore Easdif::SDIFNameValueTable::end() const;

// const char* and std::string are the same
%ignore Easdif::EasdifInit(const char*);
// we know how to convert SdifSignature to std::string ...
%ignore Easdif::SDIFFrame::SetSignature(SdifSignature);
%ignore Easdif::SDIFMatrix::SetSignature(SdifSignature);

%ignore Easdif::SDIFMatrix::Init(SdifSignature, int, int, SdifDataTypeET);

// include typemaps
%include sdiftypemap-java.i

// include common interface files
%include ../easdif-defines.i
%include ../easdif-common.i

// java module need to know EasdifInit function
%include "../../easdif/sdifinit.hpp"
