/* $Id: SdifHighLevel.h,v 3.1 2000-03-01 11:20:21 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
LIBRARY
 *
 * SdifHighLevel.h	8.12.1999	Diemo Schwarz
 *
LOG
 * $Log: not supported by cvs2svn $
 */


#ifndef _SdifHighLevel_H_
#define _SdifHighLevel_H_


#include "SdifGlobals.h"
#include "SdifFileStruct.h"



#if 0	/* TBI */

/*
//FUNCTION GROUP: to be implemented / TBI
*/


/*DOC: 
  Write whole matrix, given as separate columns in array "columns" of
  pointer to "DataType".  Each columns [i], i = 0..NbCol-1, points to 
  NbRow * SdifSizeofDataType (DataType) bytes.  
  TBI 
*/
SdifFWriteMatrixColumns (SdifFileT     *file,
			 SdifSignature  Signature,
			 SdifDataTypeET DataType,
			 SdifUInt4      NbRow,
			 SdifUInt4      NbCol,
			 void	       *columns []);


SdifFWriteTextMatrix (SdifFileT f, SdifSignature matrixsig, const char *str);
{
    /* convert to UTF-8 */
    SdifConvertToUTF8 (str, utfstr);
    nbytes = strlen (utfstr);
    SdifFSetCurrMatrixHeader (f, matrixsig, eText, nbytes, 1);
    SdifFWriteMatrixHeader (f);

    /* write text data and padding */
}


/*DOC:
  Definition of the matrix callback function type, used for SdifReadSimple. 
  TBI 
*/
typedef int (*SdifMatrixCallbackT) (SdifFileT *file, 
				    int nummatrix, 
				    void *userdata);

/*DOC: 
  Reads an entire SDIF file, calling matrixfunc for each matrix in the
  SDIF selection taken from the filename.  Matrixfunc is called with
  the SDIF file pointer, the matrix count within the current frame,
  and the userdata unchanged. 
  TBI 
*/
SdifReadSimple (char		    *filename, 
		SdifMatrixCallbackT matrixfunc,
		void		    *userdata);


/* see SdifFRead */

SdifFReadMatrixAs_TYPE_ ();

#if 0
/* Alternative: extend SdifDataTypeET by */
  eTranslate = 0x1000, /* flag to be or'ed with data type: Translate on read */
  eTypeMask  = 0x0fff  /* mask to eliminate flags */
#endif

SdifFReadMatrixAs (SdifFileT *file, void *target,
		   SdifDataTypeET as_type, int translate);

void *SdifGetColumn ();

#endif /* TBI */

#endif /* _SdifHighLevel_H_ */
