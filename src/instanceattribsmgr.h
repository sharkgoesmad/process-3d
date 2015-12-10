#ifndef PB_INSTANCEATTRIBSMGR_H
#define PB_INSTANCEATTRIBSMGR_H

#include <cstring>
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include "core/pbutil.h"

namespace pb
{

struct attribDesc
{
    size_t attribSize;

    static constexpr size_t SizeMat4 = 4 * 4 * sizeof( float );
    static constexpr size_t SizeRGBA = sizeof( uint32_t );
};

template < typename f32_layout, typename uint32_layout >
class InstanceAttribsMgr
{

private:

    struct accumUpdate
    {
        unsigned int startOffset;
        unsigned int size;

        accumUpdate() :
            startOffset( 0 ),
            size( 0 )
        {}
    };

    struct attribBufferState
    {
        unsigned int idF32VBO;
        unsigned int idU32VBO;
        unsigned int count;
        accumUpdate mF32updateAccum;
        accumUpdate mU32updateAccum;

        attribBufferState() :
            count( 0 ),
            idF32VBO( 0 ),
            idU32VBO( 0 )
        {}
    };

private:

    InstanceAttribsMgr(const InstanceAttribsMgr& rhs);
    InstanceAttribsMgr& operator=(const InstanceAttribsMgr& rhs);

public:

    InstanceAttribsMgr(unsigned int hintNumInstancesPerBuffer);
    ~InstanceAttribsMgr();

    void UseF32Layout(const attribDesc* attribDescs, int n);
    void UseUint32Layout(const attribDesc* attribDescs, int n);

    attribBufferState* BufferState(unsigned int id);
    unsigned int BuffersCount();
    unsigned int AddInstance();

    PBError Bind(unsigned int bufferId, unsigned int startAttribLocation);

    f32_layout* AttributesF32(unsigned int instanceId);
    uint32_layout* AttributesU32(unsigned int instanceId);

    void UpdateF32(unsigned int instanceId);
    void UpdateU32(unsigned int instanceId);

    void UpdateBufferF32(unsigned int bufferId);
    void UpdateBufferU32(unsigned int bufferId);

private:

    PBError makeF32buffer(f32_layout*& pBuffer);
    PBError makeUint32buffer(uint32_layout*& pBuffer);
    PBError createVBOs(attribBufferState& state, f32_layout* f32Input, uint32_layout* u32Input);
    size_t f32bufferSize();
    size_t uint32bufferSize();

private:

    const unsigned int mNumInstancesPerBuffer;
    unsigned int mTotalInstanceCount;
    std::vector< attribBufferState > mStates;
    std::vector< f32_layout* > mF32AttribsBuffers;
    std::vector< uint32_layout* > mUint32AttribsBuffers;

    attribDesc* mF32AttribsDesc;
    unsigned int mNumF32AttribsDesc;
    attribDesc* mUint32AttribsDesc;
    unsigned int mNumUint32AttribsDesc;

};

template < typename f32_layout, typename uint32_layout >
InstanceAttribsMgr< f32_layout, uint32_layout >::InstanceAttribsMgr(unsigned int hintNumInstancesPerBuffer) :
    mNumInstancesPerBuffer( hintNumInstancesPerBuffer ),
    mTotalInstanceCount( 0 ),
    mF32AttribsDesc( NULL ),
    mNumF32AttribsDesc( 0 ),
    mUint32AttribsDesc( NULL ),
    mNumUint32AttribsDesc( 0 )
{
}

template < typename f32_layout, typename uint32_layout >
InstanceAttribsMgr< f32_layout, uint32_layout >::~InstanceAttribsMgr()
{
    PB_DELETE_ARRAY( mF32AttribsDesc );
    PB_DELETE_ARRAY( mUint32AttribsDesc );

    for ( int idx = 0; idx < mStates.size(); ++idx )
    {
        glDeleteBuffers( 1, &mStates[ idx ].idF32VBO );
        glDeleteBuffers( 1, &mStates[ idx ].idU32VBO );

        PB_DELETE_ARRAY( mF32AttribsBuffers[ idx ] );
        PB_DELETE_ARRAY( mUint32AttribsBuffers[ idx ] );
    }
}

template < typename f32_layout, typename uint32_layout >
PBError InstanceAttribsMgr< f32_layout, uint32_layout >::makeF32buffer(f32_layout*& pBuffer)
{
    if ( (pBuffer = new f32_layout[ mNumInstancesPerBuffer ]) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }
}

template < typename f32_layout, typename uint32_layout >
PBError InstanceAttribsMgr< f32_layout, uint32_layout >::makeUint32buffer(uint32_layout*& pBuffer)
{
    if ( (pBuffer = new uint32_layout[ mNumInstancesPerBuffer ]) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }
}

template < typename f32_layout, typename uint32_layout >
PBError InstanceAttribsMgr< f32_layout, uint32_layout >::createVBOs(
    attribBufferState& state,
    f32_layout* f32Input,
    uint32_layout* u32Input)
{
    glGenBuffers( 1, &state.idF32VBO );
    glGenBuffers( 1, &state.idU32VBO );

    glBindBuffer( GL_ARRAY_BUFFER, state.idF32VBO );
    glBufferData( GL_ARRAY_BUFFER, f32bufferSize(), NULL, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, state.idU32VBO );
    glBufferData( GL_ARRAY_BUFFER, uint32bufferSize(), NULL, GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

template < typename f32_layout, typename uint32_layout >
inline size_t InstanceAttribsMgr< f32_layout, uint32_layout >::f32bufferSize()
{
    return mNumInstancesPerBuffer * sizeof( f32_layout );
}

template < typename f32_layout, typename uint32_layout >
inline size_t InstanceAttribsMgr< f32_layout, uint32_layout >::uint32bufferSize()
{
    return mNumInstancesPerBuffer * sizeof( uint32_layout );
}

template < typename f32_layout, typename uint32_layout >
void InstanceAttribsMgr< f32_layout, uint32_layout >::UseF32Layout(const attribDesc* attribDescs, int n)
{
    mNumF32AttribsDesc = n;
    if ( (mF32AttribsDesc = new attribDesc[ n ]) == NULL )
    {
        // TODO
    }

    memcpy( mF32AttribsDesc, attribDescs, n * sizeof(attribDesc) );
}

template < typename f32_layout, typename uint32_layout >
void InstanceAttribsMgr< f32_layout, uint32_layout >::UseUint32Layout(const attribDesc* attribDescs, int n)
{
    mNumUint32AttribsDesc = n;
    if ( (mUint32AttribsDesc = new attribDesc[ n ]) == NULL )
    {
        // TODO
    }

    memcpy( mUint32AttribsDesc, attribDescs, n * sizeof(attribDesc) );
}

template < typename f32_layout, typename uint32_layout >
typename InstanceAttribsMgr< f32_layout, uint32_layout >::attribBufferState*
InstanceAttribsMgr< f32_layout, uint32_layout >::BufferState(unsigned int id)
{
    if ( BuffersCount() == 0 )
    {
        return NULL;
    }

    return &mStates[ id ];
}

template < typename f32_layout, typename uint32_layout >
unsigned int InstanceAttribsMgr< f32_layout, uint32_layout >::BuffersCount()
{
    return mStates.size();
}

template < typename f32_layout, typename uint32_layout >
unsigned int InstanceAttribsMgr< f32_layout, uint32_layout >::AddInstance()
{
    attribBufferState* pState = BufferState( BuffersCount()-1 );

    if ( pState == NULL || pState->count >= mNumInstancesPerBuffer )
    {
        f32_layout* pF32Bfr;
        uint32_layout* pU32Bfr;

        makeF32buffer( pF32Bfr );
        makeUint32buffer( pU32Bfr );

        mF32AttribsBuffers.push_back( pF32Bfr );
        mUint32AttribsBuffers.push_back( pU32Bfr );
        mStates.emplace_back( attribBufferState() );

        pState = &mStates[ BuffersCount()-1 ];
        createVBOs( *pState, pF32Bfr, pU32Bfr );

        Log::Info("buffers: " + STR( BuffersCount() ) + "    instances: " + STR( mTotalInstanceCount+1 ) );
    }

    ++pState->count;
    return mTotalInstanceCount++;
}

template < typename f32_layout, typename uint32_layout >
PBError InstanceAttribsMgr< f32_layout, uint32_layout >::Bind(unsigned int bufferId, unsigned int startAttribLocation)
{
    unsigned int attribLocation = startAttribLocation;
    attribBufferState& state = mStates[ bufferId ];


    glBindBuffer( GL_ARRAY_BUFFER, state.idF32VBO );
    uint8_t* attribPtr = 0;
    for ( unsigned int idx = 0; idx < mNumF32AttribsDesc; ++idx )
    {
        switch ( mF32AttribsDesc[ idx ].attribSize )
        {
            case attribDesc::SizeMat4:
                glEnableVertexAttribArray( attribLocation+0 );
                glEnableVertexAttribArray( attribLocation+1 );
                glEnableVertexAttribArray( attribLocation+2 );
                glEnableVertexAttribArray( attribLocation+3 );

                glVertexAttribPointer( attribLocation+0, 4, GL_FLOAT, GL_FALSE, sizeof(f32_layout), attribPtr );
                glVertexAttribPointer( attribLocation+1, 4, GL_FLOAT, GL_FALSE, sizeof(f32_layout), attribPtr + (sizeof(float) * 4) );
                glVertexAttribPointer( attribLocation+2, 4, GL_FLOAT, GL_FALSE, sizeof(f32_layout), attribPtr + (sizeof(float) * 8) );
                glVertexAttribPointer( attribLocation+3, 4, GL_FLOAT, GL_FALSE, sizeof(f32_layout), attribPtr + (sizeof(float) * 12) );

                glVertexAttribDivisor( attribLocation+0, 1 );
                glVertexAttribDivisor( attribLocation+1, 1 );
                glVertexAttribDivisor( attribLocation+2, 1 );
                glVertexAttribDivisor( attribLocation+3, 1 );

                attribPtr += attribDesc::SizeMat4;
                attribLocation += 4;
                break;

            default:
                Log::Error( "Unsupported f32 vertex attribute size: " +
                    std::to_string(mF32AttribsDesc[ idx ].attribSize) );
                return PB_ERR;
        }
    }


    glBindBuffer( GL_ARRAY_BUFFER, state.idU32VBO );
    attribPtr = 0;
    for ( unsigned int idx = 0; idx < mNumUint32AttribsDesc; ++idx )
    {
        switch ( mUint32AttribsDesc[ idx ].attribSize )
        {
            case attribDesc::SizeRGBA:
                glEnableVertexAttribArray( attribLocation );
                glVertexAttribPointer( attribLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(uint32_layout), attribPtr );
                glVertexAttribDivisor( attribLocation, 1 );

                attribPtr += attribDesc::SizeRGBA;
                attribLocation += 1;
                break;

            default:
                Log::Error( "Unsupported u32 vertex attribute size: " +
                    std::to_string(mUint32AttribsDesc[ idx ].attribSize) );
                return PB_ERR;
        }
    }

    return PB_ERR_OK;
}

template < typename f32_layout, typename uint32_layout >
inline f32_layout* InstanceAttribsMgr< f32_layout, uint32_layout >::AttributesF32(unsigned int instanceId)
{
    unsigned int bufferId = instanceId / mNumInstancesPerBuffer;
    unsigned int attributesOffset = instanceId % mNumInstancesPerBuffer;
    return &mF32AttribsBuffers[ bufferId ][ attributesOffset ];
}

template < typename f32_layout, typename uint32_layout >
inline uint32_layout* InstanceAttribsMgr< f32_layout, uint32_layout >::AttributesU32(unsigned int instanceId)
{
    unsigned int bufferId = instanceId / mNumInstancesPerBuffer;
    unsigned int attributesOffset = instanceId % mNumInstancesPerBuffer;
    return &mUint32AttribsBuffers[ bufferId ][ attributesOffset ];
}

template < typename f32_layout, typename uint32_layout >
void InstanceAttribsMgr< f32_layout, uint32_layout >::UpdateF32(unsigned int instanceId)
{
    unsigned int bufferId = instanceId / mNumInstancesPerBuffer;
    unsigned int attributesOffset = instanceId % mNumInstancesPerBuffer;
    attribBufferState& state = mStates[ bufferId ];

    if ( state.mF32updateAccum.startOffset + state.mF32updateAccum.size == attributesOffset )
    {
        ++state.mF32updateAccum.size;
        return;
    }
    else
    {
        UpdateBufferF32( bufferId );
        state.mF32updateAccum.startOffset += attributesOffset + 1;
    }

    glBindBuffer( GL_ARRAY_BUFFER, state.idF32VBO );
    glBufferSubData(
        GL_ARRAY_BUFFER,
        attributesOffset * sizeof( f32_layout ),
        sizeof( f32_layout ),
        (void*)&mF32AttribsBuffers[ bufferId ][ attributesOffset ]);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

template < typename f32_layout, typename uint32_layout >
void InstanceAttribsMgr< f32_layout, uint32_layout >::UpdateBufferF32(unsigned int bufferId)
{
    attribBufferState& state = mStates[ bufferId ];

    if ( state.mF32updateAccum.size == 0 )
    {
        return;
    }

    glBindBuffer( GL_ARRAY_BUFFER, state.idF32VBO );
    glBufferSubData(
        GL_ARRAY_BUFFER,
        state.mF32updateAccum.startOffset * sizeof( f32_layout ),
        state.mF32updateAccum.size * sizeof( f32_layout ),
        (void*)&mF32AttribsBuffers[ bufferId ][ state.mF32updateAccum.startOffset ]);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    state.mF32updateAccum.size = 0;
    state.mF32updateAccum.startOffset = 0;
}

template < typename f32_layout, typename uint32_layout >
void InstanceAttribsMgr< f32_layout, uint32_layout >::UpdateU32(unsigned int instanceId)
{
    unsigned int bufferId = instanceId / mNumInstancesPerBuffer;
    unsigned int attributesOffset = instanceId % mNumInstancesPerBuffer;
    attribBufferState& state = mStates[ bufferId ];

    if ( state.mU32updateAccum.startOffset + state.mU32updateAccum.size == attributesOffset )
    {
        ++state.mU32updateAccum.size;
        return;
    }
    else
    {
        UpdateBufferU32( bufferId );
        state.mU32updateAccum.startOffset += attributesOffset + 1;
    }

    glBindBuffer( GL_ARRAY_BUFFER, state.idU32VBO );
    glBufferSubData(
        GL_ARRAY_BUFFER,
        attributesOffset * sizeof( uint32_layout ),
        sizeof( uint32_layout ),
        (void*)&mUint32AttribsBuffers[ bufferId ][ attributesOffset ]);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

template < typename f32_layout, typename uint32_layout >
void InstanceAttribsMgr< f32_layout, uint32_layout >::UpdateBufferU32(unsigned int bufferId)
{
    attribBufferState& state = mStates[ bufferId ];

    if ( state.mU32updateAccum.size == 0 )
    {
        return;
    }

    glBindBuffer( GL_ARRAY_BUFFER, state.idU32VBO );
    glBufferSubData(
        GL_ARRAY_BUFFER,
        state.mU32updateAccum.startOffset * sizeof( uint32_layout ),
        state.mU32updateAccum.size * sizeof( uint32_layout ),
        (void*)&mUint32AttribsBuffers[ bufferId ][ state.mU32updateAccum.startOffset ]);

    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    state.mU32updateAccum.size = 0;
    state.mU32updateAccum.startOffset = 0;
}


}

#endif  // PB_INSTANCEATTRIBSMGR_H
