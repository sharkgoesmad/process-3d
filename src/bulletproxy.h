#ifndef BULLETPROXY_H
#define BULLETPROXY_H

#include "core/pbutil.h"
#include "core/glmcfg.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace pb
{

class Object;

class BulletProxy
{
    static BulletProxy* mInstance;

public:

    struct ObjectData
    {
        Object* obj;
        void* context;
    };

    struct CollisionInfo
    {
        void* ctx1;
        void* ctx2;
        Vec3  normal1;
        Vec3  normal2;
    };

    typedef void (*CollisionCallback)(const CollisionInfo&);

private:

    BulletProxy(const BulletProxy& rhs);
    BulletProxy& operator=(const BulletProxy& rhs);

public:

    static BulletProxy* Get();
    static void Release();

    void Step(float timeStep );
    void TrackObjects(std::vector<ObjectData>& objects);
    void SetCollisionCallback(void (*callback)(const CollisionInfo&));

private:

    BulletProxy();
    ~BulletProxy();
    PBError init();

private:

    PBError mStatus;

    // mind the order
    btDefaultCollisionConfiguration* mpCollisionConfig;
    btCollisionDispatcher* mpCollisionDispatcher;
    btBroadphaseInterface* mpBVTiface;
    btSequentialImpulseConstraintSolver* mpSolver;
    btDiscreteDynamicsWorld* mpWorld;

};


inline BulletProxy* BulletProxy::Get()
{
    if ( mInstance == NULL )
    {
        mInstance = new BulletProxy();
    }

    return mInstance;
}

inline void BulletProxy::Release()
{
    PB_DELETE( mInstance );
}

}

#endif // BULLETPROXY_H
