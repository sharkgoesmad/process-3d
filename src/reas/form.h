#ifndef PB_FORM_H
#define PB_FORM_H

#include <core/glmcfg.h>
#include <core/pbutil.h>
#include <instancedobject.h>

namespace pb
{

namespace reas
{

class BoundsVolume;

class Form : public InstancedObject
{

private:

    Form(const Form& rhs);
    Form& operator=(const Form& rhs);

public:

    Form(const Vec3& pos = Vec3(0,0,0), const Vec3& dir = Vec3(0,0,0));
    virtual ~Form();

    virtual bool CollidesWith(Form* form) = 0;

public:

    Vec3 direction;
    Vec3 position;

private:

    PBError init();

protected:

    PBError mStatus;

};


}

}

#endif // PB_FORM_H
