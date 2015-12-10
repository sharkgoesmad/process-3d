#ifndef INSTANCEDMESH_H
#define INSTANCEDMESH_H

#include <list>
#include "core/glmcfg.h"
#include "core/pbutil.h"
#include "geometry.h"
#include "instanceattribsmgr.h"

namespace pb
{

template < typename f32_layout, typename uint32_layout >
class InstancedGeometry : public IInstancedGeometry
{

/*
    struct transformBufferState
    {
        unsigned int idVBO;
        unsigned int count;
    };

    struct transformAttribMgr
    {
        const unsigned int MAX_TRANSFORMS_PER_ENTRY;
        unsigned int mCount;
        std::vector<Transforms> mTransforms;
        std::vector<transformBufferState> mStates;

        transformAttribMgr( unsigned int hintPerInstanceTableSize ) :
            MAX_TRANSFORMS_PER_ENTRY( hintPerInstanceTableSize ),
            mCount( 0 )
        {
                mTransforms.emplace_back( MAX_TRANSFORMS_PER_ENTRY, Mat4(1.0f) );
                mStates.emplace_back( transformBufferState() );
        }

        size_t EntryBufferSize()
        {
            return MAX_TRANSFORMS_PER_ENTRY * sizeof( Mat4 );
        }

        inline Transforms& EntryBuffer(unsigned int entryId)
        {
            return mTransforms[ entryId ];
        }

        inline transformBufferState& EntryState(unsigned int entryId)
        {
            return mStates[ entryId ];
        }

        inline unsigned int EntriesCount()
        {
            return mTransforms.size();
        }

        unsigned int AddInstance()
        {
           transformBufferState* pState = &mStates[ EntriesCount()-1 ];

           if ( pState->count >= MAX_TRANSFORMS_PER_ENTRY )
           {
               mTransforms.emplace_back( MAX_TRANSFORMS_PER_ENTRY, Mat4(1.0f) );
               mStates.emplace_back( transformBufferState() );

               pState = &mStates[ EntriesCount()-1 ];
               Transforms& transforms = mTransforms[ EntriesCount()-1 ];
               CreateBuffer( *pState, transforms );
           }

           ++pState->count;
           return mCount++;
        }

        void CreateBuffer(transformBufferState& state, Transforms& transforms);

        inline Mat4* Transform(unsigned int transformId)
        {
            unsigned int entryId = transformId / MAX_TRANSFORMS_PER_ENTRY;
            unsigned int transformOffset = transformId % MAX_TRANSFORMS_PER_ENTRY;
            return &mTransforms[ entryId ][ transformOffset ];
        }

        inline void UpdateTransform(unsigned int transformId);

    };


*/

private:

    InstancedGeometry(const InstancedGeometry& rhs);
    InstancedGeometry& operator=(const InstancedGeometry& rhs);

public:

    InstancedGeometry(unsigned int primitiveType,
                    Indices* pIndicies,
                    Positions* pPositions,
                    ColorsRGBA* pColors,
                    unsigned int idProgram,
                    unsigned int hintPerInstanceTableSize);
    virtual ~InstancedGeometry();

    unsigned int AddInstance();

    f32_layout* AttributesF32(unsigned int id);
    void UpdateAttributesF32(unsigned int transformId);
    uint32_layout* AttributesU32(unsigned int id);
    void UpdateAttributesU32(unsigned int transformId);

    void UseF32Layout(const attribDesc* attribDescs, int n);
    void UseUint32Layout(const attribDesc* attribDescs, int n);

    void Render(const glm::mat4& vp);

private:

    PBError init();

private:

    PBError mStatus;
    unsigned int mPrimitiveType;
    unsigned int mIdProgram;

    // TODO refactor to use packed attributes
    unsigned int mIdPositionVBO;
    unsigned int mIdColorVBO;
    unsigned int mIdIndexArray;
    unsigned int mIdVAO;

    // backing buffers
    Indices* mpBkIndicies;
    Positions* mpBkPositions;
    ColorsRGBA* mpBkColors;
    InstanceAttribsMgr< f32_layout, uint32_layout > mInstancedAttribsMgr;

};


template < typename f32_layout, typename uint32_layout >
InstancedGeometry< f32_layout, uint32_layout >::InstancedGeometry(GLuint primitiveType,
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
    mInstancedAttribsMgr( hintPerInstanceTableSize )
{
    GLint attribLocation;

    Indices& indicies = *mpBkIndicies;
    Positions& positions = *mpBkPositions;
    ColorsRGBA& colors = *mpBkColors;

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
    //mTransformsMgr.CreateBuffer( transformsState, transforms );


    mStatus = PB_ERR;
}

template < typename f32_layout, typename uint32_layout >
InstancedGeometry< f32_layout, uint32_layout >::~InstancedGeometry()
{
    PB_DELETE( mpBkIndicies );
    PB_DELETE( mpBkPositions );
    PB_DELETE( mpBkColors );

    glDeleteProgram( mIdProgram );
    glDeleteBuffers( 1, &mIdPositionVBO );
    glDeleteBuffers( 1, &mIdColorVBO );
    glDeleteBuffers( 1, &mIdIndexArray );
    glDeleteVertexArrays( 1, &mIdVAO );
}

template < typename f32_layout, typename uint32_layout >
unsigned int InstancedGeometry< f32_layout, uint32_layout >::AddInstance()
{
    return mInstancedAttribsMgr.AddInstance();
}

template < typename f32_layout, typename uint32_layout >
f32_layout* InstancedGeometry< f32_layout, uint32_layout >::AttributesF32(unsigned int id)
{
    return mInstancedAttribsMgr.AttributesF32( id );
}

template < typename f32_layout, typename uint32_layout >
void InstancedGeometry< f32_layout, uint32_layout >::UpdateAttributesF32(unsigned int id)
{
    mInstancedAttribsMgr.UpdateF32( id );
}

template < typename f32_layout, typename uint32_layout >
uint32_layout* InstancedGeometry< f32_layout, uint32_layout >::AttributesU32(unsigned int id)
{
    return mInstancedAttribsMgr.AttributesU32( id );
}

template < typename f32_layout, typename uint32_layout >
void InstancedGeometry< f32_layout, uint32_layout >::UpdateAttributesU32(unsigned int id)
{
    mInstancedAttribsMgr.UpdateU32( id );
}

template < typename f32_layout, typename uint32_layout >
void InstancedGeometry< f32_layout, uint32_layout >::UseF32Layout(const attribDesc* attribDescs, int n)
{
    mInstancedAttribsMgr.UseF32Layout( attribDescs, n );
}

template < typename f32_layout, typename uint32_layout >
void InstancedGeometry< f32_layout, uint32_layout >::UseUint32Layout(const attribDesc* attribDescs, int n)
{
    mInstancedAttribsMgr.UseUint32Layout( attribDescs, n );
}

template < typename f32_layout, typename uint32_layout >
void InstancedGeometry< f32_layout, uint32_layout >::Render(const glm::mat4& vp)
{
    GLint uniformVpLocation;

    glUseProgram( mIdProgram );
    glBindVertexArray( mIdVAO );

    // set vp matrix
    uniformVpLocation = glGetUniformLocation( mIdProgram, "uniformVP" );
    glUniformMatrix4fv( uniformVpLocation, 1, GL_FALSE, &vp[0][0] );

    GLint attribLocation = 2;

    //glDrawElements( mPrimitiveType, mpBkIndicies->size(), GL_UNSIGNED_INT, (void*)0 );
//    for ( unsigned int idx = 0; idx < mTransformsMgr.EntriesCount(); ++idx )
//    {
//        transformBufferState& state = mTransformsMgr.EntryState( idx );
//
//        GLint attribLocation = 2;
//        glEnableVertexAttribArray( attribLocation );
//        glEnableVertexAttribArray( attribLocation+1 );
//        glEnableVertexAttribArray( attribLocation+2 );
//        glEnableVertexAttribArray( attribLocation+3 );
//        glBindBuffer( GL_ARRAY_BUFFER, state.idVBO );
//        glVertexAttribPointer( attribLocation+0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)0 );
//        glVertexAttribPointer( attribLocation+1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 4) );
//        glVertexAttribPointer( attribLocation+2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 8) );
//        glVertexAttribPointer( attribLocation+3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 12) );
//        glVertexAttribDivisor( attribLocation+0, 1 );
//        glVertexAttribDivisor( attribLocation+1, 1 );
//        glVertexAttribDivisor( attribLocation+2, 1 );
//        glVertexAttribDivisor( attribLocation+3, 1 );
//
//        glDrawElementsInstanced( mPrimitiveType, mpBkIndicies->size(), GL_UNSIGNED_INT, (void*)0, state.count );
//
//        glDisableVertexAttribArray( attribLocation );
//        glDisableVertexAttribArray( attribLocation+1 );
//        glDisableVertexAttribArray( attribLocation+2 );
//        glDisableVertexAttribArray( attribLocation+3 );
//    }

    for ( unsigned int idx = 0; idx < mInstancedAttribsMgr.BuffersCount(); ++idx )
    {
        mInstancedAttribsMgr.UpdateBufferF32( idx );
        mInstancedAttribsMgr.UpdateBufferU32( idx );

        mInstancedAttribsMgr.Bind( idx, attribLocation );
        glDrawElementsInstanced(
            mPrimitiveType,
            mpBkIndicies->size(),
            GL_UNSIGNED_INT,
            (void*)0,
            mInstancedAttribsMgr.BufferState( idx )->count );
    }

    glBindVertexArray( 0 );
}


}

#endif // INSTANCEDMESH_H
