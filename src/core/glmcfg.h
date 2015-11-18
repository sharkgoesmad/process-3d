#ifndef GLMCFG_H
#define GLMCFG_H

// TODO appropriate defines for glm
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace pb
{

typedef std::vector<uint32_t> Indicies;
typedef std::vector<glm::vec3> Positions;
typedef std::vector<uint32_t> ColorsRGBA;

};

#endif  // GLMCFG_H