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
                    Indicies& indicies,
                    Positions& positions,
                    ColorsRGBA& colors,
                    unsigned int idProgram);
    virtual ~InstancedMesh();
    
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

};

}

#endif // INSTANCEDMESH_H
