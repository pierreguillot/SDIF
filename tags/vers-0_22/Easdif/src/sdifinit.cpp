#include "sdifinit.h"

namespace Easdif {

//void EasdifInit(char* PredefinedType)
void EasdifInit(const std::string& PredefinedType)
{
    SdifGenInit(PredefinedType.c_str());
    SdifSetErrorFunc(ExceptionThrower);
}

void EasdifEnd()
{
 SdifGenKill();
}

} // end of namespace Easdif
