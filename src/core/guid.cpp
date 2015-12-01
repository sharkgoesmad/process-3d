#include "guid.h"

using namespace pb;

const guid_t GUID::InvalidGUID = 0;
static guid_t guid = 0;

guid_t Get()
{
    return ++guid;
}
