#include "sdifinit.h"

namespace Easdif {

//void EasdifInit(char* PredefinedType)
void EasdifInit(const std::string& PredefinedType)
{
    EasdifInit(PredefinedType.c_str());
}

//void EasdifInit(char* PredefinedType)
void EasdifInit(const char *PredefinedType)
{
    SdifGenInit(PredefinedType);
    SdifSetErrorFunc(ExceptionThrower);
}

void EasdifEnd()
{
 SdifGenKill();
}

} // end of namespace Easdif
