#include <cassert>
//#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include "pbconfig.h"
#include "core/pbutil.h"
#include "shadercompiler.h"
#include "instancedgeometry.h"
#include "instancedobject.h"

namespace pb
{

InstancedObject::InstancedObject(const InstancedObject& rhs) :
    InstancedObject()
{
    //obj = rhs;
    mpGeometry = rhs.mpGeometry;
    mId = mpGeometry->AddInstance();
}

InstancedObject::InstancedObject() :
    mId( 0 ),
    mpGeometry( NULL )
{
    //mStatus = init();
    mStatus = PB_ERR;
}

InstancedObject::~InstancedObject()
{
    // TODO
    //PB_DELETE( mpTransform ); <- mesh owns thetransform
    //PB_DELETE( mpGeometry );
}

void InstancedObject::Make(InstancedObject* pObj, unsigned int hintPerInstanceTableSize)
{
    pObj->mHintPerInstanceTableSize = hintPerInstanceTableSize;
    pObj->mStatus = pObj->init();
}

//void InstancedObject::MakeAnother(const InstancedObject& alpha, InstancedObject& another)
//{
//    another.mpGeometry = alpha.mpGeometry;
//    another.mId = another.mpGeometry->AddInstance();
////    another.mpTransform = another.mpGeometry->Transform( another.mId );
//}

PBError InstancedObject::init()
{
    return PB_ERR_OK;
}

void InstancedObject::Update()
{
    if ( transformDirty() )
    {
        computeTransform();
    }
}

void InstancedObject::Draw(const glm::mat4& vp)
{
    // should be carried out by alpha instance only
    assert( mId == 0 );

    if ( Visible() )
    {
        mpGeometry->Render( vp );
    }
}

void InstancedObject::computeTransform()
{
    if ( transformDirty() )
    {
        Object::computeTransform();

        if ( Visible() )
        {
            updateTransformAttrib();
        }
    }
}


}

