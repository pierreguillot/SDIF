/* $Id: writesdif-subs.c,v 1.1 2000-05-12 16:14:12 tisseran Exp $
   writesdif-subs.c     12. May 2000           Patrice Tisserand

   Subroutines for writesdif, function to write an SDIF file.

   All SDIF dependencies are here!

   Usage from matlab (only one input parameters is required):

   For open the file:
   writesdif(filename)
                     Open a sdif file named filename in writeMode.
		     The default sdif description types are used.

   writesdif(filename, types)
                     Open a sdif file named filename in writeMode.
                     The sdif description types are defined in given file types

   writesdif(filename, types, number of parameters)
                     "number of parameters" wil be written in Information Value Table.
		     And jmax sdifplayer will read information table to know how outlets
		     must send evenements.
 
   For write data:
   writesdif(time,streamid,frame signature, matrix signature, matrix data)
                     Write a frame with signature "frame signature" at time "time"
		     with streamId "streamid"
		     Frame contains one matrix with signature "matrix signature"
		     and data "matrix data"

   To close file:
   writesdif('close')
                     Close the sdif file
		     
   $Log: not supported by cvs2svn $

   TODO:
   1/ Assurer l'utilisation de plusieurs matrices passees en arguments de la fonction
   => Boucle sur le nombre de matrices passe en argument.
   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#if defined(__i386)
#include <linux/limits.h>
#else
#include <limits.h>
#endif

#include <sdif/sdif.h>
#include "matrix.h"
#include "writesdif.h"

static void   exitwrite(void);

static int    matricesread    = 0, /* counters for reporting */
              matricesskipped = 0; /* reset by beginwrite */

static int    eof = 1;             /* reset by beginwrite */


SdifFileT *
beginwrite (int nrhs, const mxArray *prhs [], char *filename, char *types)
{
  SdifFileT      *output = NULL;
  char           *fileonly;
  int            index;
  int            nbParam = 2;
  char           nbParamString[3];
  
  /* Sdif library initialisation */
  if (!gSdifInitialised)
    {
      SdifGenInit (types);
      SdifSetExitFunc (exitwrite);
    }

  if (nrhs == 3)
      nbParam = (int) mxGetScalar(prhs[2]);
      
  if ((output = SdifFOpen(filename, eWriteFile)))
    {
      /* Information table chunk creation */
      SdifNameValuesLNewTable(output->NameValues, _SdifNVTStreamID);
      SdifNameValuesLPutCurrNVT(output->NameValues, "WrittenBy","MatlabSdifWriter");

      /* Now we must add number of parameters in sdif file 
	 for use with jmax sdifplayer */
      sprintf(nbParamString,"%d",nbParam);
      SdifNameValuesLPutCurrNVT(output->NameValues,"NumberOfParameters",nbParamString);

      /* Write general header and ascii chunks */
      SdifFWriteGeneralHeader(output);
      SdifFWriteAllASCIIChunks(output);
    }

  return(output);
}

void 
endwrite (SdifFileT *output)
{
  if (!output)
    mexErrMsgTxt("You must open a file before");
  SdifFClose(output);
}

/* writeframe(time, streamid, framesize, matrixsig1, data1, matrxisig2, data2 ...) */

int
writeframe (int nrhs, const mxArray *prhs[], SdifFileT *output)
{
  size_t         SizeFrameW = 0;
  size_t         SizeMatrixW = 0;
  mxArray        *mxarray[MaxNumOut];
  int            m;
  int            i; /* Loop indice for various number of matrix */
  int            indice; /* Matrix position in argument */
  int            ArgNumber; /* Number of arguments */
  SdifUInt4      NbMatrix;
  int            nbMatrix;
  SdifFloat8     SdifTime;
  double         time;
  SdifUInt4      SdifStreamId;
  double         streamId;
  SdifSignature  FrameSignature;
  SdifSignature  MatrixSignature;  
  char           signature [PATH_MAX] = "";
  SdifUInt4      fsz;
  int            NbRow;
  int            NbCol;
  SdifDataTypeET DataType;
  double         *real_data_ptr;
  double         CurrentData;
  int            c, total_num_of_elements;
  
  ArgNumber = nrhs; /* We affect number of function arguments */
  
  /* Get frame time */
  if (!mxIsDouble(prhs[0]))
    mexErrMsgTxt(" Time is not in double format");
  
  time = mxGetScalar(prhs[0]);
  SdifTime = (SdifFloat8) time;

  /* Get stream id */
  if (!mxIsDouble(prhs[1]))
    mexErrMsgTxt(" StreamId is not in double format");

  streamId = mxGetScalar(prhs[1]);
  SdifStreamId = (SdifUInt4) streamId;
  
  /* Get frame signature */
  if (!mxIsChar(prhs[2]))
    mexErrMsgTxt(" Frame signature must be a string");
  
  mxGetString(prhs[2], signature, PATH_MAX);
  fprintf(stderr,"Frame signature:%s\n",signature);
  FrameSignature = SdifStringToSignature(signature);

  /* Size of frame header to use with SdifFSetCurrFrameHeader */
  fsz = SdifSizeOfFrameHeader();

  nbMatrix = (int) ((ArgNumber - 3 )/2); /* arguments are time, streamid, signature,
					    matrix(i), signature(i) */


  /* Set Matrix signature */
  /*
  if (!mxIsChar(prhs[3]))
    mexErrMsgTxt("Matrix signature must be a string");
  mxGetString(prhs[3], signature, PATH_MAX);
  mexPrintf("Matrix Signature:%s\n",signature);
  MatrixSignature = SdifStringToSignature(signature);
  */
  mexPrintf("NbMatrix = %d \n",nbMatrix);
  
  /* Matrix are in argument position: 4+2k with k from 0 to nbMatrix */
  for (i = 0; i < nbMatrix ;i++)
    {
      /* We must know number of row and number of columns of each matrix */
      /* Affect number of rows */
      indice = 4 + (2 * i);
      NbRow = mxGetM(prhs[indice]);
      /* Affect number of columns */
      NbCol = mxGetN(prhs[indice]);
      /* DataType */
      DataType = eFloat4;
      
    fsz += SdifSizeOfMatrix (DataType, NbRow, NbCol);
    mexPrintf("Frame Size=%d\n",fsz);
    }

  /* Now we set frame header */
  NbMatrix = (SdifUInt4) nbMatrix;
  SdifFSetCurrFrameHeader(output, FrameSignature, fsz, NbMatrix, streamId, SdifTime);

  /* Write frame header */
  SizeFrameW = SdifFWriteFrameHeader(output);
  
  /* MATRIX */
  /* Call The writeMatrix function */
  for (i = 0; i < nbMatrix; i++)
    {
      indice = 3 + (2 * i);
      mexPrintf("Matrix Write Loop\n");
      writeMatrix(nrhs, prhs, output, indice);
    }

  return(1);
}

void
writeMatrix(int nrhs, const mxArray* prhs[], SdifFileT *output, int indice)
{
  /* This function set Matrix Header, and write Matrix Data */
  int            i, m; /* Loop indice */
  int            NbRow, NbCol;
  SdifSignature  MatrixSignature;
  char           signature [PATH_MAX] = "";
  size_t         SizeMatrixW          = 0;
  size_t         SizeFrameW           = 0;
  double         *matlabMatrixPtr;
  double         currentData;
  SdifDataTypeET DataType;
  
  /* MATRIX */
  DataType = eFloat4;

  mexPrintf("Argument indice:%d\n",indice);
  
  /* Set Number of col and number of row */
  NbRow = mxGetM(prhs[indice + 1]);
  NbCol = mxGetN(prhs[indice + 1]);
  mexPrintf("NbRow:%d, NbCol:%d\n",NbRow,NbCol);

  /* Get Matrix Signature */
  mxGetString(prhs[indice], signature, PATH_MAX);
  mexPrintf("Signature :%s\n",signature);
  MatrixSignature = SdifStringToSignature(signature);
  
  /* Set Matrix header */
  SdifFSetCurrMatrixHeader(output, MatrixSignature, DataType, NbRow, NbCol);
  SizeMatrixW += SdifFWriteMatrixHeader(output);
  mexPrintf("Matrix Header Set\n");
  /* Write each row */
  matlabMatrixPtr = (double *)mxGetPr(prhs[indice + 1]);
  mexPrintf("Current Value :%g\n",*matlabMatrixPtr);
  
  for(i = 0; i < NbRow; i++)
    /* ROW */
    {
      /* COL */
      for( m = 0; m < NbCol; m++)
	{
	  currentData = matlabMatrixPtr[(m*NbRow)+i];
	  SdifFSetCurrOneRowCol(output, (m + 1), (SdifFloat8) currentData);
	}
      SizeMatrixW += SdifFWriteOneRow(output);


      SizeFrameW += SizeMatrixW;
    }
      SizeMatrixW += SdifFWritePadding(output, SdifFPaddingCalculate(output->Stream,
								     SizeMatrixW));
}

static void
exitwrite (void)
{
  mexErrMsgTxt ("SDIF error: exiting writesdif!");
}
