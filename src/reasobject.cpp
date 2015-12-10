#include <cassert>
//#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include "pbconfig.h"
#include "core/pbutil.h"
#include "shadercompiler.h"
#include "instancedgeometry.h"
#include "reasobject.h"

namespace pb
{

ReasObject::ReasObject(const ReasObject& rhs) :
    InstancedObject( rhs )
{
    //mpTransform = mpMesh->Transform( mId );
}

ReasObject::ReasObject() :
    InstancedObject(),
    mpColor( NULL )
{
}

ReasObject::~ReasObject()
{
    //PB_DELETE( mpTransform );
    //PB_DELETE( mpMesh );
}

//void ReasObject::Make(ReasObject& obj)
//{
//    // TODO set size based in GL_MAX_INSTANCES
//    obj.mHintPerInstanceTableSize = 500000;
//    obj.mStatus = obj.init();
//}

void ReasObject::MakeAnother(InstancedObject* pAnother)
{
    assert( mId == 0 );

    ReasObject* pOther = static_cast< ReasObject* >( pAnother );

    pOther->mpGeometry = mpGeometry;
    pOther->mId = mpGeometry->AddInstance();

    f32_attribs* attribs = geometry< f32_attribs, u32_attribs >()->AttributesF32( pOther->mId );
    pOther->mpTransform = &attribs->transform;
    u32_attribs* attribsu32 = geometry< f32_attribs, u32_attribs >()->AttributesU32( pOther->mId );
    pOther->mpColor = &attribsu32->color;
}

PBError ReasObject::init()
{
    glm::vec3 srcPos[] = {

        glm::vec3( 0.0f, -0.5f, 0.0f ),
        glm::vec3( 0.0f, 0.5f, 0.0f )
//        glm::vec3( 0.4f, 0.1f, 0.0f ),
//        glm::vec3( -0.5f, -0.3f, 0.0f )

    };

// good color
//    uint32_t srcColors[] = {
//        //ABGR
//        0x02020000,
//        0x02040700,
//        0x02080004
////        0x00010000,
////        0x01020000
//
//    };

    uint32_t srcColors[] = {
        //ABGR
        0xFF080000,
        0xFF080000
//        0x00010000,
//        0x01020000

    };

    uint32_t srcIdx[] = { 0, 1 };


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

    mpGeometry.reset( new InstancedGeometry< f32_attribs, u32_attribs >(
        GL_LINE_STRIP,
        pIndices.Disown(),
        pPositions.Disown(),
        pColors.Disown(),
        idProgram,
        mHintPerInstanceTableSize) );

    if ( mpGeometry == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    mId = mpGeometry->AddInstance();

    f32_attribs* attribsf32 = geometry< f32_attribs, u32_attribs >()->AttributesF32( mId );
    mpTransform = &attribsf32->transform;
    u32_attribs* attribsu32 = geometry< f32_attribs, u32_attribs >()->AttributesU32( mId );
    mpColor = &attribsu32->color;

    attribDesc f32attribDesc[] = { { attribDesc::SizeMat4 } };
    attribDesc u32attribDesc[] = { { attribDesc::SizeRGBA } };

    geometry< f32_attribs, u32_attribs >()->UseF32Layout( f32attribDesc, 1 );
    geometry< f32_attribs, u32_attribs >()->UseUint32Layout( u32attribDesc, 1 );

    return InstancedObject::init();
}

void ReasObject::SetColor(unsigned int rgba)
{
    *mpColor = rgba;
}

void ReasObject::Update()
{
    if ( transformDirty() )
    {
        computeTransform();
    }

    geometry< f32_attribs, u32_attribs >()->UpdateAttributesU32( mId );
}

void ReasObject::Draw(const glm::mat4& vp)
{
    assert( mId == 0 );
    mpGeometry->Render( vp );
}

void ReasObject::updateTransformAttrib()
{
    // TODO update just the transform
    geometry< f32_attribs, u32_attribs >()->UpdateAttributesF32( mId );
}


}

