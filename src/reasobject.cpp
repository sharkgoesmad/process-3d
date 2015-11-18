#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include "pbconfig.h"
#include "core/log.h"
#include "shadercompiler.h"
#include "instancedmesh.h"
#include "reasobject.h"

namespace pb
{

ReasObject::ReasObject() :
    mMesh( NULL ),
    mTransform( NULL )
{
    mStatus = init();
}

ReasObject::~ReasObject()
{
    PB_DELETE( mTransform );
    PB_DELETE( mMesh );
}

PBError ReasObject::init()
{
    glm::vec3 srcPositions[] = {
        
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 0.5f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f )
        
    };
    
    uint32_t srcColors[] = {
        
        0x000000FF,
        0x000033FF,
        0x000077FF
        
    };
    
    uint32_t srcIndicies[] = { 0, 1, 2 };
    
    
    Positions positions( srcPositions, srcPositions + sizeof(srcPositions) / sizeof(srcPositions[0]) );
    ColorsRGBA colors( srcColors, srcColors + sizeof(srcColors) / sizeof(srcColors[0]) );
    Indicies indicies( srcIndicies, srcIndicies + sizeof(srcIndicies) / sizeof(srcIndicies[0]) );


    GLuint idVs, idFs, idProgram;
    
    idVs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/reas_vs.glsl"), ShaderCompiler::Shader_Vertex );
    idFs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/reas_fs.glsl"), ShaderCompiler::Shader_Fragment );
    idProgram = ShaderCompiler::Program( idVs, idFs );
    
    if ( idProgram == PB_GL_INVALID_ID )
    {
        Log::Error( "Failed creating program" );
        return PB_ERR;
    }


    if ( (mMesh = new InstancedMesh(GL_LINES, indicies, positions, colors, idProgram)) == NULL )
    {
        PB_ALLOCFAIL_RETURN(__FILE__, __LINE__);
    }
    
    if ( (mTransform = new glm::mat4(1.0f)) == NULL )
    {
        PB_ALLOCFAIL_RETURN(__FILE__, __LINE__);
    }
    

    return PB_ERR_OK;
}


}

