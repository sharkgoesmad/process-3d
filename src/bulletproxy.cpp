#include <vector>
#include <core/object.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btDefaultMotionState.h>
#include "bulletproxy.h"

namespace pb
{


BulletProxy::CollisionCallback gCollisionCb;
void* gpExtraUserData;


class MotionState : public btDefaultMotionState
{

private:

    Object* mpObj;

public:

    MotionState(Object* pObj) : mpObj( pObj ) {}

    virtual void getWorldTransform(btTransform &worldTrans) const
    {
        const glm::vec3& v = mpObj->Position();
        const glm::quat& q = mpObj->Orientation();
        btVector3 pos( v.x, v.y, v.z );
        btQuaternion quat( q.x, q.y, q.z, q.w );

        worldTrans.setOrigin( pos );
        worldTrans.setRotation( quat );
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
        btPersistentManifold* contactManifold =  pWorld->getDispatcher()->getManifoldByIndexInternal( i );
        const btCollisionObject* obA = static_cast< const btCollisionObject* >( contactManifold->getBody0() );
        const btCollisionObject* obB = static_cast< const btCollisionObject* >( contactManifold->getBody1() );

        int numContacts = contactManifold->getNumContacts();
        for ( int j = 0; j < numContacts; ++j )
        {
            btManifoldPoint& pt = contactManifold->getContactPoint( j );
            if ( pt.getDistance() < 0.0f )
            {
                BulletProxy::CollisionInfo cInfo;
                cInfo.ctx1 = obA->getUserPointer();
                cInfo.ctx2 = obB->getUserPointer();
                //const btVector3& ptA = pt.getPositionWorldOnA();
                //const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& n2 = pt.m_normalWorldOnB;
                cInfo.normal1 = Vec3( n2.x(), n2.y(), n2.z() );
                cInfo.normal2 = Vec3( n2.x(), n2.y(), n2.z() ) * -1.0f;

                gCollisionCb( cInfo, gpExtraUserData );
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
    ReleaseTrackedObjects();

    PB_DELETE( mpCollisionShapes );
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

    mpCollisionShapes = new btAlignedObjectArray<btCollisionShape*>();

    mpWorld->setInternalTickCallback( tickCallback );

    return PB_ERR_OK;
}

void BulletProxy::Step(float timeStep)
{
    mpWorld->stepSimulation( timeStep );
}

btConvexInternalShape* BulletProxy::getShape(ColliderType type, float halfExtent)
{
    btConvexInternalShape* pShape = NULL;

    switch ( type )
    {
        case Sphere:
            pShape = new btSphereShape( btScalar(halfExtent) );
            break;

        case LineZ:
        {
            btVector3 v( 0.1f, 0.1f, halfExtent );
            pShape = new btCylinderShapeZ( v );
            break;
        }

        default:
            Log::Warn( "Assigning default collision type (sphere)" );
            pShape = new btSphereShape( btScalar(halfExtent) );
            break;
    }

    return pShape;
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
        //btSphereShape* pShape = new btSphereShape( btScalar(it->halfExtent) );
        btConvexInternalShape* pShape = BulletProxy::getShape( it->type, it->halfExtent );
        mpCollisionShapes->push_back( pShape );

        btRigidBody* pRb = new btRigidBody( mass, pMotionState, pShape );
        pRb->setUserPointer( it->context );
        pRb->setCollisionFlags( pRb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
        pRb->setActivationState( DISABLE_DEACTIVATION );

        mpWorld->addRigidBody( pRb, 1, 1 );

        ++it;
    }
}

void BulletProxy::ReleaseTrackedObjects()
{
    Log::Info( "Objects left: " + STR(mpWorld->getNumCollisionObjects()));

//    for ( int idx = 0; idx < size; ++idx )
//    {
//        mpWorld->removeCollisionObject( array[idx] );
//    }

    for ( int i = mpWorld->getNumCollisionObjects() - 1; i >= 0; --i )
    {
        btCollisionObject* obj = mpWorld->getCollisionObjectArray()[ i ];
        btRigidBody* body = btRigidBody::upcast( obj );
        if ( body && body->getMotionState() )
        {
            delete body->getMotionState();
        }

        mpWorld->removeCollisionObject( obj );
        delete obj;
    }

    //delete collision shapes
    for ( int j = 0; j < mpCollisionShapes->size(); ++j )
    {
        btCollisionShape* shape = (*mpCollisionShapes)[j];
        (*mpCollisionShapes)[ j ] = NULL;
        delete shape;
    }

    mpCollisionShapes->resize( 0 );

    Log::Info( "Objects left: "+ STR(mpWorld->getNumCollisionObjects()));
}

void BulletProxy::SetCollisionCallback(void (*cb)(const CollisionInfo&, void*))
{
    gCollisionCb = cb;
}

void BulletProxy::SetCallbackExtraData(void* pData)
{
    gpExtraUserData = pData;
}



}

