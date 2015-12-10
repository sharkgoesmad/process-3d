#ifndef FORMLINE_H
#define FORMLINE_H

#include "form.h"

namespace pb
{

namespace reas
{

class FormLine : public Form
{

private:

    FormLine(const FormLine& rhs);
    FormLine& operator=(const FormLine& rhs);

public:

    FormLine(Progress* pProgress, const Vec3& pos, const Vec3& dir, float speed, float angSpeed, float halfExtent);
    ~FormLine();
    void Update();
    void MakeAnother(InstancedObject* pAnother);
    float HalfSize() { return length * 0.5f; };
    BulletProxy::ColliderType ColliderType() { return BulletProxy::LineZ; };

protected:

    PBError init();
    void updateTransformAttrib();

public:

    float length;

};

}

}

#endif // FORMLINE_H
