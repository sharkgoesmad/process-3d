#ifndef PB_REAS_BEHAVIOR_H
#define PB_REAS_BEHAVIOR_H

namespace pb
{

namespace reas
{

class Form;

struct bhv_move_straight
{
    void operator()(Form* pForm);
};
typedef bhv_move_straight B1;


struct bhv_bounds_reflect
{
    void operator()(Form* pForm, BoundsVolume* pBounds);
};
typedef bhv_bounds_reflect B2;


struct bhv_collision_changeDir
{
    void operator()(Form* pForm1, const Vec3& normal1, Form* pForm2);
};
typedef bhv_collision_changeDir B3;


struct bhv_overlap_moveAway
{
    void operator()(Form* pForm1);
};
typedef bhv_overlap_moveAway B4;


};

};

#endif  // PB_REAS_BEHAVIOR_H
