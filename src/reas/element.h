#ifndef PB_ELEMENT_H
#define PB_ELEMENT_H

#include <core/pbutil.h>
#include "boundsvolume.h"
#include "formsphere.h"
#include "behaviors.h"

namespace pb
{

namespace reas
{

#define PB_REAS_ELEMENT_TEMPLATE template < \
    typename TForm, \
    typename TBhv_move, \
    typename TBhv_bounds_collision, \
    typename TBhv_collision, \
    typename TBhv_overlap >

#define PB_REAS_ELEMENT_PARAMS <TForm, TBhv_move, TBhv_bounds_collision, TBhv_collision, TBhv_overlap>

PB_REAS_ELEMENT_TEMPLATE
class Element
{

private:


public:

    Element(const Element& rhs);
    Element& operator=(const Element& rhs);
    Element(BoundsVolume* pBounds);
    ~Element();
    static void Make(Element& obj, unsigned int instances);
    static void MakeAnother(const Element& alpha, Element& another);

    void Draw(const Mat4& vp) { mpForm->Draw( vp ); };
    void Update();
    void MarkCollided(TForm* other, const Vec3& normal) { mCollisionOther = other; mCollisionNormal = normal; };

    inline TForm* Form() { return mpForm; };

    inline void move();
    inline void onBoundsCollision();
    inline void onElementCollision();
    inline void onElementOverlap();

private:

    PBError init();

private:

    PBError mStatus;
    TForm* mpForm;
    BoundsVolume* mpBounds;
    TForm* mCollisionOther;
    Vec3 mCollisionNormal;

    // behaviors
    TBhv_move mMove;
    TBhv_bounds_collision mBoundsCollision;
    TBhv_collision mCollision;
    TBhv_overlap mOverlap;

};

typedef Element<FormSphere, B1, B2, B3, B4> Element1;

PB_REAS_ELEMENT_TEMPLATE
Element PB_REAS_ELEMENT_PARAMS::Element(const Element& rhs) :
    Element( rhs.mpBounds )
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

PB_REAS_ELEMENT_TEMPLATE
Element PB_REAS_ELEMENT_PARAMS::Element(BoundsVolume* pBounds) :
    mpForm( NULL ),
    mpBounds( pBounds )
{
    mStatus = init();
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::Make(Element& obj, unsigned int instances)
{
    TForm::Make( *obj.mpForm, instances );
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::MakeAnother(const Element& alpha, Element& another)
{
    TForm::MakeAnother( *alpha.mpForm, *another.mpForm );
}

PB_REAS_ELEMENT_TEMPLATE
Element PB_REAS_ELEMENT_PARAMS::~Element()
{
    PB_DELETE( mpForm );
}

PB_REAS_ELEMENT_TEMPLATE
PBError Element PB_REAS_ELEMENT_PARAMS::init()
{
    if ( mpBounds == NULL )
    {
        return PB_ERR;
    }

    if ( (mpForm = new TForm()) == NULL )
    {
        PB_RETURN_ALLOCFAIL();
    }

    mCollisionOther = NULL;

    return PB_ERR_OK;
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::Update()
{
    if ( !mpBounds->IsFormInside(mpForm) )
    {
        onBoundsCollision();
    }

    if ( mCollisionOther != NULL )
    {
        onElementCollision();
        mCollisionOther = NULL;
    }

    move();

    mpForm->Update();
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::move()
{
    mMove( mpForm );
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::onBoundsCollision()
{
    mBoundsCollision( mpForm, mpBounds );
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::onElementCollision()
{
    mCollision( mpForm, mCollisionNormal, mCollisionOther );
}

PB_REAS_ELEMENT_TEMPLATE
void Element PB_REAS_ELEMENT_PARAMS::onElementOverlap()
{
    mOverlap( mpForm );
}

};

};

#endif  // PB_ELEMENT_H
