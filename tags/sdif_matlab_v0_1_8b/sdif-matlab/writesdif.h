/* $Id: writesdif.h,v 1.4 2000-12-19 16:44:12 roebel Exp $

   writesdif.h		12. May 2000	Patrice Tisserand

   Declarations for writesdif.c, writesdif-subs.c
  
   $Log: not supported by cvs2svn $
   Revision 1.3  2000/08/04 14:42:35  schwarz
   Added reset of file variable, prevents crash on double-close.
   Version number is written in NVTs, and is used for distribution,
   defined in makefile (major.minor.release).
   Types file now included in distribution and documentation.

*/

#include <mex.h>
#include <sdif.h>



/* #define MaxNumOut 5 */

extern int matrixcount;
SdifFileT *beginwrite (int nrhs, const mxArray *prhs [], char *filename, char *types);
int	   writeframe (int nrhs, const mxArray *prhs [], SdifFileT *input);
void       writeMatrix(int nrhs, const mxArray *prhs [], SdifFileT *input, int indice);
SdifFileT *endwrite   (SdifFileT *input);
