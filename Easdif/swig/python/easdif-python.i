// $Id: easdif-python.i,v 1.6 2006-12-08 18:02:20 roebel Exp $ -*-c-*-
//
// easdif-python.i		30.04.2003		Patrice Tisserand
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
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
// Made configure switching map support according tothe swig version installed.
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

// include common module directive
%include ../easdif-common.i

// include typemap for std::string from SWIG library
%include std_set.i

// provide access to std::set as eaSDIF.vector
namespace std {
   %template(ISet) set<unsigned int>;
};

%template(SelectionSet) Easdif::SelectionSet<unsigned int>;

