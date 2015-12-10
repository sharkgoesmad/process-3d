#ifndef GLMCFG_H
#define GLMCFG_H

// TODO appropriate defines for glm
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

namespace pb
{

typedef glm::vec3 Vec3;
typedef glm::quat Quat;
typedef glm::mat4x4 Mat4;
typedef uint32_t ColorRGBA;

typedef std::vector<uint32_t> Indices;
typedef std::vector<glm::vec3> Positions;
typedef std::vector<ColorRGBA> ColorsRGBA;
typedef std::vector<Mat4> Transforms;

};

#endif  // GLMCFG_H
