/* $Id: SdifSelect.h,v 3.1 1999-08-31 10:03:00 schwarz Exp $
 *
 *               Copyright (c) 1998 by IRCAM - Centre Pompidou
 *                          All rights reserved.
 *
 *  For any information regarding this and other IRCAM software, please
 *  send email to:
 *                            manager@ircam.fr
 *
 *
 * SdifSelect.c		27. August 1999		Diemo Schwarz
 */

/*

DESCRIPTION

  Standardize access path to a selection of data from an SDIF file.

  I repeat my suggestion for the syntax of the select specification ,
  which I sent to Matt on June 21:
  
  I think it's a good idea to include the filename in the selection.
  This would encourage programmers to use some parsing procedure
  SdifGetFilenameAndSelection, which splits a command-line argument up
  into the filename and the selection, thus transparently allowing SDIF
  readers to handle a specific part of an SDIF file only.
  
  For various problems with filename and shell metacharacters, I suggest
  to mark the start of the select spec with '::' (or '##' or '--').
  This way, there is no ambiguity with filenames containing the
  select-spec characters, and to specify a filename containing '::'
  itself, you just have to append '::'.
  
  The syntax for the select-spec would then be:
  
  [filename][::[[#stream]:][frame][/matrix][.column][_row][@time]]
  
  With these mnemonics:
  - #stream as with "number", 
  - frame/matrix as in a file system hierarchy,
  - .column as in a C-struct (with column name or number) 
    Watch out: "@1.2" will be parsed as time 1.2 seconds, not time 1 column 2!
    (use ".2@1" for that)
  - _row as in a LaTeX index.  
  (As .column_row may seem unnatural for people with a maths background
  these can also be given in inverse order as _row.column, which still 
  looks good.)
  - @time like in "at time ..."
  - @time+delta can have delta < 0, so we have "at time plus/minus delta"
  
  - Streams and columns can be given either as a number or as a name.  
  
  - All specifications can be comma-separated lists of values.
  Numerical specifications can also be ranges l-u (a missing value l or u 
  would be replaced by the respective min/max).
  
  - Whitespace between the specs should be allowed.
  
  - Allow quoting names for weird frame and matrix types.
  
  - One could later allow regular expressions for names. 

LOG
  $Log: not supported by cvs2svn $

*/


#ifndef _SdifSelect_
#define _SdifSelect_


/* tokens (numerical ids) for sdifspec separators */
typedef enum { sst_specsep, sst_stream, sst_frame, sst_matrix, sst_column, 
	       sst_row,     sst_time,   sst_list,  sst_range,  sst_delta,
	       sst_num	/* number of tokens */
} SdifSelectTokens;


/*DOC: 
  one value of different possible types in a selection element
  (the element specifies which type is actually used).  
*/
typedef union SdifSelectValueS 
{
    int		   integer;
    double	   real;
    SdifSignature  signature;
} SdifSelectValueT;


/*DOC: 
  selection element: one basic data element, 
  with optional range (this is lower, range is upper bound) 
 */
typedef struct SdifSelectElementS
{
    SdifSelectValueT value;
    SdifSelectValueT range;
    SdifSelectTokens rangetype;	/* 0 for not present, sst_range, sst_delta */
} SdifSelectElementT, *SdifSelectElementP;


/*DOC: */
typedef struct
{
    char	*filename,	/* allocated / freed by 
				   SdifInitSelection / SdifFreeSelection */
		*basename;	/* points into filename */
    SdifListP	stream, frame, matrix, column, row, time;
} SdifSelectionT;




/* init module, called by SdifGenInit */
int SdifInitSelect (void);

int SdifInitSelection (SdifSelectionT *sel, char *filename, int namelen);
int SdifFreeSelection (SdifSelectionT *sel);

char *SdifGetFilenameAndSelection (/*in*/  char *filename, 
				   /*out*/ SdifSelectionT *sel);

int		SdifSelectGetNextInt (SdifListP list);
double		SdifSelectGetNextDouble (SdifListP list);
SdifSignature	SdifSelectGetNextSignature (SdifListP list);



#endif /* _SdifSelect_ */
