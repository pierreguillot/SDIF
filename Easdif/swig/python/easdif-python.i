// $Id: easdif-python.i,v 1.2 2003-05-28 16:57:03 roebel Exp $ -*-c-*-
//
// easdif-python.i		30.04.2003		Patrice Tisserand
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
// Revision 1.1  2003/04/30 11:45:48  tisseran
// Added swig python stuff
// Started autoconfiscation of swig
//

// generate python module named 'eaSDIF'
%module eaSDIF

%init
%{
    EasdifInit();
%}

#ifdef SWIG_HAVE_STLMAP
%include stl.i

// for name value table
%template(stringmap) std::map<std::string,std::string>;
#endif

// include typemaps
%include sdiftypemap-python.i

// define only the needed basic SDIF stuff from sdif.h
%include ../easdif-defines.i

// include common module directive
%include ../easdif-common.i

