/* $Id: writesdif.h,v 1.3 2000-08-04 14:42:35 schwarz Exp $

   writesdif.h		12. May 2000	Patrice Tisserand

   Declarations for writesdif.c, writesdif-subs.c
  
   $Log: not supported by cvs2svn $
*/

#include <mex.h>

#ifndef _SdifFileStruct_
typedef void SdifFileT;
#endif

/* #define MaxNumOut 5 */

extern int matrixcount;
SdifFileT *beginwrite (int nrhs, const mxArray *prhs [], char *filename, char *types);
int	   writeframe (int nrhs, const mxArray *prhs [], SdifFileT *input);
void       writeMatrix(int nrhs, const mxArray *prhs [], SdifFileT *input, int indice);
SdifFileT *endwrite   (SdifFileT *input);
