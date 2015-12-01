#include <cassert>
#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include "pbconfig.h"
#include "core/pbutil.h"
#include "shadercompiler.h"
#include "instancedmesh.h"
#include "instancedobject.h"

namespace pb
{

InstancedObject::InstancedObject(const InstancedObject& rhs) :
    InstancedObject()
{
    //obj = rhs;
    mpMesh = rhs.mpMesh;
    mId = mpMesh->AddInstance();
    mpTransform = mpMesh->Transform( mId );
}

InstancedObject::InstancedObject() :
    mId( 0 ),
    mpMesh( NULL )
{
    //mStatus = init();
    mStatus = PB_ERR;
}

InstancedObject::~InstancedObject()
{
    // TODO
    //PB_DELETE( mpTransform );
    //PB_DELETE( mpMesh );
}

void InstancedObject::Make(InstancedObject& obj, unsigned int hintPerInstanceTableSize)
{
    obj.mHintPerInstanceTableSize = hintPerInstanceTableSize;
    obj.mStatus = obj.init();
}

void InstancedObject::MakeAnother(const InstancedObject& alpha, InstancedObject& another)
{
    another.mpMesh = alpha.mpMesh;
    another.mId = another.mpMesh->AddInstance();
    another.mpTransform = another.mpMesh->Transform( another.mId );
}

PBError InstancedObject::init()
{
    mId = mpMesh->AddInstance();
    mpTransform = mpMesh->Transform( mId );

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
    assert( mId == 0 );
    mpMesh->Render( vp );
}

void InstancedObject::computeTransform()
{
    if ( transformDirty() )
    {
        Object::computeTransform();
        mpMesh->UpdateTransform( mId );
    }
}


}

