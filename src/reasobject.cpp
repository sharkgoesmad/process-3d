#include <cassert>
#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include "pbconfig.h"
#include "core/pbutil.h"
#include "shadercompiler.h"
#include "instancedmesh.h"
#include "reasobject.h"

namespace pb
{

ReasObject::ReasObject(const ReasObject& rhs) :
    ReasObject()
{
    //obj = rhs;
    mpMesh = rhs.mpMesh;
    mId = mpMesh->AddInstance();
    mpTransform = mpMesh->Transform( mId );
}

ReasObject::ReasObject() :
    mId( 0 ),
    mpMesh( NULL )
{
    //mStatus = init();
    mStatus = PB_ERR;
}

ReasObject::~ReasObject()
{
    //PB_DELETE( mpTransform );
    //PB_DELETE( mpMesh );
}

void ReasObject::Make(ReasObject& obj)
{
    // TODO set size based in GL_MAX_INSTANCES
    obj.mHintPerInstanceTableSize = 50000;
    obj.mStatus = obj.init();
}

void ReasObject::MakeAnother(const ReasObject& alpha, ReasObject& another)
{
    another.mpMesh = alpha.mpMesh;
    another.mId = another.mpMesh->AddInstance();
    another.mpTransform = another.mpMesh->Transform( another.mId );
}

PBError ReasObject::init()
{
    glm::vec3 srcPos[] = {

        glm::vec3( 0.0f, -0.5f, 0.0f ),
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 0.5f, 0.0f )
//        glm::vec3( 0.4f, 0.1f, 0.0f ),
//        glm::vec3( -0.5f, -0.3f, 0.0f )

    };

    uint32_t srcColors[] = {
        //ABGR
        0x01010000,
        0x01020000,
        0x02040000
//        0x00010000,
//        0x01020000

    };

    uint32_t srcIdx[] = { 0, 1, 2 };


    scoped_ptr<Positions> pPositions( new Positions(srcPos, srcPos + sizeof(srcPos) / sizeof(srcPos[0])) );
    scoped_ptr<ColorsRGBA> pColors( new ColorsRGBA(srcColors, srcColors + sizeof(srcColors) / sizeof(srcColors[0])) );
    scoped_ptr<Indices> pIndices( new Indices( srcIdx, srcIdx + sizeof(srcIdx) / sizeof(srcIdx[0])) );

    if ( !pPositions || !pColors || !pIndices )
    {
        PB_RETURN_ALLOCFAIL();
    }


    GLuint idVs, idFs, idProgram;

    idVs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/reas_vs.glsl"), ShaderCompiler::Shader_Vertex );
    idFs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/reas_fs.glsl"), ShaderCompiler::Shader_Fragment );
    idProgram = ShaderCompiler::Program( idVs, idFs );

    if ( idProgram == PB_GL_INVALID_ID )
    {
        Log::Error( "Failed creating program" );
        return PB_ERR;
    }

    mpMesh.reset( new InstancedMesh(
        GL_LINE_STRIP,
        pIndices.Disown(),
        pPositions.Disown(),
        pColors.Disown(),
        idProgram,
        mHintPerInstanceTableSize) );

    if ( mpMesh == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    mId = mpMesh->AddInstance();
    mpTransform = mpMesh->Transform( mId );


    return PB_ERR_OK;
}

void ReasObject::Update()
{
    if ( transformDirty() )
    {
        computeTransform();
    }
}

void ReasObject::Draw(const glm::mat4& vp)
{
    assert( mId == 0 );
    mpMesh->Render( vp );
}

void ReasObject::computeTransform()
{
    if ( transformDirty() )
    {
        Object::computeTransform();
        mpMesh->UpdateTransform( mId );
    }
}


}

