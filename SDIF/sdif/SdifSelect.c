/* $Id: SdifSelect.c,v 3.7 2000-05-12 14:38:13 schwarz Exp $
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

  - Extension: Use !element to negate selection, e.g. !@0-1 selects
    all outside time range

TODO
  - int SdifSelectFlattenIntList (in list, out array, in nummax)
    write ordered expanded list of int selection 
    (ranges/deltas x-y expanded to x..y)
    return number of selection written to array, not more than nummax.
  - keep signature selections parallely as hash tables for fast lookup
  - optimize selecttestint/real: loop from current 
  - SdifSelectionT: array of select elements
      struct { SdifListP list; SdifSelectElementT minmax; } elem [eSelNum];
    indexed by
      enum   { eTime, eStream, eFrame, eMatrix, eColumn, eRow, eSelNum }
    to use in all API functions instead of SdifListP.

LOG
  $Log: not supported by cvs2svn $
  Revision 3.6  2000/03/01  11:18:45  schwarz
  Added SdifCreateSelection.
  Fixed Linux-only bug in SdifGetFilenameAndSelection (high addresses).
  SdifSelectTestReal first tests from current select element onwards.

  Revision 3.5  1999/10/15  12:21:48  schwarz
  Changed min/max to upper case.
  Test frame takes _SdifAllStreamID into account.

  Revision 3.4  1999/10/07  15:06:42  schwarz
  Added SdifSelectGetFirst<type>, SdifSelectGetNext(Int|Real).

  Revision 3.3  1999/09/28  13:09:11  schwarz
  Included #include <preincluded.h> for cross-platform uniformisation,
  which in turn includes host_architecture.h and SDIF's project_preinclude.h.

  Revision 3.2  1999/09/20  13:23:02  schwarz
  First finished version, API to be improved.

  Revision 3.1  1999/08/31  10:02:59  schwarz
  Added module SdifSelect which parses an access specification to a
  chosen part of SDIF data.  Can be added to a file name.  */



/* include */		 /* to get these definitions: */
#include <preincluded.h>
#include <stdio.h>
#include <string.h>	 /* string functions */
#include <math.h>	 /* fabs */
#include <assert.h>	 /* N.B. that assert() calls will vanish with NDEBUG */

#include "SdifFile.h"	 /* SdifFileT */
#include "SdifRWLowLevel.h" /* SdifSignatureToString */
#include "SdifHard_OS.h" /* SdifSignature */
#include "SdifGlobals.h" /* eEmptySignature, MIN/MAX */
#include "SdifMemory.h"	 /* SdifMalloc, SdifCalloc */
#include "SdifMatrixType.h"
#include "SdifList.h"	 /* List structs and functions */
#include "SdifSelect.h"



static int debug = 0;

char* SdifBaseName(char* inPathFileName);
char* SdifBaseName(char* inPathFileName)
{
	char* retFileName = strrchr(inPathFileName, HOST_DIRECTORY_DIVIDER);
	if(retFileName != NULL)	return (char*)(retFileName + 1);
	else					return inPathFileName;
}

/*
// DATA GROUP:		terminal symbols and character classes for parsing
*/

/* terminal symbols (strings) for SdifSelectTokens, MUST follow that order!
   N.B.: When symbols have common prefixes (e.g. "::" and ":"),
   the longer symbol must come first. */
char *SdifSelectSeparators   [sst_num + 1] = { "::", "#", ":", "/", ".", 
					       "_",  "@", ",", "-", "+", "??"};
int   SdifSelectSeparatorLen [sst_num + 1];
char  sep_first		     [sst_num + 1]; /* first chars of all separators */
char *SdifSelectWhitespace = " \t\r\n";	    /* chars considered as space */
/* todo: build charmap for first and whitespace */


/* static prototypes (to be warning free) */
static SdifSelectTokens findtoken (void); 
static void skipspace (void);
static SdifSelectTokens parsenexttoken (void);

static int parseint	  (SdifSelectValueT *valu);
static int parsereal      (SdifSelectValueT *valu);
static int parsestring	  (void);
static int parsesig	  (SdifSelectValueT *valu);
static int parsecol	  (SdifSelectValueT *valu);
static int parse	  (int (*parseval) (SdifSelectValueT *valu),
			   SdifListP list, int range_allowed, char *name);

static int getint (SdifSelectValueT val);
static double getreal (SdifSelectValueT val);
static SdifSignature getsignature (SdifSelectValueT val);
static char *getstring (SdifSelectValueT val);




/* 
// FUNCTION GROUP:	Init/Deinit
 */

/* init module, called by SdifGenInit */
int 
SdifInitSelect (void)
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



/* killer function for SdifKillList: free one SdifSelectElement */
static void 
elimselelem (SdifSelectionT *victim)
{
    SdifFree (victim);
}


int 
SdifInitSelection (SdifSelectionT *sel, const char *filename, int namelen)
{
    assert (gSdifInitialised  &&  "SDIF library not initialised!");

    if (!filename)
	filename = "";
	
    /* copy and null-terminate filename */
    sel->filename = SdifCreateStrNCpy (filename, namelen + 1);
    sel->filename [namelen] = 0;
    sel->basename = SdifBaseName (sel->filename);

    sel->stream = SdifCreateList (elimselelem);
    sel->frame  = SdifCreateList (elimselelem);
    sel->matrix = SdifCreateList (elimselelem);
    sel->column = SdifCreateList (elimselelem);
    sel->row    = SdifCreateList (elimselelem);
    sel->time   = SdifCreateList (elimselelem);
    
    return (1);
}


SdifSelectionT *
SdifCreateSelection ()
{
    return (SdifMalloc (SdifSelectionT));
}


int 
SdifFreeSelection (SdifSelectionT *sel)
{
    if (sel->filename)	
	SdifFree (sel->filename);
    else
	_SdifError (eFreeNull, "Selection->filename");

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
#define symbol(token)	(assert(token <= sst_num), \
			 SdifSelectSeparators [token])
#define symlen(token)	(assert(token <= sst_num), \
			 SdifSelectSeparatorLen [token])
#else
#define symbol(token)	(SdifSelectSeparators   [token])
#define symlen(token)	(SdifSelectSeparatorLen [token])
#endif


static SdifSelectTokens	TOKEN;
static char		*INPUT, *SYMBOL, *ORIG;


static SdifSelectTokens 
findtoken ()
{
    SdifSelectTokens t = sst_norange;
    while (t < sst_num  &&  strncmp (INPUT, symbol(t), symlen(t)) != 0)
	t++;
    return (t);
}


/* skip space by advancing INPUT to first non-space character */
static void 
skipspace (void)
{
    INPUT += strspn (INPUT, SdifSelectWhitespace);
}


/* Sets TOKEN to next token read.  Consumes INPUT, when token found
   (if not, caller has to advance).  Returns true if a token from
   SdifSelectTokens was found. */
static SdifSelectTokens
parsenexttoken ()
{
    skipspace ();
    TOKEN  = findtoken ();				/* find token */
    SYMBOL = INPUT;			  /* points to token's symbol */
    INPUT += symlen(TOKEN);	  /* advance INPUT, when token found. */

    if (debug)  fprintf (stderr, 
	"parsenexttoken\tTOKEN %2d %-2s  SYMBOL '%10s'  INPUT '%10s'\n",
	TOKEN, symbol (TOKEN), SYMBOL, INPUT);
    return (TOKEN);
}



/* parse<type> functions parse one <type> element (ignoring leading
   whitespace).  They consume the input, i.e. advance INPUT to the
   char after what they've parsed!  (SYMBOL stays on what was parsed,
   for error output.)  They return true on success 
*/

static int 
parseint (SdifSelectValueT *valu)
{
    SYMBOL = INPUT;
    valu->integer = strtol (SYMBOL, &INPUT, 10);
    if (debug)  fprintf (stderr, 
    "parseint\t\tTOKEN %2d %-2s  SYMBOL '%10s'  INPUT '%10s'  ret %d  val %d\n",
    TOKEN, symbol (TOKEN), SYMBOL, INPUT, INPUT > SYMBOL, valu->integer);
    return (INPUT > SYMBOL);	/* if pointer has advanced, we have
				   successfully parsed something */
}

static int 
parsereal (SdifSelectValueT *valu)
{
    SYMBOL = INPUT;
    valu->real = strtod (SYMBOL, &INPUT);
    return (INPUT > SYMBOL);	/* if pointer has advanced, we have
				   successfully parsed something */
}


/* read until next separator character
   Later: handle quoting "..." or '...'
   Set SYMBOL to string read (non-terminated!), return length, advance INPUT.
*/
static int 
parsestring ()
{
    int len;

    skipspace ();	/* advance INPUT to first non-space character */
    SYMBOL = INPUT;

    /* find first separator character (NULL if string ended) */
    INPUT = strpbrk (SYMBOL, sep_first);
    len   = INPUT  ?  INPUT - SYMBOL  :  strlen (SYMBOL);
    if (!INPUT)   INPUT = "";
    return (len);
}


/* read until next separator character
   Later: read only allowed chars for signature, handle "...". */
static int 
parsesig (SdifSelectValueT *valu)
{
    char sigstr [4] = "\0\0\0\0";
    int	 siglen = parsestring ();

    strncpy (sigstr, SYMBOL, MIN (siglen, 4));
    valu->signature = SdifStringToSignature (sigstr);

    if (debug) fprintf (stderr, 
	"parsesig\t\tTOKEN %2d %-2s  SYMBOL '%10s'  INPUT '%10s'  "
	"ret %d  val '%s'  siglen %d\n",
	TOKEN, symbol (TOKEN), SYMBOL, INPUT, siglen != 0, 
	SdifSignatureToString (valu->signature), siglen);
    return (siglen != 0);
}


/* read either column name (later) or number
   Later: handle quotes "...". 
*/
static int 
parsecol (SdifSelectValueT *valu)
{
    if (parseint (valu))	/* see if we can find a number */
	return (1);
    else
    {	/* no, parse column name and later find column index from types */
	SdifColumnDefT  *col   = NULL;
	int		len    = parsestring ();
	char		*cname = SdifCalloc (char, 100);

	strncpy (cname, SYMBOL, len);
	cname [len] = '0';
	/*
	col = SdifMatrixTypeGetColumnDef (SdifMatrixTypeT *MatrixType, cname);
        */

	if (debug) fprintf (stderr,
	    "parsecol\t\tTOKEN %2d %-2s  SYMBOL '%10s'  INPUT '%10s'  "
	    "ret %d  colname '%s'\n",
	    TOKEN, symbol (TOKEN), SYMBOL, INPUT, col != NULL, cname);
	assert (!"can't handle column names, yet");
	SdifFree (cname);
	return (col != NULL);
    }
}



/* Parse one element's list of values plus range or delta.  
   Return true if ok. */
static int 
parse (int (*parseval) (SdifSelectValueT *valu), SdifListP list, 
       int range_allowed, char *name)
{
#   define print_error1(msg, arg)	/* todo: use sdiferr... */       \
	   fprintf (stderr,						 \
		    "ERROR: SDIF selection: can't parse %s from '%s'\n(recently read: '%.*s'):\n" msg "\n\n", name, SYMBOL, SYMBOL - ORIG, ORIG, arg)

    int		       ret   = 0;	/* being pessimistic */
    SdifSelectElementT *elem = SdifMalloc (SdifSelectElementT);
    elem->rangetype = sst_norange;

    if (parseval (&elem->value))
    {	
	switch (parsenexttoken ())
	{
	    case sst_range:
	    case sst_delta:	/* add elem only if range ok */
		if (range_allowed)
		{
		    if (parseval (&elem->range))
		    {   /* add elem and continue looking for list */
			elem->rangetype = TOKEN;
			SdifListPutTail (list, elem);
			if (parsenexttoken () == sst_list)
			    ret = parse (parseval, list, range_allowed, name);
			else
			    if (!(ret = TOKEN != sst_range  &&  TOKEN != sst_delta))
				print_error1 ("Another range symbol '%s' after complete range", symbol(TOKEN));
				
		    }
		    else
			print_error1 ("Malformed range expression\n", 0);
		}
		else
		    print_error1 ("Range only permitted with numerical values\n", 0);
	    break;
	
	    case sst_list:	/* add elem and recursively read comma-list */
		SdifListPutTail (list, elem);
		ret = parse (parseval, list, range_allowed, name);
	    break;

	    case sst_num:	/* next token is no separator */
		SdifListPutTail (list, elem);   /* add this elem in any case */
		if (*INPUT)	/* error when not at end of INPUT */
		    print_error1 ("No separator after value '%s'\n", INPUT);
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
	print_error1 ("Can't read value from '%s'\n", INPUT);

    if (debug)  fprintf (stderr, 
	"parse %s\tTOKEN %2d %-2s  SYMBOL '%10s'  INPUT '%10s'  ret %d\n",
	name, TOKEN, symbol (TOKEN), SYMBOL, INPUT, ret);
    return (ret);
}




/*
// FUNCTION GROUP:	externally callable functions
*/

/* parse elements #stream :frame /matrix .column _row @time in any order */
int 
SdifParseSelection (SdifSelectionT *sel, char *str)
{
    int ret = 2;	/* first iteration */

    INPUT = ORIG = str;	/* set input to parse from */
    if (INPUT)
	parsenexttoken ();
    while (ret  &&  INPUT  &&  *INPUT)
    {
	switch (TOKEN)
	{
	    case sst_stream: 
		ret = parse (parseint,  sel->stream, 1, "stream"); break;
	    case sst_frame:  
		ret = parse (parsesig,  sel->frame,  0, "frame");  break;
	    case sst_matrix: 
		ret = parse (parsesig,  sel->matrix, 0, "matrix"); break;
	    case sst_column: 
		ret = parse (parsecol,  sel->column, 1, "column"); break;
	    case sst_row:    
		ret = parse (parseint,  sel->row,    1, "row");    break;
	    case sst_time:   
		ret = parse (parsereal, sel->time,   1, "time");   break;
	    default:
		if (ret == 2  &&  *INPUT)
		{   /* special case: if frame is first element, don't
		       need to use symbol : to avoid ":::1FRM" */
		    ret = parse (parsesig, sel->frame, 0, "frame");
		}
	    break;
	}
    }
    return (ret);
}


/* Returns pointer to first char of select spec (starting with ::), or
   NULL if not found.  */
char *
SdifSelectFindSelection (const char *filename)
{
    const char  *spec = filename;
    char	*last = NULL;

    assert (gSdifInitialised  &&  "SDIF library not initialised!");

    /* find last spec separator '::' in last */
    while (spec)
    {
	if (spec  = strstr (spec, symbol(sst_specsep)))
	{
	    last  = (char *) spec;
	    spec += symlen (sst_specsep);
	}
    }
    return (last);
}


char *
SdifGetFilenameAndSelection (/*in*/  const char *filename, 
			     /*out*/ SdifSelectionT *sel)
{
    const char *spec = SdifSelectFindSelection (filename);

    SdifInitSelection  (sel, filename, spec  ?  spec - filename 
					     :  strlen (filename));
    if (spec)
	SdifParseSelection (sel, spec + symlen (sst_specsep));
    return (sel->filename);
}


void 
SdifPrintSelection (FILE *out, SdifSelectionT *sel, int options)
{
    char *tn [] = {" ", " any\n"}, 
	 *nc [] = {"\n", ", "};
    SdifSignature sig;

#   define printinit(elem) \
    fprintf (out, "   %-6s:%s", #elem, tn [SdifListIsEmpty(sel->elem)]);\
    SdifListInitLoop (sel->elem)

#   define printrange(elem, type, fmt)		\
    {	SdifSelectElement##type##T range;	printinit (elem);\
        while(SdifSelectGetNext##type##Range (sel->elem, &range, options & 1))\
        {  \
	    int nor = range.rangetype == sst_norange, \
		nxt = SdifListIsNext (sel->elem);     \
	    fprintf (out,         nor ? fmt "%s" : fmt " %s " fmt "%s",	      \
		     range.value, nor ? nc [nxt] : symbol (range.rangetype),  \
		     range.range, nc [nxt]); \
	} \
    }

#   define printsig(elem)    \
    {	SdifSignature sig;   printinit (elem); \
        while (sig = SdifSelectGetNextSignature (sel->elem))   {	\
	    fprintf (out, "%s%s", SdifSignatureToString (sig),		\
				  nc [SdifListIsNext (sel->elem)]);     \
	} \
    }

    fprintf (out, "selection: file %s  basename %s\n", 
	     sel->filename, sel->basename);
    printrange (stream, Int,  "%d");
    printsig   (frame);
    printsig   (matrix);
    printrange (column, Int,  "%d");
    printrange (row,    Int,  "%d");
    printrange (time,   Real, "%g");
}




/*
// FUNCTION GROUP:	Access functions for SdifSelectValueT
*/

static int	      getint	   (SdifSelectValueT val)
{ return (val.integer);   }

static double	      getreal	   (SdifSelectValueT val)
{ return (val.real);	  }	  
					  
static SdifSignature  getsignature (SdifSelectValueT val)
{ return (val.signature); }	  
					  
static char	     *getstring	   (SdifSelectValueT val)
{ return (val.string);    }


/*
// FUNCTION GROUP:	Add Selections to Element Lists
*/


#define _foralltypes(macro) \
macro (Int,	  int,		 integer)   \
macro (Real,	  double,	 real)	    \
macro (Signature, SdifSignature, signature) \
macro (String,	  char *,	 string)


#define _addrange(name, type, field) \
_addrangeproto   (name, type, field) \
{   SdifSelectElementT *elem = SdifMalloc (SdifSelectElementT);	        \
    elem->value.field = value;     \
    elem->range.field = range;     \
    elem->rangetype   = rt;        \
    SdifListPutTail (list, elem);  \
}

#define _addsimple(name, type, field) \
_addsimpleproto   (name, type, field) \
{   SdifSelectAdd##name##Range (list, value, sst_norange, (type) 0);   }

#define _add(name, type, field) \
_addrange  (name, type, field)  \
_addsimple (name, type, field)

_foralltypes (_add)


/*
// FUNCTION GROUP:	Query parsed ranges (list of ranges).

   Init list traversal with SdifListInitLoop, then call SdifSelectGetNext* 
   until it returns 0.
*/

int 
SdifSelectGetNextIntRange  (/*in*/  SdifListP list, 
			    /*out*/ SdifSelectElementIntT  *range, 
			    /*in*/  int force_range)
{
    int avail, delta;

    if (avail = SdifListIsNext (list))
    {
	SdifSelectElementT *elem = SdifListGetNext (list);
	
	if (force_range)
	{
	    switch (elem->rangetype)
	    {
	    	case sst_range:
		    range->value = MIN (elem->value.integer, elem->range.integer);
		    range->range = MAX (elem->value.integer, elem->range.integer);
	    	break;
    
	    	case sst_delta:
		    delta        = abs (elem->range.integer);
		    range->value = elem->value.integer - delta;
		    range->range = elem->value.integer + delta;
	    	break;

		default:
		    range->value = elem->value.integer;
		    range->range = elem->value.integer;
		break;
	    }
	    range->rangetype = sst_range;
	}
	else
	{
	    range->value     = elem->value.integer;
	    range->range     = elem->range.integer;
	    range->rangetype = elem->rangetype;
	}
    }
    return (avail);
}


int 
SdifSelectGetNextRealRange (/*in*/  SdifListP list, 
			    /*out*/ SdifSelectElementRealT *range, 
			    /*in*/  int force_range)
{
    int		avail;
    double	delta;

    if (avail = SdifListIsNext (list))
    {
	SdifSelectElementT *elem = SdifListGetNext (list);
	
	if (force_range)
	{
	    switch (elem->rangetype)
	    {
	    	case sst_range:
		    range->value = MIN (elem->value.real, elem->range.real);
		    range->range = MAX (elem->value.real, elem->range.real);
	    	break;
    
	    	case sst_delta:
		    delta        = abs (elem->range.real);
		    range->value = elem->value.real - delta;
		    range->range = elem->value.real + delta;
	    	break;

		default:
		    range->value = elem->value.real;
		    range->range = elem->value.real;
		break;
	    }
	    range->rangetype = sst_range;
	}
	else
	{
	    range->value     = elem->value.real;
	    range->range     = elem->range.real;
	    range->rangetype = elem->rangetype;
	}
    }
    return (avail);
}


char *
SdifSelectGetNextString    (/*in*/  SdifListP list)
{
     return (SdifListIsNext (list)  
	     ?  ((SdifSelectElementT *) SdifListGetNext (list))->value.string
	     :  NULL);
}


SdifSignature 
SdifSelectGetNextSignature (/*in*/  SdifListP list)
{
     return (SdifListIsNext (list)  
	     ?  ((SdifSelectElementT *) SdifListGetNext(list))->value.signature
	     :  NULL);
}


/* define code for:
int	       SdifSelectGetFirstInt       (SdifListP l, int defval);
double	       SdifSelectGetFirstReal      (SdifListP l, double defval);
char	      *SdifSelectGetFirstString    (SdifListP l, char *defval);
SdifSignature  SdifSelectGetFirstSignature (SdifListP l, SdifSignature defval);

Return value of first selection (ignoring range).
*/


#define _getfirst(name, type, field)					     \
type SdifSelectGetFirst##name (/*in*/ SdifListP list, type defval)	     \
{    SdifListInitLoop (list);						     \
     return (SdifListIsNext (list)  					     \
	     ?  ((SdifSelectElementT *) SdifListGetNext (list))->value.field \
	     :  defval);						     \
}

_foralltypes (_getfirst)



/*
// FUNCTION GROUP:	Selection Testing Functions
*/

int
SdifSelectTestIntRange (SdifSelectElementT *elem, int cand)
{
    if (!elem)   return (0);

    switch (elem->rangetype)
    {
    	case sst_norange: 
	    return (elem->value.integer == cand);
    	case sst_range: 
	    if (elem->value.integer <= elem->range.integer)
	        return (elem->value.integer <= cand  &&  cand <= elem->range.integer);
	    else
	        return (elem->value.integer >= cand  &&  cand >= elem->range.integer);	    
    	case sst_delta: 
	    return (abs (elem->value.integer - cand) <= abs (elem->range.integer));
    }
    assert (!"corrupt rangetype");
    return (0);
}


int
SdifSelectTestRealRange (SdifSelectElementT *elem, double cand)
{
    if (!elem)   return (0);

    switch (elem->rangetype)
    {
    	case sst_norange: 
	    return (elem->value.real == cand);
    	case sst_range: 
	    if (elem->value.real <= elem->range.real)
	        return(elem->value.real <= cand  &&  cand <= elem->range.real);
	    else
	        return(elem->value.real >= cand  &&  cand >= elem->range.real);
    	case sst_delta: 
	    return (fabs (elem->value.real - cand) <= fabs (elem->range.real));
    }
    assert (!"corrupt rangetype");
    return (0);
}


int
SdifSelectTestInt (SdifListT *list, int cand)
{
    if (SdifListIsEmpty (list))
	return (1);	/* no select spec means: take everything */
    SdifListInitLoop (list);
    while (SdifListIsNext (list))
    {
	if (SdifSelectTestIntRange (SdifListGetNext (list), cand))
	    return (1);
    }
    return (0);
}

int
SdifSelectTestReal (SdifListT *list, double cand)
{
    SdifSelectElementT *first;

    if (SdifListIsEmpty (list))
	return (1);	/* no select spec means: take everything */

    /* first test from current select element onwards */
    if (SdifSelectTestRealRange (SdifListGetCurr (list), cand))
        return (1);

    SdifListInitLoop (list);
    while (SdifListIsNext (list))
    {
	if (SdifSelectTestRealRange (SdifListGetNext (list), cand))
	    return (1);
    }
    return (0);
}

int
SdifSelectTestSignature (SdifListT *list, const SdifSignature cand)
{
    if (SdifListIsEmpty (list))
	return (1);	/* no select spec means: take everything */
    SdifListInitLoop (list);
    while (SdifListIsNext (list))
    {
	if (((SdifSelectElementT *) SdifListGetNext (list))->value.signature 
	    == cand)
	    return (1);
    }
    return (0);
}

int
SdifSelectTestString (SdifListT *list, const char *cand)
{
    if (SdifListIsEmpty (list))
	return (1);	/* no select spec means: take everything */
    SdifListInitLoop (list);
    while (SdifListIsNext (list))
    {
	if (strcmp (((SdifSelectElementT *) SdifListGetNext (list))->value.string, cand) == 0)
	    return (1);
    }
    return (0);
}


/*
// FUNCTION GROUP:	Using a Selection in File I/O.
*/

/*DOC:
  Test the selection elements applicable to frames: time, stream, frame type.
  Can be called after SdifFReadFrameHeader().
*/
int
SdifFCurrFrameIsSelected (SdifFileT *file, SdifSelectionT *sel)
{
    return (
       (SdifSelectTestInt	(sel->stream, SdifFCurrID   (file))
        ||  SdifFCurrID (file) == _SdifAllStreamID)		     &&
        SdifSelectTestReal	(sel->time,   SdifFCurrTime (file))  &&
	SdifSelectTestSignature (sel->frame,  SdifFCurrFrameSignature (file)));
}

/*DOC:
  Test the selection elements applicable to matrices: the matrix signature
  Can be called after SdifFReadMatrixHeader().
*/
int
SdifFCurrMatrixIsSelected (SdifFileT *file, SdifSelectionT *sel)
{
    return (SdifSelectTestSignature (sel->matrix, 
				     SdifFCurrMatrixSignature (file)));
}
