#include "sdifinit.h"


//void EasdifInit(char* PredefinedType)
void EasdifInit(std::string PredefinedType)
{
    char* Predefined;    
    Predefined = const_cast<char*>(PredefinedType.c_str()); 
    SdifGenInit(Predefined);
    SdifSetErrorFunc(ExceptionThrower);
}

void EasdifEnd()
{
 SdifGenKill();
}
