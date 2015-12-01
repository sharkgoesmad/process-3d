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

    FormSphere(const Vec3& pos = Vec3(0,0,0), const Vec3& dir = Vec3(0,0,0));
    virtual ~FormSphere();

    virtual bool CollidesWith(Form* form);

protected:

    PBError init();

private:

    float mRadius;

};

}

}

#endif // PB_FORMSPHERE_H
