// $Id: sdif.i,v 1.1 2003-04-18 16:00:22 schwarz Exp $ -*-c-*-
//
// sdif.i		18.04.2003		Diemo Schwarz
//
// Interface file for swig, defining the callable sdif library functions
//
// $Log: not supported by cvs2svn $


// generate perl module named 'SDIF'
%module SDIF

// include typemaps
%include sdifsignature.i

// include header to make wrapper compile
%{
#include "../SDIF/include/sdif.h"
%}


// read all SDIF definitions
%include "../SDIF/include/sdif.h"
