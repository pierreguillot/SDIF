/* $Id: testselect.c,v 1.2 1999-09-20 13:23:09 schwarz Exp $
 *
 * testselect		30. August 1999		Diemo Schwarz
 *
 * Test functions from SdifSelect.c
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.1  1999/08/31  10:03:38  schwarz
 * Added test code for module SdifSelect which parses an access specification to a
 * chosen part of SDIF data.  Can be added to a file name.
 *
 */

#include <stdio.h>
#include <SdifFile.h>
#include <SdifSelect.h>

main (int argc, char *argv [])
{
    SdifSelectionT		sel;
    SdifSelectElementIntT	intrange;
    SdifSelectElementRealT	realrange;
    SdifSignature		sig;
    char			*arg = argc > 1  ?  argv [1]  :  NULL;
    
    printf ("%s %s\n", argv [0], arg);
    SdifGenInit ("");
    
    SdifGetFilenameAndSelection (arg, &sel);
    
    printf ("selection: file %s  basename %s", sel.filename, sel.basename);
    
    SdifListInitLoop (sel.stream);
    while (SdifSelectGetNextInt (sel.stream, &intrange, 1))
	printf ("\n  stream\t%d - %d ", intrange.value, intrange.range);

    SdifListInitLoop (sel.frame);
    while (sig = SdifSelectGetNextSignature (sel.frame))
	printf ("\n  frame\t'%s' ", SdifSignatureToString (sig));

    SdifListInitLoop (sel.matrix);
    while (sig = SdifSelectGetNextSignature (sel.matrix))
	printf ("\n  matrix\t'%s' ", SdifSignatureToString (sig));

    SdifListInitLoop (sel.column);
    while (SdifSelectGetNextInt (sel.column, &intrange, 1))
	printf ("\n  column\t%d - %d ", intrange.value, intrange.range);

    SdifListInitLoop (sel.row);
    while (SdifSelectGetNextInt (sel.row, &intrange, 1))
	printf ("\n  row\t%d - %d ", intrange.value, intrange.range);

    SdifListInitLoop (sel.time);
    while (SdifSelectGetNextReal (sel.time, &realrange, 1))
	printf ("\n  time\t%f - %f ", realrange.value, realrange.range);

    printf ("\n\n");

    SdifPrintSelection (stdout, &sel, 0);
	
    SdifGenKill ();
}
