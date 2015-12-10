#include <glm/gtc/random.hpp>
#include <core/mathutil.h>
#include <reasobject.h>
#include "process.h"
#include "boundsvolume.h"
#include "form.h"
#include "behaviors.h"

namespace pb
{

using namespace reas;


static const Quat gNoRotation;


const Quat gSlightRotation = glm::rotation( Vec3(0,0,1), Vec3(0.05f, 0.05f, 0.998f) ); //Quat( 0.1f, glm::sphericalRand(1.0f) );

void bhv_move_straight::operator()(Form* pForm, float tScale)
{
    pForm->Translate( pForm->direction * pForm->speed * tScale );
}

void bhv_move_deviate::operator()(Form* pForm, float tScale)
{
    pForm->direction = glm::normalize( glm::mix(pForm->direction + glm::sphericalRand(0.1f), pForm->direction, 0.8f) );
    pForm->Translate( pForm->direction * pForm->speed * tScale );
}

void bhv_bounds_bounceoff::operator()(Form* pForm, BoundsVolume* pBounds)
{
    pForm->direction *= -1.0f;
    pForm->Translate( pForm->direction * pForm->HalfSize() );
}

void bhv_bounds_returnToOrigin::operator()(Form* pForm, BoundsVolume* pBounds)
{
    pForm->SetPosition( pBounds->Origin() );
};

void bhv_bounds_returnToProgress::operator()(Form* pForm, BoundsVolume* pBounds)
{
    // slerp
    float x = pBounds->HalfExtent() - pForm->pprogress->tProgress;
    float r = pBounds->HalfExtent();
    float val = glm::sqrt( r*r - x*x );
    pForm->SetPosition( pBounds->Origin() + pForm->direction * val );
};

void bhv_bounds_enterOpposite::operator()(Form* pForm, BoundsVolume* pBounds)
{
    Vec3 pos;
    pBounds->OppositeSide( pForm, pos );
    pForm->SetPosition( pos + (pForm->direction * (pForm->HalfSize() + 1.0f)) );
}


void bhv_bounds_enterOppositeNormal::operator()(Form* pForm, BoundsVolume* pBounds)
{
    Vec3 pos;
    pBounds->OppositeSideNormal( pForm, pos );
    pForm->SetPosition( pos + (pForm->direction * (pForm->HalfSize() + 1.0f)) );
}


void bhv_collision_changeDir::operator()(Form* pForm1, const Vec3& normal, Form* pForm2, float tScale)
{
    float dot = glm::dot( normal, pForm1->direction );
    if ( dot == -1.0f )
    {
        pForm1->direction = glm::rotate( gSlightRotation, pForm1->direction );
    }

    //
    Vec3 tangent( glm::cross(pForm1->direction, normal) );
    Quat rotation( glm::angleAxis(pForm1->angSpeed * tScale, tangent) );
    pForm1->direction = glm::normalize( glm::rotate(rotation, pForm1->direction) );
}

void bhv_collision_follow::operator()(Form* pForm1, const Vec3& normal1, Form* pForm2, float tScale)
{
    Quat rotation( glm::rotation(pForm1->direction, pForm2->direction) );
    rotation = glm::lerp( gNoRotation, rotation, pForm1->angSpeed * tScale );
    pForm1->direction = glm::normalize( glm::rotate(rotation, pForm1->direction) );
}

void bhv_overlap_moveAway::operator()(Form* pForm, Form** arraypColliders, unsigned int count, float tScale)
{
    Form** it = &arraypColliders[ 0 ];
    Form** itEnd = ( &arraypColliders[ count-1 ] ) + 1;

    const Vec3& formPos( pForm->Position() );
    Vec3 netDir( 0.0f );
    while ( it != itEnd )
    {
        netDir += formPos - (*it)->Position();
        ++it;
    }

    netDir /= count;
    netDir = glm::normalize( netDir );

    pForm->Translate( netDir * pForm->speed * tScale );
}

};
