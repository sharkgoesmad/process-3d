#ifndef REASOBJECT_H
#define REASOBJECT_H

#include <memory>
#include "core/pbutil.h"
#include "core/glmcfg.h"
#include "instancedobject.h"


namespace pb
{

class ReasObject : public InstancedObject
{

private:

    struct f32_attribs
    {
        Mat4 transform;
    };

    struct u32_attribs
    {
        ColorRGBA color;
    };

private:

    ReasObject(const ReasObject& rhs);
    ReasObject& operator=(const ReasObject& rhs);

public:

    ReasObject();
    virtual ~ReasObject();
    //static void Make(ReasObject& obj);
    void MakeAnother(InstancedObject* pAnother);
    void SetColor(unsigned int rgba);
    void Update();
    void Draw(const Mat4& vp);

private:

    PBError init();
    void updateTransformAttrib();

private:

    ColorRGBA* mpColor;

};

}

#endif // REASOBJECT_H
