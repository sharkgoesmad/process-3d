#include <GL/gl.h>
#include <shadercompiler.h>
#include <pbconfig.h>
#include <instancedmesh.h>
#include "formsphere.h"

namespace pb
{

using namespace reas;


void create(Positions& vertices, Indices& indices);


FormSphere::FormSphere(const Vec3& pos, const Vec3& dir) :
    Form( pos, dir )
{
    SetPosition( position );
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
    pSrcCol->assign( pSrcPos->size(), 0x02FFFFFF );

    scoped_ptr<Positions> pPositions( pSrcPos );
    scoped_ptr<ColorsRGBA> pColors( pSrcCol );
    scoped_ptr<Indices> pIndices( pSrcInd );

    unsigned int idVs, idFs, idProgram;
    idVs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/sphere_vs.glsl"), ShaderCompiler::Shader_Vertex );
    idFs = ShaderCompiler::CompileFromFile( PB_RESOURCE_PATH("shaders/sphere_fs.glsl"), ShaderCompiler::Shader_Fragment );
    idProgram = ShaderCompiler::Program( idVs, idFs );

    if ( idProgram == PB_GL_INVALID_ID )
    {
        Log::Error( "Failed creating program" );
        return PB_ERR;
    }

    mpMesh.reset( new InstancedMesh(
        GL_TRIANGLES,
        pIndices.Disown(),
        pPositions.Disown(),
        pColors.Disown(),
        idProgram,
        mHintPerInstanceTableSize) );

    if ( mpMesh == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    return InstancedObject::init();
}

bool FormSphere::CollidesWith(Form* pForm)
{
    FormSphere* pSF = static_cast<FormSphere*>( pForm );
    Vec3 dist( position - pSF->position );
    float radii = mRadius + pSF->mRadius;

    return glm::length( dist ) <= radii;
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

