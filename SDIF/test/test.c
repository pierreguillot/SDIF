/* tosdif.c
 *
 *
 *
 *
 */


#include "sdif.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int
main(int argc, char *argv[])
{
  SdifFileT *SdifF;
  float t[10] = { 1., 2., 3., 4., 5., 6., 7., 8., 9., 0.};




  SdifGenInit("SdifTypes.STYP");
  SdifF = SdifOpenFile("test.sdif", eWriteFile);
  SdifPrintFrameHeader   (stderr, SdifSetCurrFrameHeader (SdifF, 'fram', _SdifUnknownSize, 1, 0, 1.234));
  SdifPrintMatrixHeader  (stderr, SdifSetCurrMatrixHeader(SdifF, 'mtrx', eFloat4, 1, 10));
  SdifPrintOneRow        (stderr, SdifSetCurrOneRow      (SdifF, t));
  SdifPrintOneRow        (stderr, SdifSetCurrOneRowCol (SdifF, 5, 10.));
  SdifPrintOneRow        (stderr, SdifSetCurrOneRowCol (SdifF, 10, 10.));
  SdifPrintOneRow        (stderr, SdifSetCurrOneRowCol (SdifF, 10, SdifGetCurrOneRowCol (SdifF, 1)));
  /*SdifPrintAllType(stderr, gSdifPredefinedTypes);*/
  SdifCloseFile(SdifF);
  SdifGenKill();

  return 0;
}
