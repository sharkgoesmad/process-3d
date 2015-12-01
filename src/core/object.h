#ifndef OBJECT_H
#define OBJECT_H

#include "pbutil.h"
#include "glmcfg.h"

namespace pb
{

class Object
{

private:

    Object(const Object& rhs);
    Object& operator=(const Object& rhs);

public:

    Object();
    virtual ~Object();

    void Translate(float x, float y, float z);
    void Translate(const Vec3& vector);
    void TranslateX(float value);
    void TranslateY(float value);
    void TranslateZ(float value);
    void SetPosition(float x, float y, float z);
    void SetPosition(const Vec3& vector);

    const Vec3& Position();

    void RotateX(float value);
    void RotateY(float value);
    void RotateZ(float value);
    void Rotate(float x, float y, float z);
    void Rotate(const Vec3& vector);
    void Rotate(const Quat& quaternion);
    void SetOrientation(const Quat& quaternion);

    void LookAt(const Vec3& vector);


    const Mat4& Transform();

protected:

    virtual void computeTransform();
    inline void markTransformDirty();
    inline void markTransformClean();
    inline bool transformDirty();

private:

    virtual PBError init();

protected:

    PBError mStatus;
    // allocation deferred to subclasses
    Mat4* mpTransform;

private:

    bool mTransformDirty;
    Mat4* mpWorldTransform;
    bool mWorldTransformDirty;


    Vec3 mPosition;
    Quat mRotation;

};

void Object::markTransformDirty()
{
    mTransformDirty = true;
}

void Object::markTransformClean()
{
    mTransformDirty = false;
}

bool Object::transformDirty()
{
    return mTransformDirty;
}

}

#endif // OBJECT_H
