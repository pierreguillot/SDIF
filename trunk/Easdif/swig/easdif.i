// $Id: easdif.i,v 1.2 2003-04-17 10:19:51 schwarz Exp $ -*-c-*-
//
// easdif.i		16.04.2003		Diemo Schwarz
//
// Interface file for swig, defining the callable easdif functions
//
// $Log: not supported by cvs2svn $
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


// change class names because we prefix with SDIF:: anyway
%name(Frame) 
class SDIFFrame
{
};

%name(Entity)
class SDIFEntity
{
 public:
    bool OpenRead(char *filename);
    int eof();
    int ReadNextFrame(SDIFFrame& frame);
};
