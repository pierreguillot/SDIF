/* $Id: testselect.c,v 1.1 1999-08-31 10:03:38 schwarz Exp $
 *
 * testselect		30. August 1999		Diemo Schwarz
 *
 * Test functions from SdifSelect.c
 *
 * $Log: not supported by cvs2svn $
 */

#include <stdio.h>
#include <SdifFile.h>
#include <SdifSelect.h>

main (int argc, char *argv [])
{
  char *arg = argc > 1  ?  argv [1]  :  NULL;
  SdifSelectionT sel;

  printf ("%s %s\n", argv [0], arg);
  SdifGenInit ("");

  SdifGetFilenameAndSelection (arg, &sel);
  printf ("selection: file %s  basename %s\nstream %d  frame %s  time %f\n",
	  sel.filename, sel.basename,
	  SdifSelectGetNextInt (sel.stream),
	  SdifSignatureToString (SdifSelectGetNextSignature (sel.frame)),
	  SdifSelectGetNextDouble (sel.time));
	
  SdifGenKill ();
}
