#ifndef PB_REAS_BEHAVIOR_H
#define PB_REAS_BEHAVIOR_H

namespace pb
{

namespace reas
{

class Form;

struct bhv_move_straight
{
    void operator()(Form* pForm, float tScale);
};
typedef bhv_move_straight B1;


struct bhv_move_deviate
{
    void operator()(Form* pForm, float tScale);
};
typedef bhv_move_deviate B7;


struct bhv_bounds_bounceoff
{
    void operator()(Form* pForm, BoundsVolume* pBounds);
};
typedef bhv_bounds_bounceoff B2;


struct bhv_bounds_returnToOrigin
{
    void operator()(Form* pForm, BoundsVolume* pBounds);
};
typedef bhv_bounds_returnToOrigin B8;


struct bhv_bounds_returnToProgress
{
    void operator()(Form* pForm, BoundsVolume* pBounds);
};
typedef bhv_bounds_returnToProgress B9;


struct bhv_bounds_enterOpposite
{
    void operator()(Form* pForm, BoundsVolume* pBounds);
};
typedef bhv_bounds_enterOpposite B5;


struct bhv_bounds_enterOppositeNormal
{
    void operator()(Form* pForm, BoundsVolume* pBounds);
};
typedef bhv_bounds_enterOppositeNormal B10;


struct bhv_collision_changeDir
{
    void operator()(Form* pForm1, const Vec3& normal1, Form* pForm2, float tScale);
};
typedef bhv_collision_changeDir B3;


struct bhv_collision_follow
{
    void operator()(Form* pForm1, const Vec3& normal1, Form* pForm2, float tScale);
};
typedef bhv_collision_follow B6;



struct bhv_overlap_dummy
{
    void operator()(Form* pForm, Form** arraypColliders, unsigned int count, float tScale) {};
};


struct bhv_overlap_moveAway
{
    void operator()(Form* pForm, Form** arraypColliders, unsigned int count, float tScale);
};
typedef bhv_overlap_moveAway B4;


};

};

#endif  // PB_REAS_BEHAVIOR_H
