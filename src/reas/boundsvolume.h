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

    BoundsVolume(){};
    virtual ~BoundsVolume(){};

    virtual bool IsFormInside(Form* pForm) = 0;
    virtual void IntersectionNormal(Form* pForm, Vec3& norm) = 0;

};


class SphereBoundsVolume : public BoundsVolume
{

public:

    SphereBoundsVolume(float radius, const Vec3 center = Vec3(0.0f,0.0f,0.0f)) :
        mCenter( center ),
        mRadius( radius )
    {}

    bool IsFormInside(Form* pForm);
    void IntersectionNormal(Form* pForm, Vec3& norm);

private:

    Vec3 mCenter;
    float mRadius;

};

inline bool SphereBoundsVolume::IsFormInside(Form* pForm)
{
    // TODO better test
    Vec3 dist( mCenter - pForm->Position() );

    return glm::length( dist ) <= mRadius;
}

inline void SphereBoundsVolume::IntersectionNormal(Form* pForm, Vec3& norm)
{
    Vec3 position;
    glm::intersectRaySphere( pForm->position, pForm->direction, mCenter, mRadius, position, norm );
}

};

};

#endif // PB_REAS_BOUNDSVOLUME_H
