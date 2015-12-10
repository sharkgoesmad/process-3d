//#include <GL/gl.h>
#include <shadercompiler.h>
#include <pbconfig.h>
#include <instancedgeometry.h>
#include "drawroutines.h"

namespace pb
{

using namespace reas;


struct f32_attribs
{
    Mat4 transform;
};

struct u32_attribs
{
    ColorRGBA color;
};

DrawRoutine::DrawRoutine() :
    InstancedObject(),
    mpColor( NULL )
{
}

PBError DrawRoutine::init()
{
    geometryInfo info = { NULL, NULL, NULL, -1 };

    if ( PB_FAILED(defineGeometry(info)) )
    {
        return PB_ERR;
    }

    unsigned int idVs, idFs, idProgram;
    idVs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/reas_vs.glsl"), ShaderCompiler::Shader_Vertex );
    idFs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/reas_fs.glsl"), ShaderCompiler::Shader_Fragment );
    idProgram = ShaderCompiler::Program( idVs, idFs );

    ShaderCompiler::DeleteShader( idVs );
    ShaderCompiler::DeleteShader( idFs );

    if ( idProgram == PB_GL_INVALID_ID )
    {
        Log::Error( "Failed creating program" );
        return PB_ERR;
    }

    mpGeometry.reset( new InstancedGeometry< f32_attribs, u32_attribs >(
        info.primitiveType,
        info.pIndices,
        info.pPositions,
        info.pColors,
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

void DrawRoutine::MakeAnother(InstancedObject* pAnother)
{
    assert( mId == 0 );

    DrawRoutine* pOther = static_cast< DrawRoutine* >( pAnother );

    pOther->mpGeometry = mpGeometry;
    pOther->mId = mpGeometry->AddInstance();

    f32_attribs* attribs = geometry< f32_attribs, u32_attribs >()->AttributesF32( pOther->mId );
    pOther->mpTransform = &attribs->transform;
    u32_attribs* attribsu32 = geometry< f32_attribs, u32_attribs >()->AttributesU32( pOther->mId );
    pOther->mpColor = &attribsu32->color;
}

void DrawRoutine::SetColor(unsigned int rgba)
{
    *mpColor = rgba;
}

void DrawRoutine::Update()
{
    if ( transformDirty() )
    {
        computeTransform();
    }

    geometry< f32_attribs, u32_attribs >()->UpdateAttributesU32( mId );
}

void DrawRoutine::Draw(const glm::mat4& vp)
{
    assert( mId == 0 );
    mpGeometry->Render( vp );
}

void DrawRoutine::updateTransformAttrib()
{
    // TODO update just the transform
    geometry< f32_attribs, u32_attribs >()->UpdateAttributesF32( mId );
}











float DrawLine::LineBetween(const Vec3& p1, const Vec3& p2)
{
    Vec3 pos( glm::mix(p1, p2, 0.5f) );
    Vec3 diff( p1 - p2 );
    Quat rot = glm::rotation( Vec3(0.0f, 1.0f, 0.0f), glm::normalize(diff) );
    float len = glm::length( diff );

    Scale( len );
    SetOrientation( rot );
    SetPosition( pos );

    return len;
}

PBError DrawLine::defineGeometry(DrawRoutine::geometryInfo& info)
{
    glm::vec3 srcPos[] = {

        glm::vec3( 0.0f, -0.5f, 0.0f ),
        glm::vec3( 0.0f, 0.5f, 0.0f )

    };

    uint32_t srcColors[] = {
        //ABGR
        0x02020000,
        0x02080004
    };

    uint32_t srcIdx[] = { 0, 1 };


    scoped_ptr<Positions> pPositions( new Positions(srcPos, srcPos + sizeof(srcPos) / sizeof(srcPos[0])) );
    scoped_ptr<ColorsRGBA> pColors( new ColorsRGBA(srcColors, srcColors + sizeof(srcColors) / sizeof(srcColors[0])) );
    scoped_ptr<Indices> pIndices( new Indices( srcIdx, srcIdx + sizeof(srcIdx) / sizeof(srcIdx[0])) );

    if ( !pPositions || !pColors || !pIndices )
    {
        PB_RETURN_ALLOCFAIL();
    }

    info.pPositions = pPositions.Disown();
    info.pColors = pColors.Disown();
    info.pIndices = pIndices.Disown();
    info.primitiveType = GL_LINE_STRIP;

    return PB_ERR_OK;
}






}
