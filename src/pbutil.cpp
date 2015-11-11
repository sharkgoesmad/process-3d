#include <pbutil.h>

using namespace pb;

float pb::Clamp(float x, float min, float max)
{
    return x < min ? min : (x < max ? x : max);
}