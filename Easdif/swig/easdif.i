// $Id: easdif.i,v 1.9 2003-04-29 15:20:38 schwarz Exp $ -*-c-*-
//
// easdif.i		16.04.2003		Diemo Schwarz
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
// Revision 1.8  2003/04/18 17:45:23  schwarz
// Renamed sdifsignature.i to sdiftypemap.i, because all types are in there.
// Include typemap for std::string from SWIG library
// Added typemap freearg SdifStringT *
//
// Revision 1.7  2003/04/18 15:41:18  schwarz
// Don't include all definitions from sdif.h, only the ones needed by
// easdif, defined in sdifdefine.i.  -> smaller wrapper size:
// .cxx	 880k -> 130k
// .o	1000k -> 210k
// .pm	 123k -> 8k
//
// Revision 1.6  2003/04/18 14:31:27  schwarz
// Added typemap for string to SdifSignature conversion (input arg):
// Setting/writing data works --> wrapper complete, but rudimentary.
//
// Revision 1.5  2003/04/17 12:20:39  schwarz
// Output (return value) mapping for SdifSignature works!
//
// Revision 1.4  2003/04/17 11:15:01  schwarz
// Swigging sdif.h works (with some warnings), but that's not yet it...
//
// Revision 1.3  2003/04/17 11:03:52  schwarz
// Swigged all easdif classes!
// Now going on to map sdif basic types.
//
// Revision 1.2  2003/04/17 10:19:51  schwarz
// Nice class and method names using renaming.
//
// Revision 1.1  2003/04/17 09:51:10  schwarz
// First working swig generated perl extension!
// Classes re-declared in easdif.i, simple method without shadow classes.
// Doesn't do much except counting frames.


// generate perl module named 'eaSDIF'
%module eaSDIF

// include typemaps
%include sdiftypemap.i

// include header to make wrapper compile
%{
#include "easdif/easdif.h"

using namespace Easdif;
%}


// module initialisation code
%init 
%{
    EasdifInit();
%}


// would have to zap other constructors, if not, SdifSignature is invisible...
//%ignore SDIFMatrix(const SdifDataTypeET);
//%ignore SDIFMatrix(const SDIFMatrix&);

// have to ignore constructor, because SdifSignature and
// SdifDataTypeET are indistinguishably int for swig
%ignore SDIFMatrix(SdifSignature, int, int, SdifDataTypeET);
//%ignore CreateMatrixData(SdifSignature, int, int, SdifDataTypeET);
%ignore CreateMatrixData(std::string &, int, int, SdifDataTypeET);

// define only the needed basic SDIF stuff from sdif.h
%include sdifdefines.i

// read all class definitions
%include "../easdif/sdifmatrix.h"
%include "../easdif/sdifnamevaluetable.h"
%include "../easdif/sdifframe.h"
%include "../easdif/sdifentity.h"
