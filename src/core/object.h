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

    const Vec3& Position() const;

    void RotateX(float value);
    void RotateY(float value);
    void RotateZ(float value);
    void Rotate(float x, float y, float z);
    void Rotate(const Vec3& vector);
    void Rotate(const Quat& quaternion);
    void SetOrientation(const Quat& quaternion);

    const Quat& Orientation() const;

    void LookAt(const Vec3& vector);

    void Scale(float value);

    void SetVisible(bool visible);
    bool Visible();

    const Mat4& Transform();

protected:

    virtual void computeTransform();
    void markTransformDirty();
    void markTransformClean();
    bool transformDirty();

private:

    virtual PBError init();

protected:

    PBError mStatus;
    // allocation deferred to subclasses
    Mat4* mpTransform;

private:

    bool mVisible;
    bool mTransformDirty;

    Vec3 mPosition;
    Quat mRotation;
    Vec3 mScale;

};


inline const Vec3& Object::Position() const
{
    return mPosition;
}

inline const Quat& Object::Orientation() const
{
    return mRotation;
}

inline bool Object::Visible()
{
    return mVisible;
}

inline void Object::markTransformDirty()
{
    mTransformDirty = true;
}

inline void Object::markTransformClean()
{
    mTransformDirty = false;
}

inline bool Object::transformDirty()
{
    return mTransformDirty;
}

inline const Mat4& Object::Transform()
{
    return *mpTransform;
}


}

#endif // OBJECT_H
