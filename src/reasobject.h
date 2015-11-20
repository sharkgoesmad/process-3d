#ifndef REASOBJECT_H
#define REASOBJECT_H

#include "core/glmcfg.h"


namespace pb
{
    
class InstancedMesh;

class ReasObject
{
    
private:

    ReasObject(const ReasObject& rhs);
    ReasObject& operator=(const ReasObject& rhs);

public:

    ReasObject();
    virtual ~ReasObject();
    void Draw();
    
private:
    
    PBError init();
    
private:
    
    PBError mStatus;
    InstancedMesh* mpMesh;
    glm::mat4* mpTransform;

};

}

#endif // REASOBJECT_H
