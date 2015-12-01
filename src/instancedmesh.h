#ifndef INSTANCEDMESH_H
#define INSTANCEDMESH_H

#include <list>
#include "core/glmcfg.h"
#include "core/pbutil.h"

namespace pb
{

class InstancedMesh
{
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

private:

    InstancedMesh(const InstancedMesh& rhs);
    InstancedMesh& operator=(const InstancedMesh& rhs);

public:

    InstancedMesh(unsigned int primitiveType,
                    Indices* pIndicies,
                    Positions* pPositions,
                    ColorsRGBA* pColors,
                    unsigned int idProgram,
                    unsigned int hintPerInstanceTableSize);
    virtual ~InstancedMesh();

    unsigned int AddInstance();
    Mat4* Transform(unsigned int id);
    void UpdateTransform(unsigned int transformId);
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
    transformAttribMgr mTransformsMgr;

};

}

#endif // INSTANCEDMESH_H
