#include <GL/glew.h>
#include <GL/gl.h>
#include "core/log.h"
#include "instancedmesh.h"

namespace pb
{

InstancedMesh::InstancedMesh(GLuint primitiveType,
    Indicies& indicies,
    Positions& positions,
    ColorsRGBA& colors,
    unsigned int idProgram) :

    mStatus( PB_ERR_OK ),
    mPrimitiveType( primitiveType ),
    mIdProgram( idProgram )
{
    unsigned int attribLocation;
    
    
    if ( positions.size() != colors.size() )
    {
        Log::Error( "Buffers are not of the same size" );
        mStatus = PB_ERR;
        return;
    }
    
    
    /*** VAO ***/
    glGenVertexArrays( 1, &mIdVAO );
    glBindVertexArray( mIdVAO );
    
    
    /*** position ***/
    
    // data
    glGenBuffers( 1, &mIdPositionVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mIdPositionVBO );
    glBufferData( GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW );
    
    // layout
    attribLocation = glGetAttribLocation( mIdProgram, "attribPosition" );
    glEnableVertexAttribArray( attribLocation );
    glBindBuffer( GL_ARRAY_BUFFER, mIdPositionVBO );
    glVertexAttribPointer( attribLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    
    
    /*** color ***/
    
    // data
    glGenBuffers( 1, &mIdColorVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mIdColorVBO );
    glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW );
    
    // layout
    attribLocation = glGetAttribLocation( mIdProgram, "attribColor" );
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
    
    mStatus = PB_ERR;
}

InstancedMesh::~InstancedMesh()
{
}


}

