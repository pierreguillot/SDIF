/* $Id: writesdif-subs.c,v 1.8 2003-04-22 16:08:55 roebel Exp $
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
   Revision 1.7  2001/05/14 17:36:31  roebel
   Added support for writing char and 1NVT matrice data!

   Revision 1.6  2001/04/20 14:37:48  roebel
   Changed VERSION macro to VERS because VERSIOn is used somewhere in
   mexversion.c.

   Revision 1.5  2000/12/19 16:44:11  roebel
   Fixed Bug in loadsdif - crash when last matrix was not selected
   Moved test file sequence4seg1.energy.sdif into repository
   Corrected sgi mex extension to mexsg such that sgi is now in
   distribution

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
beginwrite (int nrhs, const mxArray *prhs [], char *filename, 
	    char *types, const char * _localtypes)
{
  SdifFileT      *output = NULL;
  int            nbParam = 2;
  char           nbParamString[3];
  SdifStringT *pLocaltypes = 0;
  
  /* Sdif library initialisation */
  if (!gSdifInitialised)
    {
      SdifGenInit (types);
      SdifSetExitFunc (exitwrite);
    }

  if (nrhs == 3) {
    if(_localtypes[0] == '\000')
      nbParam = (int) mxGetScalar(prhs[2]);
    else {
      pLocaltypes = SdifStringNew();
      SdifStringAppend(pLocaltypes , _localtypes );
    }
      
  }

  if ((output = SdifFOpen(filename, eWriteFile)))
    {
      /* Information table chunk creation */
      SdifNameValuesLNewTable(output->NameValues, _SdifNVTStreamID);
      SdifNameValuesLPutCurrNVT(output->NameValues, 
				"WrittenBy", "MatlabSdifWriter");
      SdifNameValuesLPutCurrNVT(output->NameValues, 
				"Version",  VERS);

      /* Now we must add number of parameters in sdif file 
	 for use with jmax sdifplayer */
      sprintf(nbParamString,"%d",nbParam);
      SdifNameValuesLPutCurrNVT(output->NameValues,
				"NumberOfParameters", nbParamString);

      if(pLocaltypes && pLocaltypes->TotalSize > 0 ) {
	SdifFGetAllTypefromSdifString(output, pLocaltypes);
      }
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
  SdifSignature  MatrixSignature;
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

      int indiceMatSig = 3 + (2 * i);
      /* Get Matrix Signature */
      mxGetString(prhs[indiceMatSig], signature, PATH_MAX);
      MatrixSignature = SdifStringToSignature(signature);
      DataType = eText;

      if (  MatrixSignature == SdifSignatureConst('1','N','V','T')){

	indice = 4 + (2 * i);
	if(!mxIsCell(prhs[indice])){
	  mexErrMsgTxt ("1NVT matrices have to be supplied as cell arrays!");
	}
	else	  {
	  int  icell,cntEl =0;
	  NbRow = mxGetM(prhs[indice]);
	  NbCol = mxGetN(prhs[indice]);

	  if(NbCol != 2){
	    mexErrMsgTxt ("1NVT cell arrays are n-rows/2 columns!");
	  }
	  for(icell=0;icell<NbRow*NbCol;++icell){
	    const mxArray *tmp = mxGetCell(prhs[indice], icell); 

	    /* For each name or value a \t or \n will be added
	     * to conform to the 1NVT matrix definition */ 
	    cntEl += mxGetNumberOfElements(tmp) +1;
	  }
	  fsz += SdifSizeOfMatrix (DataType, cntEl,1);

	}  

      }
      else{
	/* We must know number of row and number of columns of each matrix */
	/* Affect number of rows */
	indice = 4 + (2 * i);
	NbRow = mxGetM(prhs[indice]);
	/* Affect number of columns */
	NbCol = mxGetN(prhs[indice]);
	/* DataType */
	if (mxIsDouble (prhs[indice]))
	  {
	    DataType = eFloat4;
	  }
	else if (mxIsChar (prhs[indice]))
	  DataType = eText;
      
	fsz += SdifSizeOfMatrix (DataType, NbRow, NbCol);
      }
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
  SdifDataTypeET DataType;
  char           *CharCpy=0;

  /* Get Matrix Signature */
  mxGetString(prhs[indice], signature, PATH_MAX);
  MatrixSignature = SdifStringToSignature(signature);

  if(MatrixSignature == SdifSignatureConst('1','N','V','T')){

    int irow,icell;
    int mNbRow = mxGetM(prhs[indice+1]);
    int mNbCol = mxGetN(prhs[indice+1]);

    DataType = eText;

    NbRow = 0;
    for(icell=0;icell<mNbRow*mNbCol;++icell){
      const mxArray *tmp = mxGetCell(prhs[indice+1], icell); 

      /* For each name or value a \t or \n will be added
       * to conform to the 1NVT matrix definition */ 
      NbRow += mxGetNumberOfElements(tmp)+1;    
    }  


    /* Set Matrix header */
    SdifFSetCurrMatrixHeader(output, MatrixSignature, DataType, NbRow, 1);
    SizeMatrixW += SdifFWriteMatrixHeader(output);


    for(irow=0,icell=0;irow<mNbRow;++irow, icell++){
      char          *matlabMatrixPtr; /* Pointer on matrix first element */
      double         currentData;      /* Current element of matrix */

      mxArray *tmp;
      tmp = mxGetCell(prhs[indice+1], icell); 

      NbRow = mxGetNumberOfElements(tmp);
      NbCol = 1;
      
      matlabMatrixPtr = mxArrayToString(tmp);  
      
      for(i = 0; i < NbRow; i++)
	/* ROW LOOP */
	{
	  /* double is used as a container for all data types */
	  currentData = matlabMatrixPtr[i];
	  SdifFSetCurrOneRowCol(output, 1, (SdifFloat8) currentData);
	  SizeMatrixW += SdifFWriteOneRow(output);
	}

      /* We have wrote all data of matrix */
      mxFree(matlabMatrixPtr);

      /* Add a tab after the name entry */
      currentData = '\t';
      SdifFSetCurrOneRowCol(output, 1, (SdifFloat8) currentData);
      SizeMatrixW += SdifFWriteOneRow(output);

      tmp = mxGetCell(prhs[indice+1], icell+mNbRow); 

      NbRow = mxGetNumberOfElements(tmp);
      NbCol = 1;
      
      matlabMatrixPtr = mxArrayToString(tmp);  
      
      for(i = 0; i < NbRow; i++)
	/* ROW LOOP */
	{
	  /* double is used as a container for all data types */
	  currentData = matlabMatrixPtr[i];
	  SdifFSetCurrOneRowCol(output,  1, (SdifFloat8) currentData);
	  SizeMatrixW += SdifFWriteOneRow(output);
	}

      /* We have wrote all data of matrix */
      mxFree(matlabMatrixPtr);

      /* Add a newline after the value entry */
      currentData = '\n';
      SdifFSetCurrOneRowCol(output, 1, (SdifFloat8) currentData);
      SizeMatrixW += SdifFWriteOneRow(output);      
    }
  }
  else{
  
    /* set matrix data type, number of col and number of row */
    if (mxIsDouble (prhs[indice + 1]))
      {
	DataType = eFloat4;
	NbRow = mxGetM(prhs[indice + 1]);
	NbCol = mxGetN(prhs[indice + 1]);
      }
    else if (mxIsChar (prhs[indice + 1]))
      {   /* text is written as one column */
	DataType = eText;
	NbRow = mxGetM(prhs[indice + 1]) * mxGetN(prhs[indice + 1]);
	NbCol = 1;
      }
    else
      mexErrMsgTxt ("Matrix data type must be double or char");
    
    
    /* Set Matrix header */
    SdifFSetCurrMatrixHeader(output, MatrixSignature, DataType, NbRow, NbCol);
    SizeMatrixW += SdifFWriteMatrixHeader(output);
    
    /* Write each row */
    switch (DataType) {
    case eFloat4:{
      double         *matlabMatrixPtr; /* Pointer on matrix first element */
      double         currentData;      /* Current element of matrix */
      
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
    }
    break;
    
    case eText:{
      char          *matlabMatrixPtr; /* Pointer on matrix first element */
      double         currentData;      /* Current element of matrix */
      
      matlabMatrixPtr = mxArrayToString(prhs[indice + 1]);  
      
      for(i = 0; i < NbRow; i++)
	/* ROW LOOP */
	{
	  /* COL LOOP */
	  for( m = 0; m < NbCol; m++)
	    {
	      /* double is used as a container fo rall data types */
	      currentData = matlabMatrixPtr[(m*NbRow)+i];
	      SdifFSetCurrOneRowCol(output, (m + 1), (SdifFloat8) currentData);
	    }
	  SizeMatrixW += SdifFWriteOneRow(output);
	}
      /* We have wrote all data of matrix */
      mxFree(matlabMatrixPtr);
    }
    break;
    }
  }

  SizeMatrixW += SdifFWritePadding(output, SdifFPaddingCalculate(output->Stream,
								   SizeMatrixW));
}

static void
exitwrite (void)
{
  mexErrMsgTxt ("SDIF error: exiting writesdif!");
}




