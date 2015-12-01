#include <vector>
#include <core/object.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btDefaultMotionState.h>
#include "bulletproxy.h"

namespace pb
{


BulletProxy::CollisionCallback gCollisionCb;


class MotionState : public btDefaultMotionState
{

private:

    Object* mpObj;

public:

    MotionState(Object* pObj) : mpObj( pObj ) {}

    virtual void getWorldTransform(btTransform &worldTrans) const
    {
        const glm::vec3& v = mpObj->Position();
        btVector3 pos( v.x, v.y, v.z );

        worldTrans.setOrigin( pos );
        // TODO
        //worldTrans = mInitialPosition;
    }

    virtual void setWorldTransform(const btTransform &worldTrans)
    {
        // TODO probably nothing
    }
};


void tickCallback(btDynamicsWorld* pWorld, btScalar timeStep)
{
    int numManifolds = pWorld->getDispatcher()->getNumManifolds();
    for ( int i = 0; i < numManifolds; ++i )
    {
        btPersistentManifold* contactManifold =  pWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>( contactManifold->getBody0() );
        const btCollisionObject* obB = static_cast<const btCollisionObject*>( contactManifold->getBody1() );

        int numContacts = contactManifold->getNumContacts();
        for (int j=0;j<numContacts;j++)
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance()<0.f)
            {
                BulletProxy::CollisionInfo cInfo;
                cInfo.ctx1 = obA->getUserPointer();
                cInfo.ctx2 = obB->getUserPointer();
                //const btVector3& ptA = pt.getPositionWorldOnA();
                //const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& n2 = pt.m_normalWorldOnB;
                cInfo.normal1 = Vec3( n2.x(), n2.y(), n2.z() );
                cInfo.normal2 = Vec3( n2.x(), n2.y(), n2.z() ) * -0.1f;

                gCollisionCb( cInfo );
            }
        }
    }
}


BulletProxy* BulletProxy::mInstance = NULL;

BulletProxy::BulletProxy() :
    mpCollisionConfig( NULL ),
    mpCollisionDispatcher( NULL ),
    mpBVTiface( NULL ),
    mpSolver( NULL ),
    mpWorld( NULL )
{
    mStatus = init();
}

BulletProxy::~BulletProxy()
{
    PB_DELETE( mpWorld );
    PB_DELETE( mpSolver );
    PB_DELETE( mpBVTiface );
    PB_DELETE( mpCollisionDispatcher );
    PB_DELETE( mpCollisionConfig );
}

PBError BulletProxy::init()
{
    if ( (mpCollisionConfig = new btDefaultCollisionConfiguration()) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    if ( (mpCollisionDispatcher = new btCollisionDispatcher(mpCollisionConfig)) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    if ( (mpBVTiface = new btDbvtBroadphase()) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    if ( (mpSolver = new btSequentialImpulseConstraintSolver()) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    mpWorld = new btDiscreteDynamicsWorld( mpCollisionDispatcher, mpBVTiface, mpSolver, mpCollisionConfig );
    if ( mpWorld == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    mpWorld->setInternalTickCallback( tickCallback );

    return PB_ERR_OK;
}

void BulletProxy::Step(float timeStep)
{
    mpWorld->stepSimulation( timeStep );
}

void BulletProxy::TrackObjects(std::vector<ObjectData>& objects)
{
    std::vector<ObjectData>::iterator it = objects.begin();
    std::vector<ObjectData>::iterator itEnd = objects.end();

    while ( it != itEnd )
    {
        // TODO init transform
        MotionState* pMotionState = new MotionState( it->obj );
        btScalar mass( 0.0f );
        btSphereShape* pShape = new btSphereShape( btScalar(1.0f) );

        btRigidBody* pRb = new btRigidBody( mass, pMotionState, pShape );
        pRb->setUserPointer( it->context );
        pRb->setCollisionFlags( pRb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
        pRb->setActivationState( DISABLE_DEACTIVATION );

        mpWorld->addRigidBody( pRb, 1, 1 );

        ++it;
    }
}

void BulletProxy::SetCollisionCallback(void (*cb)(const CollisionInfo&))
{
    gCollisionCb = cb;
}



}

