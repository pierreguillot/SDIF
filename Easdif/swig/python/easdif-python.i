// $Id: easdif-python.i,v 1.1 2003-04-30 11:45:48 tisseran Exp $ -*-c-*-
//
// easdif-python.i		30.04.2003		Patrice Tisserand
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $

// generate python module named 'eaSDIF'
%module eaSDIF

%init
%{
    EasdifInit();
%}

// include typemaps
%include sdiftypemap-python.i

// define only the needed basic SDIF stuff from sdif.h
%include ../easdif-defines.i

// include common module directive
%include ../easdif-common.i

