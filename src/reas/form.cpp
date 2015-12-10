#include <glm/gtc/random.hpp>
#include <core/mathutil.h>
#include "progress.h"
#include "form.h"

namespace pb
{

using namespace reas;

Form::Form(Progress* pProgress, const Vec3& pos, const Vec3& dir, float speed, float angSpeed) :
    pprogress( pProgress ),
    initialPosition( pos ),
    direction( dir ),
    speed( speed ),
    angSpeed( angSpeed )
{
    mStatus = init();
}

Form::~Form()
{
}

PBError Form::init()
{
    //if ( IsZero(glm::length(direction)) )
    //{
        //direction = glm::sphericalRand( 1.0f );
    //}

    // TODO
    //position = glm::ballRand( 8.0f );
    SetPosition( initialPosition );
}


}

