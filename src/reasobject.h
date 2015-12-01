#ifndef REASOBJECT_H
#define REASOBJECT_H

#include <memory>
#include "core/pbutil.h"
#include "core/glmcfg.h"
#include "core/object.h"


namespace pb
{

class InstancedMesh;

class ReasObject : public Object
{

private:

    ReasObject(const ReasObject& rhs);
    ReasObject& operator=(const ReasObject& rhs);

public:

    ReasObject();
    virtual ~ReasObject();
    static void Make(ReasObject& obj);
    static void MakeAnother(const ReasObject& alpha, ReasObject& another);
    void Update();
    void Draw(const Mat4& vp);

protected:

    void computeTransform();

protected:

    unsigned int mHintPerInstanceTableSize;

private:

    PBError init();

private:

    unsigned int mId;
    std::shared_ptr<InstancedMesh> mpMesh;

};

}

#endif // REASOBJECT_H
