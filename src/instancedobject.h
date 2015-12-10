#ifndef PB_INSTANCEDOBJ_H
#define PB_INSTANCEDOBJ_H

#include <memory>
#include "core/pbutil.h"
#include "core/glmcfg.h"
#include "core/object.h"


namespace pb
{

class IInstancedGeometry;
template < typename FT, typename UT > class InstancedGeometry;

class InstancedObject : public Object
{

private:

    InstancedObject& operator=(const InstancedObject& rhs);

public:

    InstancedObject();
    InstancedObject(const InstancedObject& rhs);
    virtual ~InstancedObject();
    static void Make(InstancedObject* pObj, unsigned int instances);
    virtual void MakeAnother(InstancedObject* pAnother) = 0;
    virtual void Update();
    void Draw(const Mat4& vp);

protected:

    template < typename FT, typename UT >
    InstancedGeometry<FT,UT>* geometry();

    void computeTransform();
    virtual void updateTransformAttrib() = 0;
    PBError virtual init();

protected:

    unsigned int mId;
    unsigned int mHintPerInstanceTableSize;
    std::shared_ptr<IInstancedGeometry> mpGeometry;

};


template < typename FT, typename UT >
InstancedGeometry<FT,UT>* InstancedObject::geometry()
{
    return static_cast< InstancedGeometry<FT,UT>* >( mpGeometry.get() );
}

}

#endif // PB_INSTANCEDOBJ_H
