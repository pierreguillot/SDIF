// $Id: sdif.i,v 1.2 2003-04-18 17:45:23 schwarz Exp $ -*-c-*-
//
// sdif.i		18.04.2003		Diemo Schwarz
//
// Interface file for swig, defining the callable sdif library functions
//
// $Log: not supported by cvs2svn $
// Revision 1.1  2003/04/18 16:00:22  schwarz
// Generate separate large module SDIF.pm with all
// low-level sdif lib wrappers.
//


// generate perl module named 'SDIF'
%module SDIF

// include typemaps
%include sdiftypemap.i

// include header to make wrapper compile
%{
#include "../SDIF/include/sdif.h"
%}


// read all SDIF definitions
%include "../SDIF/include/sdif.h"
