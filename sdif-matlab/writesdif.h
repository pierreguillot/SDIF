/* $Id: writesdif.h,v 1.5 2003-04-22 16:08:58 roebel Exp $

   writesdif.h		12. May 2000	Patrice Tisserand

   Declarations for writesdif.c, writesdif-subs.c
  
   $Log: not supported by cvs2svn $
   Revision 1.4  2000/12/19 16:44:12  roebel
   Fixed Bug in loadsdif - crash when last matrix was not selected
   Moved test file sequence4seg1.energy.sdif into repository
   Corrected sgi mex extension to mexsg such that sgi is now in
   distribution

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
SdifFileT *beginwrite (int nrhs, const mxArray *prhs [], char *filename, 
		       char *types, const char *localtypes);
int	   writeframe (int nrhs, const mxArray *prhs [], SdifFileT *input);
void       writeMatrix(int nrhs, const mxArray *prhs [], SdifFileT *input, int indice);
SdifFileT *endwrite   (SdifFileT *input);
