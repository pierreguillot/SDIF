/* $Id: loadsdif.h,v 1.3 2000-08-04 14:42:30 schwarz Exp $

   loadsdif.h	25. January 2000	Diemo Schwarz

   Declarations for loadsdif.c, loadsdif-subs.c
  
   $Log: not supported by cvs2svn $
  Revision 1.2  2000/07/19  16:32:09  schwarz
  Updated to new SDIF selection API.
  Proper handling of matrix selection now.

 * Revision 1.1  2000/05/04  13:24:06  schwarz
 * Matlab mex extension and support functions to load SDIF files.
 * 
*/


#include <mex.h>


#ifndef _SdifFileStruct_
typedef void SdifFileT;
#endif


#define MaxNumOut 5

extern int matrixcount;

SdifFileT *beginread (int nlhs, mxArray *plhs [], char *filename, char *types);
int	   readframe (int nlhs, mxArray *plhs [], SdifFileT *input);
SdifFileT *endread   (SdifFileT *input);
