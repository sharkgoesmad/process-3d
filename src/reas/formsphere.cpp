#include <shadercompiler.h>
#include <glm/gtc/random.hpp>
#include <pbconfig.h>
#include <instancedgeometry.h>
#include "formsphere.h"

namespace pb
{

using namespace reas;


struct f32_attribs
{
    Mat4 transform;
};

typedef uint32_t u32_attribs;


static void create(Positions& vertices, Indices& indices);


FormSphere::FormSphere(
    Progress* pProgress,
    const Vec3& pos,
    const Vec3& dir,
    float speed,
    float angSpeed,
    float halfExtent) :
    Form( pProgress, pos, dir, speed, angSpeed ),
    radius( halfExtent )
{
    // TODO
    //radius = glm::linearRand( 0.1f, 2.0f );
    //radius = 1.0f;
    Scale( radius );
}

FormSphere::~FormSphere()
{
}

PBError FormSphere::init()
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
        GL_POINTS,
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


void FormSphere::MakeAnother(InstancedObject* pAnother)
{
    assert( mId == 0 );

    FormSphere* pOther = static_cast< FormSphere* >( pAnother );

    pOther->mpGeometry = mpGeometry;
    pOther->mId = mpGeometry->AddInstance();
    f32_attribs* attribs = geometry< f32_attribs, u32_attribs >()->AttributesF32( pOther->mId );
    pOther->mpTransform = &attribs->transform;
}

void FormSphere::updateTransformAttrib()
{
    // TODO update just the transform
    geometry< f32_attribs, u32_attribs >()->UpdateAttributesF32( mId );
}

// https://gist.github.com/zwzmzd/0195733fa1210346b00d
void create(Positions& vertices, Indices& indices)
{
    int lats = 40;
    int longs = 40;

    int i, j;
    int indicator = 0;
    for(i = 0; i <= lats; i++)
    {
        double lat0 = glm::pi<double>() * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);

        double lat1 = glm::pi<double>() * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        for(j = 0; j <= longs; j++)
        {
            double lng = 2 * glm::pi<double>() * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            vertices.push_back( Vec3(x * zr0, y * zr0, z0) );
            indices.push_back(indicator);
            indicator++;

            vertices.push_back( Vec3(x * zr1, y * zr1, z1) );
            indices.push_back(indicator);
            indicator++;
        }
       //indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
   }
}


}

