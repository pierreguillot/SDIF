#ifndef SDIFINIT_H_
#define SDIFINIT_H_ 1

#include <sdif.h>
#include "easdif/sdifexception.h"

/*
  Initialise the SDIF library, providing a name for an optional additional
  file with type definitions or "". And install exceptions.
  <b>This function has to be called once and only once per process 
  before any other call to the SDIF library.</b> 
*/
void EasdifInit(std::string PredefinedType = "");

void EasdifEnd();

#endif
