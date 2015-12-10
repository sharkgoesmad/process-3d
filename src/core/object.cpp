#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "mathutil.h"
#include <cmath>
#include "object.h"

namespace pb
{

static const Vec3 axis_x( 1.0f, 0.0f, 0.0f );
static const Vec3 axis_y( 0.0f, 1.0f, 0.0f );
static const Vec3 axis_z( 0.0f, 0.0f, 1.0f );
static const Vec3 vec_zero( 0.0f, 0.0f, 0.0f );
static const Vec3 vec_fwd( 0.0f, 0.0f, -1.0f );

Object::Object() :
    mStatus( PB_ERR ),
    mpTransform( NULL ),
    mVisible( true ),
    mTransformDirty( true ),
    mPosition( 0.0f ),
    mRotation( Vec3(0.0f, 0.0f, 0.0f) ),
    mScale( 1.0f )
{
    mStatus = init();
}

Object::~Object()
{
}

PBError Object::init()
{
    return PB_ERR_OK;
}

void Object::computeTransform()
{
    mRotation = glm::normalize( mRotation );
//    Mat4 m = glm::toMat4( mRotation );
//    Log::Info("After:\n" +
//        STR(m[0][0])+" "+STR(m[1][0])+" "+STR(m[2][0])+" "+STR(m[3][0])+"\n" +
//        STR(m[0][1])+" "+STR(m[1][1])+" "+STR(m[2][1])+" "+STR(m[3][1])+"\n" +
//        STR(m[0][2])+" "+STR(m[1][2])+" "+STR(m[2][2])+" "+STR(m[3][2])+"\n" +
//        STR(m[0][3])+" "+STR(m[1][3])+" "+STR(m[2][3])+" "+STR(m[3][3])+"\n\n");
    *mpTransform = glm::translate( mPosition ) * glm::toMat4( mRotation ) * glm::scale( mScale );
}

void Object::Translate(float x, float y, float z)
{
    mPosition.x += x;
    mPosition.y += y;
    mPosition.z += z;
    markTransformDirty();
}

void Object::Translate(const Vec3& vector)
{
    mPosition += vector;
    markTransformDirty();
}

void Object::TranslateX(float value)
{
    mPosition.x += value;
    markTransformDirty();
}

void Object::TranslateY(float value)
{
    mPosition.y += value;
    markTransformDirty();
}

void Object::TranslateZ(float value)
{
    mPosition.z += value;
    markTransformDirty();
}

void Object::SetPosition(float x, float y, float z)
{
    mPosition.x = x;
    mPosition.y = y;
    mPosition.z = z;
    markTransformDirty();
}

void Object::SetPosition(const Vec3& vector)
{
    mPosition = vector;
    markTransformDirty();
}

void Object::RotateX(float value)
{
    mRotation *= Quat( axis_x * value );
    markTransformDirty();
}

void Object::RotateY(float value)
{
    mRotation *= Quat( axis_y * value );
    markTransformDirty();
}

void Object::RotateZ(float value)
{
    mRotation *= Quat( axis_z * value );
    markTransformDirty();
}

void Object::Rotate(float x, float y, float z)
{
    mRotation *= Quat( Vec3(x, y, z) );
    markTransformDirty();
}

void Object::Rotate(const Vec3& vector)
{
    mRotation *= Quat( vector );
    markTransformDirty();
}

void Object::Rotate(const Quat& quaternion)
{
    mRotation *= quaternion;
    markTransformDirty();
}

void Object::SetOrientation(const Quat& quaternion)
{
    mRotation = quaternion;
    markTransformDirty();
}

void Object::LookAt(const Vec3& vector)
{
    Vec3 nowForward( vec_fwd );

    Vec3 newForward( vector - mPosition );
    newForward = glm::normalize( newForward );
    Quat rot1 = glm::rotation( nowForward, newForward );

    // set desired Up to be perpendicular to newForward
    Vec3 desiredUp( axis_y );
    Vec3 right = glm::normalize(glm::cross( newForward, desiredUp ));
    desiredUp = glm::normalize(glm::cross( right, newForward ));
    Vec3 newUp( glm::normalize(rot1 * axis_y) );
    Quat rot2 = glm::rotation( newUp, desiredUp );

    // TODO handle case when forward vector is parallel with y

    mRotation = (rot2 * rot1);

    markTransformDirty();
}

void Object::Scale(float value)
{
    mScale *= value;
}

void Object::SetVisible(bool visible)
{
    mVisible = visible;
}


}

