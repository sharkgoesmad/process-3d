#ifndef CAMERA_H
#define CAMERA_H

#include "core/object.h" // Base class: Object

namespace pb
{

class Camera : public Object
{

private:

    Camera(const Camera& rhs);
    Camera& operator=(const Camera& rhs);

public:

    Camera();
    virtual ~Camera();

    inline const Mat4& VP();

private:

    PBError init();
    void computeVP();

private:

    Mat4* mpProjection;
    Mat4* mpVP;

};

const Mat4& Camera::VP()
{
    computeVP();
    return *mpVP;
}

}

#endif // CAMERA_H
