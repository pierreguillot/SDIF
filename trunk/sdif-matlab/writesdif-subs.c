/* $Id: writesdif-subs.c,v 1.5 2000-12-19 16:44:11 roebel Exp $
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
   Revision 1.4  2000/08/04 14:42:33  schwarz
   Added reset of file variable, prevents crash on double-close.
   Version number is written in NVTs, and is used for distribution,
   defined in makefile (major.minor.release).
   Types file now included in distribution and documentation.

 * Revision 1.3  2000/07/19  16:32:28  schwarz
 * First attempt for writing text matrices doesn't work.
 *
 * Revision 1.2  2000/05/15  13:07:46  tisseran
 * Added test for input arguments:
 *    Is Matrix Signature a char?
 * Removed debugging message (mexPrintf("Matrix Loop") ...)
 * Removed unused variable
 * Change comment style for writesdif.m
 *
 * Revision 1.1  2000/05/12  16:14:12  tisseran
 * Mexfile to write sdif files in matlab.
 * TODO: add possibility to use several file at same time.
 *       add test on arguments
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>	/* For alpha bzero */
#include <limits.h>	/* For definition of PATH_MAX */
#include "writesdif.h"
#include "matrix.h"

static void   exitwrite(void);

SdifFileT *
beginwrite (int nrhs, const mxArray *prhs [], char *filename, char *types)
{
  SdifFileT      *output = NULL;
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
      SdifNameValuesLPutCurrNVT(output->NameValues, 
				"WrittenBy", "MatlabSdifWriter");
      SdifNameValuesLPutCurrNVT(output->NameValues, 
				"Version",   VERSION);

      /* Now we must add number of parameters in sdif file 
	 for use with jmax sdifplayer */
      sprintf(nbParamString,"%d",nbParam);
      SdifNameValuesLPutCurrNVT(output->NameValues,
				"NumberOfParameters", nbParamString);

      /* Write general header and ascii chunks */
      SdifFWriteGeneralHeader(output);
      SdifFWriteAllASCIIChunks(output);
    }

  return(output);
}

SdifFileT *endwrite (SdifFileT *output)
{
  if (!output)
    mexErrMsgTxt("You must open a file before");
  SdifFClose(output);
  return NULL;
}

/* writeframe(time, streamid, framesize, matrixsig1, data1, matrxisig2, data2 ...) */

int
writeframe (int nrhs, const mxArray *prhs[], SdifFileT *output)
{
  size_t         SizeFrameW = 0;
  int            i; /* Loop indice for various number of matrix */
  int            indice; /* Matrix position in argument */
  SdifUInt4      NbMatrix;
  int            nbMatrix;
  SdifFloat8     SdifTime;
  double         time;
  SdifUInt4      SdifStreamId;
  double         streamId;
  SdifSignature  FrameSignature;
  char           signature [PATH_MAX] = "";
  SdifUInt4      fsz;
  int            NbRow;
  int            NbCol;
  SdifDataTypeET DataType;
  
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
  FrameSignature = SdifStringToSignature(signature);

  /* Size of frame header to use with SdifFSetCurrFrameHeader */
  fsz = SdifSizeOfFrameHeader();

  nbMatrix = (int) ((nrhs - 3 )/2); /* arguments are time, streamid, signature,
					    matrix(i), signature(i) */


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
      writeMatrix(nrhs, prhs, output, indice);
    }

  return(1);
}

void
writeMatrix(int nrhs, const mxArray* prhs[], SdifFileT *output, int indice)
{
  /* This function set Matrix Header, and write Matrix Data */
  int            i, m; /* Loop indice */
  int            NbRow, NbCol; /* Number of row and number of col */
  SdifSignature  MatrixSignature; 
  char           signature [PATH_MAX] = "";
  size_t         SizeMatrixW          = 0; /* Size of Matrix use for padding */
  size_t         SizeFrameW           = 0;
  double         *matlabMatrixPtr; /* Pointer on matrix first element */
  double         currentData;      /* Current element of matrix */
  SdifDataTypeET DataType;
  
  /* set matrix data type, number of col and number of row */
  if (mxIsDouble (prhs[indice + 1]))
  {
      DataType = eFloat4;
      NbRow = mxGetM(prhs[indice + 1]);
      NbCol = mxGetN(prhs[indice + 1]);
  }
#if not_yet
  else if (mxIsChar (prhs[indice + 1]))
  {   /* text is written as one column */
      DataType = eText;
      NbRow = mxGetM(prhs[indice + 1]) * mxGetN(prhs[indice + 1]);
      NbCol = 1;
  }
#endif
  else
      mexErrMsgTxt ("Matrix data type must be double or char");

  /* Get Matrix Signature */
  mxGetString(prhs[indice], signature, PATH_MAX);
  MatrixSignature = SdifStringToSignature(signature);
  
  /* Set Matrix header */
  SdifFSetCurrMatrixHeader(output, MatrixSignature, DataType, NbRow, NbCol);
  SizeMatrixW += SdifFWriteMatrixHeader(output);

  /* Write each row */
  matlabMatrixPtr = (double *)mxGetPr(prhs[indice + 1]);
  
  for(i = 0; i < NbRow; i++)
    /* ROW LOOP */
    {
      /* COL LOOP */
      for( m = 0; m < NbCol; m++)
	{
	  currentData = matlabMatrixPtr[(m*NbRow)+i];
	  SdifFSetCurrOneRowCol(output, (m + 1), (SdifFloat8) currentData);
	}
      SizeMatrixW += SdifFWriteOneRow(output);
      SizeFrameW += SizeMatrixW;
    }
  /* We have wrote all data of matrix */
  SizeMatrixW += SdifFWritePadding(output, SdifFPaddingCalculate(output->Stream,
								     SizeMatrixW));
}

static void
exitwrite (void)
{
  mexErrMsgTxt ("SDIF error: exiting writesdif!");
}




