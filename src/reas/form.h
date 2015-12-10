#ifndef PB_FORM_H
#define PB_FORM_H

#include <core/glmcfg.h>
#include <core/pbutil.h>
#include <bulletproxy.h>
#include <instancedobject.h>

namespace pb
{

namespace reas
{

class Progress;

class Form : public InstancedObject
{

private:

    Form(const Form& rhs);
    Form& operator=(const Form& rhs);

public:

    Form(Progress* pProgress, const Vec3& pos, const Vec3& dir, float speed, float angSpeed);
    virtual ~Form();
    virtual float HalfSize() = 0;
    virtual BulletProxy::ColliderType ColliderType() = 0;

public:

    Vec3 direction;
    Vec3 initialPosition;
    float speed;
    float angSpeed;
    Progress* pprogress;

private:

    PBError init();

protected:

    PBError mStatus;

};


}

}

#endif // PB_FORM_H
