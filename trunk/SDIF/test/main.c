/* main for test
 */


#include "sdif.h"

int main(void)
{  
  SdifGenInit(_SdifGenHashSize, _SdifTypesFileName);
  SdifTextConv("File.sdif.txt.r", "File.sdif.w");
  SdifGenKill();
  
  SdifGenInit(_SdifGenHashSize, _SdifTypesFileName);
  SdifConvToText("File.sdif.w", "File.sdif.txt.w");
  SdifGenKill();

  return 0;
}

