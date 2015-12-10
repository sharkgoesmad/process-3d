#ifndef PB_FORMSPHERE_H
#define PB_FORMSPHERE_H

#include <instancedobject.h>
#include "form.h"

namespace pb
{

namespace reas
{

class FormSphere : public Form
{

private:

    FormSphere(const FormSphere& rhs);
    FormSphere& operator=(const FormSphere& rhs);

public:

    FormSphere(Progress* pProgress, const Vec3& pos, const Vec3& dir, float speed, float angSpeed, float halfExtent);
    virtual ~FormSphere();
    void MakeAnother(InstancedObject* pAnother);
    float HalfSize() { return radius; };
    BulletProxy::ColliderType ColliderType() { return BulletProxy::Sphere; };

protected:

    PBError init();
    void updateTransformAttrib();

public:

    float radius;

};

}

}

#endif // PB_FORMSPHERE_H
