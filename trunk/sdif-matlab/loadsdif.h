/* $Id: loadsdif.h,v 1.1 2000-05-04 13:24:06 schwarz Exp $

   loadsdif.h	25. January 2000	Diemo Schwarz

   Declarations for loadsdif.c
  
   $Log: not supported by cvs2svn $ 
*/


#include <mex.h>


#ifndef _SdifFileStruct_
typedef void SdifFileT;
#endif


#define MaxNumOut    5

extern int matrixcount;

SdifFileT *beginread (int nlhs, mxArray *plhs [], char *filename, char *types);
int	   readframe (int nlhs, mxArray *plhs [], SdifFileT *input);
void	   endread   (SdifFileT *input);
