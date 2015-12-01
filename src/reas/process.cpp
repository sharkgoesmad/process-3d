#include <vector>
#include <bulletproxy.h>
#include <reasobject.h>
#include "boundsvolume.h"
#include "process.h"

namespace pb
{

using namespace reas;

static ReasObject gAlpha;

void collisionCallback(const BulletProxy::CollisionInfo& cInfo)
{
    Element1* e1 = static_cast<Element1*>( cInfo.ctx1 );
    Element1* e2 = static_cast<Element1*>( cInfo.ctx2 );
    e1->MarkCollided( e2->Form(), cInfo.normal1 );
    e2->MarkCollided( e1->Form(), cInfo.normal2 );
}

Process::Process()
{
    ReasObject::Make( gAlpha );

    std::vector<BulletProxy::ObjectData> bulletObjs;
    mpBounds = new SphereBoundsVolume( 30.0f );

    mEls.insert( mEls.begin(), 500, mpBounds );
    Element1::Make( mEls[0], 500 );
    BulletProxy::ObjectData data;
    data.obj = mEls[0].Form();
    data.context = &mEls[0];
    bulletObjs.push_back( data );

    std::vector<Element1>::iterator it = mEls.begin();
    std::vector<Element1>::iterator itEnd = mEls.end();
    ++it;
    while ( it != itEnd )
    {
        Element1::MakeAnother( mEls[0], *it );
        data.obj = it->Form();
        data.context = &(*it);
        bulletObjs.push_back( data );
        ++it;
    }

    // add to bullet for collision detection
    BulletProxy::Get()->TrackObjects( bulletObjs );
    BulletProxy::Get()->SetCollisionCallback( collisionCallback );
}

Process::~Process()
{
    PB_DELETE( mpBounds );
}

ReasObject* Process::ReasAlpha()
{
    return &gAlpha;
}

void Process::Update()
{
    std::vector<Element1>::iterator it = mEls.begin();
    std::vector<Element1>::iterator itEnd = mEls.end();

    while ( it != itEnd )
    {
        it->Update();
        ++it;
    }
}

void Process::Draw(const Mat4& vp)
{
    mEls[0].Draw( vp );
}


}

