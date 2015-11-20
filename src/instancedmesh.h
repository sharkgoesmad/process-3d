#ifndef INSTANCEDMESH_H
#define INSTANCEDMESH_H

#include "core/glmcfg.h"
#include "core/pbutil.h"

namespace pb
{

class InstancedMesh
{
    
private:

    InstancedMesh(const InstancedMesh& rhs);
    InstancedMesh& operator=(const InstancedMesh& rhs);

public:

    InstancedMesh(unsigned int primitiveType,
                    Indicies* pIndicies,
                    Positions* pPositions,
                    ColorsRGBA* pColors,
                    unsigned int idProgram);
    virtual ~InstancedMesh();
    
    void Render(const glm::mat4& transform);
    
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
    Indicies* mpBkIndicies;
    Positions* mpBkPositions;
    ColorsRGBA* mpBkColors;

};

}

#endif // INSTANCEDMESH_H
