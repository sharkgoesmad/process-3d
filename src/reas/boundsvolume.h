#ifndef PB_REAS_BOUNDSVOLUME_H
#define PB_REAS_BOUNDSVOLUME_H

#include <core/glmcfg.h>
#include <glm/gtx/intersect.hpp>
#include "form.h"

namespace pb
{

namespace reas
{

class BoundsVolume
{

private:

    BoundsVolume(const BoundsVolume& rhs);
    BoundsVolume& operator=(const BoundsVolume& rhs);

public:

    BoundsVolume(const Vec3 center = Vec3(0.0f,0.0f,0.0f)) : mCenter( center ) {};
    virtual ~BoundsVolume(){};

    const Vec3& Origin() { return mCenter; };

    virtual bool IsFormInside(Form* pForm) = 0;
    virtual void IntersectionNormal(Form* pForm, Vec3& norm) = 0;
    virtual void OppositeSide(Form* pForm, Vec3& position) = 0;
    virtual void OppositeSideNormal(Form* pForm, Vec3& position) = 0;
    virtual float HalfExtent() = 0;

protected:

    Vec3 mCenter;

};


class SphereBoundsVolume : public BoundsVolume
{

public:

    SphereBoundsVolume(float radius, const Vec3 center = Vec3(0.0f,0.0f,0.0f)) :
        BoundsVolume( center ),
        mRadius( radius )
    {}

    bool IsFormInside(Form* pForm);
    void IntersectionNormal(Form* pForm, Vec3& norm);
    void OppositeSide(Form* pForm, Vec3& position);
    void OppositeSideNormal(Form* pForm, Vec3& position);
    float HalfExtent() { return mRadius; };

private:

    float mRadius;

};

inline bool SphereBoundsVolume::IsFormInside(Form* pForm)
{
    // TODO better test
    Vec3 dist( mCenter - pForm->Position() );

    return glm::length( dist ) <= ( mRadius - pForm->HalfSize() );
}

inline void SphereBoundsVolume::IntersectionNormal(Form* pForm, Vec3& norm)
{
    Vec3 position;
    glm::intersectRaySphere( pForm->Position(), pForm->direction, mCenter, mRadius, position, norm );
}

inline void SphereBoundsVolume::OppositeSide(Form* pForm, Vec3& position)
{
    Vec3 norm;
    glm::intersectRaySphere( pForm->Position(), pForm->direction * -1.0f, mCenter, mRadius, position, norm );
}

inline void SphereBoundsVolume::OppositeSideNormal(Form* pForm, Vec3& position)
{
    Vec3 norm;
    Vec3 dir( glm::normalize(pForm->Position() - mCenter) );
    glm::intersectRaySphere( pForm->Position(), dir * -1.0f, mCenter, mRadius, position, norm );
}

};

};

#endif // PB_REAS_BOUNDSVOLUME_H
