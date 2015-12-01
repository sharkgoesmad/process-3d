#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

namespace pb
{

Camera::Camera() :
    mpProjection( NULL ),
    mpVP( NULL )
{
    mStatus = init();
}

Camera::~Camera()
{
    PB_DELETE( mpVP );
    PB_DELETE( mpProjection );
    PB_DELETE( mpTransform );
}

PBError Camera::init()
{
    if ( mStatus == PB_ERR ) return PB_ERR;

    if ( (mpTransform = new Mat4(1.0f)) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    if ( (mpProjection = new Mat4(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 1000.0f))) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

//    *mpTransform = glm::lookAt(
//        glm::vec3( 0, 0, 0 ),
//        glm::vec3( 0, 0, 0 ),
//        glm::vec3( 0, 1, 0 ));

    if ( (mpVP = new Mat4(1.0f)) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    SetPosition( 0.0f, 0.0f, 5.0f );
    computeTransform();

    return PB_ERR_OK;
}

void Camera::computeVP()
{
    if ( transformDirty() )
    {
        computeTransform();
        *mpVP = *mpProjection * glm::inverse( *mpTransform );;
        markTransformClean();
    }
}

}

