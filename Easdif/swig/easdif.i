// $Id: easdif.i,v 1.1 2003-04-17 09:51:10 schwarz Exp $ -*-c-*-
//
// easdif.i		16.04.2003		Diemo Schwarz
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $

// generate perl module named 'SDIF'
%module SDIF
%{
#include "easdif/easdif.h"

using namespace Easdif;
%}

// module initialisation code
%init 
%{
    EasdifInit();
%}

class SDIFFrame
{
};

class SDIFEntity
{
 public:
    bool OpenRead(char *filename);
    int eof();
    int ReadNextFrame(SDIFFrame& frame);
};
