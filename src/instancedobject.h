#ifndef PB_INSTANCEDOBJ_H
#define PB_INSTANCEDOBJ_H

#include <memory>
#include "core/pbutil.h"
#include "core/glmcfg.h"
#include "core/object.h"


namespace pb
{

class InstancedMesh;

class InstancedObject : public Object
{

private:

    InstancedObject(const InstancedObject& rhs);
    InstancedObject& operator=(const InstancedObject& rhs);

public:

    InstancedObject();
    virtual ~InstancedObject();
    static void Make(InstancedObject& obj, unsigned int instances);
    static void MakeAnother(const InstancedObject& alpha, InstancedObject& another);
    void Update();
    void Draw(const Mat4& vp);

protected:

    void computeTransform();
    PBError virtual init();

protected:

    unsigned int mHintPerInstanceTableSize;
    std::shared_ptr<InstancedMesh> mpMesh;

private:

    unsigned int mId;

};

}

#endif // PB_INSTANCEDOBJ_H
