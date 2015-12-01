#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/mat4x4.hpp>
#include "core/log.h"
#include "instancedmesh.h"

#include <list>

namespace pb
{

void InstancedMesh::transformAttribMgr::CreateBuffer(transformBufferState& state, Transforms& transforms)
{
    glGenBuffers( 1, &state.idVBO );
    glBindBuffer( GL_ARRAY_BUFFER, state.idVBO );
    glBufferData( GL_ARRAY_BUFFER, EntryBufferSize(), &transforms[0][0][0], GL_DYNAMIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void InstancedMesh::transformAttribMgr::UpdateTransform(unsigned int transformId)
{
    unsigned int entryId = transformId / MAX_TRANSFORMS_PER_ENTRY;
    unsigned int transformOffset = transformId % MAX_TRANSFORMS_PER_ENTRY;

    transformBufferState& state = EntryState( entryId );
    glBindBuffer( GL_ARRAY_BUFFER, state.idVBO );
    glBufferSubData(
        GL_ARRAY_BUFFER,
        transformOffset * sizeof(Mat4),
        sizeof(Mat4),
        (void*)&mTransforms[ entryId ][ transformOffset ][ 0 ][ 0 ]);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

InstancedMesh::InstancedMesh(GLuint primitiveType,
    Indices* pIndicies,
    Positions* pPositions,
    ColorsRGBA* pColors,
    unsigned int idProgram,
    unsigned int hintPerInstanceTableSize) :

    mStatus( PB_ERR_OK ),
    mPrimitiveType( primitiveType ),
    mIdProgram( idProgram ),
    mpBkIndicies( pIndicies ),
    mpBkPositions( pPositions ),
    mpBkColors( pColors ),
    mTransformsMgr( hintPerInstanceTableSize )
{
    GLint attribLocation;

    Indices& indicies = *mpBkIndicies;
    Positions& positions = *mpBkPositions;
    ColorsRGBA& colors = *mpBkColors;
    Transforms& transforms = mTransformsMgr.EntryBuffer( 0 );
    transformBufferState& transformsState = mTransformsMgr.EntryState( 0 );

    if ( positions.size() != colors.size() )
    {
        Log::Error( "Buffers are not of the same size" );
        mStatus = PB_ERR;
        return;
    }

    Log::Info("length: "+std::to_string(indicies.size()));

    /*** VAO ***/
    glGenVertexArrays( 1, &mIdVAO );
    glBindVertexArray( mIdVAO );


    /*** position ***/

    // data
    glGenBuffers( 1, &mIdPositionVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mIdPositionVBO );
    glBufferData( GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW );

    // layout
    attribLocation = 0;//glGetAttribLocation( mIdProgram, "attribPosition" );
    glEnableVertexAttribArray( attribLocation );
    glBindBuffer( GL_ARRAY_BUFFER, mIdPositionVBO );
    glVertexAttribPointer( attribLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );


    /*** color ***/

    // data
    glGenBuffers( 1, &mIdColorVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mIdColorVBO );
    glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW );

    // layout
    attribLocation = 1;//glGetAttribLocation( mIdProgram, "attribColor" );
    glEnableVertexAttribArray( attribLocation );
    glBindBuffer( GL_ARRAY_BUFFER, mIdColorVBO );
    glVertexAttribPointer( attribLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0 );


    /*** indecies ***/

    // data
    glGenBuffers( 1, &mIdIndexArray );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIdIndexArray );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(indicies[0]), &indicies[0], GL_STATIC_DRAW );


    // unbind VAO
    glBindVertexArray( 0 );


    // outside VAO state
    mTransformsMgr.CreateBuffer( transformsState, transforms );


    mStatus = PB_ERR;
}

InstancedMesh::~InstancedMesh()
{
    PB_DELETE( mpBkIndicies );
    PB_DELETE( mpBkPositions );
    PB_DELETE( mpBkColors );
}

unsigned int InstancedMesh::AddInstance()
{
    return mTransformsMgr.AddInstance();
}

Mat4* InstancedMesh::Transform(unsigned int id)
{
    return mTransformsMgr.Transform( id );
}

void InstancedMesh::UpdateTransform(unsigned int transformId)
{
    mTransformsMgr.UpdateTransform( transformId );
}

void InstancedMesh::Render(const glm::mat4& vp)
{
    GLint uniformVpLocation;

    glUseProgram( mIdProgram );
    glBindVertexArray( mIdVAO );

    // set vp matrix
    uniformVpLocation = glGetUniformLocation( mIdProgram, "uniformVP" );
    glUniformMatrix4fv( uniformVpLocation, 1, GL_FALSE, &vp[0][0] );

    //glDrawElements( mPrimitiveType, mpBkIndicies->size(), GL_UNSIGNED_INT, (void*)0 );
    for ( unsigned int idx = 0; idx < mTransformsMgr.EntriesCount(); ++idx )
    {
        transformBufferState& state = mTransformsMgr.EntryState( idx );

        GLint attribLocation = 2;
        glEnableVertexAttribArray( attribLocation );
        glEnableVertexAttribArray( attribLocation+1 );
        glEnableVertexAttribArray( attribLocation+2 );
        glEnableVertexAttribArray( attribLocation+3 );
        glBindBuffer( GL_ARRAY_BUFFER, state.idVBO );
        glVertexAttribPointer( attribLocation+0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)0 );
        glVertexAttribPointer( attribLocation+1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 4) );
        glVertexAttribPointer( attribLocation+2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 8) );
        glVertexAttribPointer( attribLocation+3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 12) );
        glVertexAttribDivisor( attribLocation+0, 1 );
        glVertexAttribDivisor( attribLocation+1, 1 );
        glVertexAttribDivisor( attribLocation+2, 1 );
        glVertexAttribDivisor( attribLocation+3, 1 );

        glDrawElementsInstanced( mPrimitiveType, mpBkIndicies->size(), GL_UNSIGNED_INT, (void*)0, state.count );

        glDisableVertexAttribArray( attribLocation );
        glDisableVertexAttribArray( attribLocation+1 );
        glDisableVertexAttribArray( attribLocation+2 );
        glDisableVertexAttribArray( attribLocation+3 );
    }

    glBindVertexArray( 0 );
}


}

