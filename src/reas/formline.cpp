#include <shadercompiler.h>
#include <glm/gtc/random.hpp>
#include <pbconfig.h>
#include <instancedgeometry.h>
#include "formline.h"

namespace pb
{

using namespace reas;

struct f32_attribs
{
    Mat4 transform;
};

typedef uint32_t u32_attribs;

static void create(Positions& vertices, Indices& indices);

FormLine::FormLine(Progress* pProgress, const Vec3& pos, const Vec3& dir, float speed, float angSpeed, float halfExtent) :
    Form( pProgress, pos, dir, speed, angSpeed ),
    length( halfExtent )
{
    // TODO
    //length = glm::linearRand( 1.0f, 3.0f );
    //radius = 1.0f;
    Scale( length );
}

FormLine::~FormLine()
{
}

PBError FormLine::init()
{
    //create mpMesh
    Positions* pSrcPos = new Positions();
    Indices* pSrcInd = new Indices();
    ColorsRGBA* pSrcCol = new ColorsRGBA();

    if ( !pSrcPos || !pSrcCol || !pSrcInd )
    {
        PB_RETURN_ALLOCFAIL();
    }

    create( *pSrcPos, *pSrcInd );
    pSrcCol->assign( pSrcPos->size(), 0x010000FF );

    scoped_ptr<Positions> pPositions( pSrcPos );
    scoped_ptr<ColorsRGBA> pColors( pSrcCol );
    scoped_ptr<Indices> pIndices( pSrcInd );

    unsigned int idVs, idFs, idProgram;
    idVs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/sphere_vs.glsl"), ShaderCompiler::Shader_Vertex );
    idFs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/sphere_fs.glsl"), ShaderCompiler::Shader_Fragment );
    idProgram = ShaderCompiler::Program( idVs, idFs );

    ShaderCompiler::DeleteShader( idVs );
    ShaderCompiler::DeleteShader( idFs );

    if ( idProgram == PB_GL_INVALID_ID )
    {
        Log::Error( "Failed creating program" );
        return PB_ERR;
    }

    mpGeometry.reset( new InstancedGeometry< f32_attribs, u32_attribs >(
        GL_LINES,
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
    f32_attribs* attribs = geometry< f32_attribs, u32_attribs >()->AttributesF32( mId );
    mpTransform = &attribs->transform;

    attribDesc f32attribDesc[] = { { attribDesc::SizeMat4 } };
    geometry< f32_attribs, u32_attribs >()->UseF32Layout( f32attribDesc, 1 );

    return InstancedObject::init();
}

void FormLine::Update()
{
    LookAt( Position() + direction );

    Form::Update();
}


void FormLine::MakeAnother(InstancedObject* pAnother)
{
    assert( mId == 0 );

    FormLine* pOther = static_cast< FormLine* >( pAnother );

    pOther->mpGeometry = mpGeometry;
    pOther->mId = mpGeometry->AddInstance();
    f32_attribs* attribs = geometry< f32_attribs, u32_attribs >()->AttributesF32( pOther->mId );
    pOther->mpTransform = &attribs->transform;
}

void FormLine::updateTransformAttrib()
{
    // TODO update just the transform
    geometry< f32_attribs, u32_attribs >()->UpdateAttributesF32( mId );
}

void create(Positions& vertices, Indices& indices)
{
    vertices.push_back( Vec3(0.0f, 0.0f, -0.5f) );
    vertices.push_back( Vec3(0.0f, 0.0f, 0.5f) );

    indices.push_back( 0 );
    indices.push_back( 1 );
}


}

