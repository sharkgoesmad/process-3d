#ifndef PBMATHUTIL_H
#define PBMATHUTIL_H

#include <glm/trigonometric.hpp>
#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>

namespace pb
{


float Clamp(float x, float min, float max);

inline bool IsZero(float v)
{
    return glm::abs( v ) <= glm::epsilon<float>();
}

}

#endif  // PBMATHUTIL_H
