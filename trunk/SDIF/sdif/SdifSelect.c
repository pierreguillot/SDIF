/* $Id: SdifSelect.c,v 3.1 1999-08-31 10:02:59 schwarz Exp $
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



/* include */		 /* to get these definitions: */
#include <stdio.h>
#include <string.h>	 /* string functions */
#include <macros.h>	 /* min/max */
#include <libgen.h>	 /* basename */
#include <assert.h>	 /* N.B. that assert() calls will vanish with NDEBUG */

#include "SdifHard_OS.h" /* SdifSignature */
#include "SdifGlobals.h" /* eEmptySignature */
#include "SdifMemory.h"	 /* SdifMalloc, SdifCalloc */
#include "SdifList.h"	 /* List structs and functions */
#include "SdifSelect.h"	 /*  */




/*
// DATA GROUP:		terminal symbols and character classes for parsing
*/

/* terminal symbols (strings) for SdifSelectTokens, MUST follow that order! */
char *SdifSelectSeparators   [sst_num + 1] = { "::", "#", ":", "/", ".", 
					       "_",  "@", ",", "-", "+", "??"};
int   SdifSelectSeparatorLen [sst_num + 1];
char  sep_first		     [sst_num + 1]; /* first chars of all separators */
char *SdifSelectWhitespace = " \t\r\n";	    /* chars considered as space */
/* todo: build charmap for first and whitespace */




/* 
// FUNCTION GROUP:	Init/Deinit
 */

/* init module, called by SdifGenInit */
int SdifInitSelect (void)
{
    int i;

    for (i = 0; i < sst_num; i++)
    {
	SdifSelectSeparatorLen [i] = strlen (SdifSelectSeparators [i]);
	sep_first [i] = SdifSelectSeparators [i][0];
    }
    /* settings for sst_num */
    SdifSelectSeparatorLen [i] = 0;
    sep_first [i] = 0;
    return (1);
}



/* killer function for SdifKillList: free data */
static void elimselelem (SdifSelectionT *victim)
{
    if (victim->filename)   
	SdifFree (victim->filename);
    SdifFree (victim);
}


int SdifInitSelection (SdifSelectionT *sel, char *filename, int namelen)
{
    if (filename)
    {    /* copy and null-terminate filename */
	if (namelen < 0)
	    namelen = strlen (filename);
	sel->filename = SdifCalloc (char, namelen + 1);
	strncpy (sel->filename, filename, namelen);
	sel->filename [namelen] = 0;
	sel->basename = basename (sel->filename);
    }
    else
	sel->filename = sel->basename = "";

    sel->stream = SdifCreateList (elimselelem);
    sel->frame  = SdifCreateList (elimselelem);
    sel->matrix = SdifCreateList (elimselelem);
    sel->column = SdifCreateList (elimselelem);
    sel->row    = SdifCreateList (elimselelem);
    sel->time   = SdifCreateList (elimselelem);
    
    return (1);
}

int SdifFreeSelection (SdifSelectionT *sel)
{
    if (sel->filename)	free (sel->filename);
    SdifKillList (sel->stream);
    SdifKillList (sel->frame);
    SdifKillList (sel->matrix);
    SdifKillList (sel->column);
    SdifKillList (sel->row);
    SdifKillList (sel->time);
    return (1);
}




/*
// FUNCTION GROUP:	internal parsing functions
*/


/* abbreviations */
#ifndef __alpha
#define symbol(token)	(assert(token < sst_num), \
			 SdifSelectSeparators [token])
#define symlen(token)	(assert(token < sst_num), \
			 SdifSelectSeparatorLen [token])
#else
#define symbol(token)	(SdifSelectSeparators   [token])
#define symlen(token)	(SdifSelectSeparatorLen [token])
#endif


#define gengetnext(name, field) \
SdifSelectGetNext##name (SdifListP list) \
{   SdifSelectElementP elem; \
    if (SdifListIsNext (list)  &&  (elem = SdifListGetNext (list))) \
        return (elem->value.field); \
    return (-1); \
}

int	      gengetnext (Int,	     integer)
double	      gengetnext (Double,    real)
SdifSignature gengetnext (Signature, signature)



static SdifSelectTokens	TOKEN;
static char		*INPUT, *SYMBOL;


static SdifSelectTokens findtoken ()
{
    SdifSelectTokens t = (SdifSelectTokens) 0;
    while (t < sst_num  &&  strncmp (INPUT, symbol(t), symlen(t)) != 0)
	t++;
    return (t);
}


/* todo: sdiferr... */
#define print_error1(msg, arg)	fprintf (stderr, "ERROR: " msg, arg)



/* Sets TOKEN to next token read.  Consumes INPUT, when token found
   (if not, caller has to advance).  Returns true if a token from
   SdifSelectTokens was found. */
static int getnexttoken ()
{
    INPUT += strspn (INPUT, SdifSelectWhitespace);      /* skip space */
    TOKEN  = findtoken ();				/* find token */
    SYMBOL = INPUT;			  /* points to token's symbol */
    INPUT += symlen(TOKEN);	  /* advance INPUT, when token found. */

    fprintf (stderr, 
	     "getnexttoken\tTOKEN %2d %-2s  SYMBOL '%10s'  INPUT '%10s'\n",
	     TOKEN, symbol (TOKEN), SYMBOL, INPUT);
    return (TOKEN < sst_num);
}



/* get<type> functions parse one <type> element (including leading
   whitespace).  They consume the input, i.e. advance INPUT to the
   char after what they've parsed!  (SYMBOL stays on what was parsed,
   for error output.)  They return true on success 
*/

static int getint (SdifSelectValueT *valu)
{
    SYMBOL = INPUT;
    valu->integer = strtol (SYMBOL, &INPUT, 10);
    fprintf (stderr, 
    "getint\t\tTOKEN %2d %-2s  SYMBOL '%10s'  INPUT '%10s'  ret %d  val %d\n",
    TOKEN, symbol (TOKEN), SYMBOL, INPUT, INPUT > SYMBOL, valu->integer);
    return (INPUT > SYMBOL);	/* if pointer has advanced, we have
				   successfully parsed something */
}

static int getreal (SdifSelectValueT *valu)
{
    SYMBOL = INPUT;
    valu->real = strtod (SYMBOL, &INPUT);
    return (INPUT > SYMBOL);	/* if pointer has advanced, we have
				   successfully parsed something */
}

/* read until next separator character
   Later: read only allowed chars for signature, handle "...". */
static int getsig (SdifSelectValueT *valu)
{
    char sigstr [4] = "\0\0\0\0";
    int	 siglen;

    SYMBOL = INPUT;

    /* find first separator character (NULL if string ended) */
    INPUT  = strpbrk (SYMBOL, sep_first);
    siglen = INPUT  ?  INPUT - SYMBOL  :  strlen (SYMBOL);
    if (!INPUT)   INPUT = "";

    strncpy (sigstr, SYMBOL, min (siglen, 4));
    valu->signature = SdifStringToSignature (sigstr);
    fprintf (stderr, "getsig\t\tTOKEN %2d %-2s  SYMBOL '%10s'  INPUT '%10s'  "
	     "ret %d  val '%s'  siglen %d\n",
	     TOKEN, symbol (TOKEN), SYMBOL, INPUT, siglen != 0, 
	     SdifSignatureToString (valu->signature), siglen);
    return (siglen != 0);
}



/* Parse one element's list of values plus range or delta.  
   Return true if ok. */
static int parse (int (*getval) (SdifSelectValueT *valu), SdifListP list)
{
    int		       ret   = 0;	/* being pessimistic */
    SdifSelectElementT *elem = SdifMalloc (SdifSelectElementT);
    elem->rangetype = (SdifSelectTokens) 0;

    if (getval (&elem->value))
    {	
	getnexttoken ();
	switch (TOKEN)
	{
	    case sst_range:
	    case sst_delta:	/* add elem only if range ok */
		ret = elem->rangetype = getval (&elem->range)  
					?  TOKEN  :  (SdifSelectTokens) 0;
		if (ret)
		    SdifListPutTail (list, elem);
	    break;
	
	    case sst_list:	/* add elem and recursively read comma-list */
		SdifListPutTail (list, elem);
		ret = parse (getval, list);
	    break;

	    case sst_num:	/* next token is no separator */
		SdifListPutTail (list, elem);   /* add this elem in any case */
		if (*INPUT)	/* error when not at end of INPUT */
		    print_error1 ("can't parse SDIF selection: "
				  "No separator after value '%s'\n", INPUT);
		else
		    ret = 1;
	    break;

	    default:		/* token for next element parsed */
		SdifListPutTail (list, elem);   /* add this elem and proceed */
		ret = 1;	/* ok: token for next element */
	    break;
	}
    }
    else
	print_error1 ("can't parse SDIF selection: "
		      "Can't read value from '%s'\n", INPUT);

    fprintf (stderr, 
	     "parse\t\tTOKEN %2d %-2s  SYMBOL '%10s'  INPUT '%10s'  ret %d\n",
	     TOKEN, symbol (TOKEN), SYMBOL, INPUT, ret);

    SdifListInitLoop (list);
    return (ret);
}




/*
// FUNCTION GROUP:	externally callable functions
*/

/* parse elements #stream :frame /matrix .column _row @time in any order */
int SdifParseSelection (SdifSelectionT *sel, char *str)
{
    if (INPUT = str)
	getnexttoken ();
    while (INPUT  &&  *INPUT)
    {
	switch (TOKEN)
	{
	    case sst_stream:	parse (getint,  sel->stream);	break;
	    case sst_frame:	parse (getsig,  sel->frame);	break;
	    case sst_matrix:	parse (getsig,  sel->matrix);	break;
/*	    case sst_column:	parse (getcol,  sel->column);	break;*/
	    case sst_row:	parse (getint,  sel->row);	break;
	    case sst_time:	parse (getreal, sel->time);	break;
	    default:
		if (*INPUT) /* special case: if frame is first element, don't
			       need to use symbol : to avoid ":::1FRM" */
		    parse (getsig, sel->frame);
	    break;
	}
    }
    return (1);
}



char *SdifGetFilenameAndSelection (/*in*/  char *filename, 
				   /*out*/ SdifSelectionT *sel)
{
    char *spec;

    /* find spec */
    spec = filename  ?  strstr (filename, symbol(sst_specsep))  :  NULL;
    SdifInitSelection  (sel, filename, spec - filename);
    if (spec)
	SdifParseSelection (sel, spec + symlen (sst_specsep));

    return (sel->filename);
}
