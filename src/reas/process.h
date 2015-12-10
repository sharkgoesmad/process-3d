#ifndef PB_REAS_PROCESS_H
#define PB_REAS_PROCESS_H

#include <vector>
#include <bulletproxy.h>
#include "progress.h"
#include "element.h"

namespace pb
{

class ReasObject;

namespace reas
{

class BoundsVolume;

class IProcess
{

public:

    virtual ~IProcess() {};
    virtual void Draw(const Mat4& vp) = 0;
    virtual void Update(float tScale) = 0;
    virtual void ToggleDebugMode() = 0;

};

template < typename El >
class Process : public IProcess
{

private:

    Process(const Process& rhs);
    Process& operator=(const Process& rhs);

public:

    Process();
    virtual ~Process();
    void Draw(const Mat4& vp);
    virtual void Update(float tScale);
    void ToggleDebugMode();

    virtual void HandleCollision(El* el1, El* el2) = 0;
    virtual unsigned int NumElements() = 0;
    virtual void CustomDraw( const Mat4& vp) = 0;

protected:

    virtual void init();

private:

    static void collisionCallback(const BulletProxy::CollisionInfo& cInfo, void* pData);

protected:

    BoundsVolume* mpBounds;
    Progress* mpProgress;
    std::vector< El > mEls;

};


template < typename El >
Process< El >::Process() :
    mpBounds( NULL ),
    mpProgress( NULL )
{
}

template < typename El >
Process< El >::~Process()
{
    BulletProxy::Get()->ReleaseTrackedObjects();
    PB_DELETE( mpBounds );
    PB_DELETE( mpProgress );
}

template < typename El >
void Process< El >::Update(float tScale)
{
    typename std::vector< El >::iterator it = mEls.begin();
    typename std::vector< El >::iterator itEnd = mEls.end();

    while ( it != itEnd )
    {
        it->Update( tScale );
        ++it;
    }
}

template < typename El >
void Process< El >::ToggleDebugMode()
{
    mEls[0].Form()->SetVisible( !mEls[0].Form()->Visible() );
}

template < typename El >
void Process< El >::init()
{
    std::vector<BulletProxy::ObjectData> bulletObjs;

    mEls[0].Form()->SetVisible( false );
    El::Make( mEls[0], NumElements() );
    BulletProxy::ObjectData data;
    data.obj = mEls[0].Form();
    data.type = mEls[0].Form()->ColliderType();
    data.context = &mEls[0];
    data.halfExtent = mEls[0].Form()->HalfSize();
    bulletObjs.push_back( data );

    typename std::vector< El >::iterator it = mEls.begin();
    typename std::vector< El >::iterator itEnd = mEls.end();
    ++it;
    while ( it != itEnd )
    {
        El::MakeAnother( mEls[0], *it );
        data.obj = it->Form();
        data.type = it->Form()->ColliderType();
        data.context = &(*it);
        data.halfExtent = it->Form()->HalfSize();
        bulletObjs.push_back( data );
        ++it;
    }

    // add to bullet for collision detection
    BulletProxy::Get()->TrackObjects( bulletObjs );
    BulletProxy::Get()->SetCollisionCallback( collisionCallback );
    BulletProxy::Get()->SetCallbackExtraData( this );
}

template < typename El >
void Process< El >::Draw(const Mat4& vp)
{
    mEls[0].Draw( vp );
    CustomDraw( vp );
}



template < typename El >
void Process< El >::collisionCallback(const BulletProxy::CollisionInfo& cInfo, void* pData)
{
    El* e1 = static_cast<El*>( cInfo.ctx1 );
    El* e2 = static_cast<El*>( cInfo.ctx2 );
    e1->MarkCollided( e2->Form(), cInfo.normal1 );
    e2->MarkCollided( e1->Form(), cInfo.normal2 );

    Process< El >* pProcInstance = static_cast< Process<El>* >( pData );

    pProcInstance->HandleCollision( e1, e2 );
}


}

}

#endif // PB_REAS_PROCESS_H
