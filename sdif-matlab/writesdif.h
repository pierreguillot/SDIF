#include <mex.h>

#ifndef _SdifFileStruct_
typedef void SdifFileT;
#endif

/* #define MaxNumOut 5 */

extern int matrixcount;
SdifFileT *beginwrite (int nrhs, const mxArray *prhs [], char *filename, char *types);
int	   writeframe (int nrhs, const mxArray *prhs [], SdifFileT *input);
void       writeMatrix(int nrhs, const mxArray *prhs [], SdifFileT *input, int indice);
void	   endwrite   (SdifFileT *input);
