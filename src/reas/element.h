#ifndef PB_ELEMENT_H
#define PB_ELEMENT_H

#include <core/pbutil.h>
#include "boundsvolume.h"
#include "form.h"
#include "formsphere.h"
#include "formline.h"
#include "behaviors.h"

namespace pb
{

namespace reas
{

class Progress;

#define PB_REAS_ELEMENT_TEMPLATE template < \
    typename TForm, \
    typename TBhv_move, \
    typename TBhv_bounds_collision, \
    typename TBhv_collision, \
    typename TBhv_overlap >

#define PB_REAS_ELEMENT_PARAMS < TForm, TBhv_move, TBhv_bounds_collision, TBhv_collision, TBhv_overlap >

PB_REAS_ELEMENT_TEMPLATE
class Element
{

private:


public:

    Element(const Element& rhs);
    Element& operator=(const Element& rhs);
    //Element(BoundsVolume* pBounds);
    Element(
        Progress* pProgress,
        BoundsVolume* pBounds,
        const Vec3& pos,
        const Vec3& dir,
        float speed,
        float angSpeed,
        float halfExtent);
    ~Element();
    static void Make(Element& obj, unsigned int instances);
    static void MakeAnother(const Element& alpha, Element& another);

    void Draw(const Mat4& vp) { mpForm->Draw( vp ); };
    void Update(float tScale);
    void MarkCollided(reas::Form* other, const Vec3& normal);

    inline reas::Form* Form() const { return mpForm; };

    inline void move(float tScale);
    inline void onBoundsCollision();
    inline void onElementCollision(float tScale);
    inline void onElementOverlap(float tScale);

private:

    PBError init(Progress* pProgress, const Vec3& pos, const Vec3& dir, float speed, float angSpeed, float halfExtent);

private:

    PBError mStatus;
    reas::Form* mpForm;
    BoundsVolume* mpBounds;
    reas::Form* mCollisionOther;
    reas::Form* mCollisionOtherOld;
    Vec3 mCollisionNormal;

    std::array< reas::Form*, 4> mColliders;
    unsigned int mCollidersCount;


    // behaviors
    TBhv_move mMove;
    TBhv_bounds_collision mBoundsCollision;
    TBhv_collision mCollision;
    TBhv_overlap mOverlap;

};

typedef Element< FormSphere, B1, B2, B3, B4 > Element1;
typedef Element< FormSphere, B1, B9, B3, B4 > Element1_B9;
typedef Element< FormSphere, B1, B9, B3, B4 > Element1_B9;
typedef Element< FormLine, B1, B5, B3, bhv_overlap_dummy > Element3;
typedef Element< FormLine, B7, B5, B6, bhv_overlap_dummy > Element5;


PB_REAS_ELEMENT_TEMPLATE
Element PB_REAS_ELEMENT_PARAMS::Element(const Element& rhs) :
    Element(
        rhs.Form()->pprogress,
        rhs.mpBounds,
        rhs.Form()->initialPosition,
        rhs.Form()->direction,
        rhs.Form()->speed,
        rhs.Form()->angSpeed,
        rhs.Form()->HalfSize())
{
}

PB_REAS_ELEMENT_TEMPLATE
Element PB_REAS_ELEMENT_PARAMS& Element PB_REAS_ELEMENT_PARAMS::operator=(const Element& rhs)
{
    if ( this == &rhs )
    {
        return *this;
    }

    mpBounds = rhs.mpBounds;
    mStatus = init();

    return *this;
}

//PB_REAS_ELEMENT_TEMPLATE
//Element PB_REAS_ELEMENT_PARAMS::Element(BoundsVolume* pBounds) :
//    mpForm( NULL ),
//    mpBounds( pBounds ),
//    mCollidersCount( 0 )
//{
//    mStatus = init();
//}

PB_REAS_ELEMENT_TEMPLATE
Element PB_REAS_ELEMENT_PARAMS::Element(
    Progress* pProgress,
    BoundsVolume* pBounds,
    const Vec3& pos,
    const Vec3& dir,
    float speed,
    float angSpeed,
    float halfExtent) :
    mpForm( NULL ),
    mpBounds( pBounds ),
    mCollidersCount( 0 )
{
    mStatus = init( pProgress, pos, dir, speed, angSpeed, halfExtent );
}



PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::Make(Element& obj, unsigned int instances)
{
    TForm::Make( obj.mpForm, instances );
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::MakeAnother(const Element& alpha, Element& another)
{
    alpha.mpForm->MakeAnother( another.mpForm );
}

PB_REAS_ELEMENT_TEMPLATE
Element PB_REAS_ELEMENT_PARAMS::~Element()
{
    PB_DELETE( mpForm );
}

PB_REAS_ELEMENT_TEMPLATE
PBError Element PB_REAS_ELEMENT_PARAMS::init(
    Progress* pProgress,
    const Vec3& pos,
    const Vec3& dir,
    float speed,
    float angSpeed,
    float halfExtent)
{
    if ( mpBounds == NULL )
    {
        return PB_ERR;
    }

    if ( (mpForm = new TForm(pProgress, pos, dir, speed, angSpeed, halfExtent)) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    mCollisionOther = NULL;
    mCollisionOtherOld = NULL;

    return PB_ERR_OK;
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::Update(float tScale)
{
    if ( !mpBounds->IsFormInside(mpForm) )
    {
        onBoundsCollision();
    }

    if ( mCollisionOther != NULL /*&& mCollisionOther != mCollisionOtherOld*/ )
    {
        onElementCollision( tScale );
        mCollisionOtherOld = mCollisionOther;
        mCollisionOther = NULL;
    }

    if ( mCollidersCount > 0 )
    {
        onElementOverlap( tScale );
        mCollidersCount = 0;
    }

    move( tScale );

    mpForm->Update();
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::MarkCollided(reas::Form* other, const Vec3& normal)
{
    mCollisionOther = other;
    mCollisionNormal = normal;

    if ( mCollidersCount < mColliders.size() )
    {
        mColliders[ mCollidersCount ] = other;
        ++mCollidersCount;
    }
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::move(float tScale)
{
    mMove( mpForm, tScale );
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::onBoundsCollision()
{
    mBoundsCollision( mpForm, mpBounds );
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::onElementCollision(float tScale)
{
    mCollision( mpForm, mCollisionNormal, mCollisionOther, tScale );
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::onElementOverlap(float tScale)
{
    mOverlap( mpForm, mColliders.data(), mCollidersCount, tScale );
}

};

};

#endif  // PB_ELEMENT_H
