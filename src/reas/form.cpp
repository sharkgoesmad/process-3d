#include <glm/gtc/random.hpp>
#include <core/mathutil.h>
#include "form.h"

namespace pb
{

using namespace reas;

Form::Form(const Vec3& pos, const Vec3& dir) :
    position( pos ),
    direction( dir )
{
    mStatus = init();
}

Form::~Form()
{
}

PBError Form::init()
{
    if ( IsZero(glm::length(direction)) )
    {
        direction = glm::sphericalRand( 1.0f );
    }

    position = glm::ballRand( 25.0f );
}


}

