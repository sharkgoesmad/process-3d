#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include "pbconfig.h"
#include "core/pbutil.h"
#include "shadercompiler.h"
#include "instancedmesh.h"
#include "reasobject.h"

namespace pb
{

ReasObject::ReasObject() :
    mpMesh( NULL ),
    mpTransform( NULL )
{
    mStatus = init();
}

ReasObject::~ReasObject()
{
    PB_DELETE( mpTransform );
    PB_DELETE( mpMesh );
}

PBError ReasObject::init()
{
    glm::vec3 srcPos[] = {

        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 0.5f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f )

    };

    uint32_t srcColors[] = {

        0x000000FF,
        0x000033FF,
        0x000077FF

    };

    uint32_t srcIdx[] = { 0, 1, 2 };


    scoped_ptr<Positions> pPositions( new Positions(srcPos, srcPos + sizeof(srcPos) / sizeof(srcPos[0])) );
    scoped_ptr<ColorsRGBA> pColors( new ColorsRGBA(srcColors, srcColors + sizeof(srcColors) / sizeof(srcColors[0])) );
    scoped_ptr<Indicies> pIndicies( new Indicies( srcIdx, srcIdx + sizeof(srcIdx) / sizeof(srcIdx[0])) );

    if ( !pPositions || !pColors || !pIndicies )
    {
        PB_ALLOCFAIL_RETURN();
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

    mpMesh = new InstancedMesh( GL_LINES, pIndicies.Disown(), pPositions.Disown(), pColors.Disown(), idProgram );
    if ( mpMesh == NULL )
    {
        PB_ALLOCFAIL_RETURN();
    }

    if ( (mpTransform = new glm::mat4(1.0f)) == NULL )
    {
        PB_ALLOCFAIL_RETURN();
    }


    return PB_ERR_OK;
}

void ReasObject::Draw()
{

}


}

