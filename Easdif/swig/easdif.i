// $Id: easdif.i,v 1.4 2003-04-17 11:15:01 schwarz Exp $ -*-c-*-
//
// easdif.i		16.04.2003		Diemo Schwarz
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
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
//

// generate perl module named 'SDIF'
%module SDIF


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


// read all basic SDIF definitions (too much?)
%include "../SDIF/include/sdif.h"

// read all class definitions
%include "../easdif/sdifmatrix.h"
%include "../easdif/sdifnamevaluetable.h"
%include "../easdif/sdifframe.h"
%include "../easdif/sdifentity.h"

