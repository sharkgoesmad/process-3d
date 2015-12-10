#include <core/pbutil.h>
#include "globalconfig.h"

namespace pb
{

using namespace reas;

GlobalConfig* GlobalConfig::mpInstance = NULL;

GlobalConfig::GlobalConfig()
{
}

GlobalConfig::~GlobalConfig()
{
}

void GlobalConfig::Release()
{
    if ( mpInstance )
    {
        PB_DELETE( mpInstance );
    }
}

void GlobalConfig::Reset()
{
    distanceFactor = 0.5f;
    lightness = 0.01f;
}

}

