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

void bhv_move_straight::operator()(Form* pForm)
{
    pForm->Translate( pForm->direction * 0.1f );
}

void bhv_bounds_reflect::operator()(Form* pForm, BoundsVolume* pBounds)
{
    Vec3 intersectionNormal;
    pForm->SetPosition( pForm->Position() - pForm->direction * 0.5f );
    pBounds->IntersectionNormal( pForm, intersectionNormal );

    Vec3 newDirection( glm::reflect(pForm->direction, intersectionNormal) );
    pForm->direction = glm::normalize( newDirection );
}

void bhv_collision_changeDir::operator()(Form* pForm1, const Vec3& normal1, Form* pForm2)
{
    // TODO better handle this
    // pick a random direction
    //pForm1->direction = glm::normalize( glm::sphericalRand( 1.0f ) );
    Vec3 newDirection( glm::reflect(pForm1->direction, normal1) );
    pForm1->direction = glm::normalize( newDirection );

    Vec3 pos( glm::mix(pForm1->Position(), pForm2->Position(), 0.5f) );
    Vec3 diff( pForm1->Position() - pForm2->Position() );
    Quat rot = glm::rotation( Vec3(0.0f, 1.0f, 0.0f), glm::normalize(diff) );
    ReasObject obj;
    ReasObject::MakeAnother( *Process::ReasAlpha(), obj );
    obj.SetOrientation( rot );
    obj.SetPosition( pos );
    obj.Update();
}

void bhv_overlap_moveAway::operator()(Form* pForm)
{
    // move in opposite direction
    pForm->direction *= -1;
}

};
