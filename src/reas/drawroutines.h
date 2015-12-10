#ifndef PB_REAS_DRAWROUTINES_H
#define PB_REAS_DRAWROUTINES_H

#include <instancedobject.h>

namespace pb
{

namespace reas
{


class DrawRoutine : public InstancedObject
{

public:

    struct geometryInfo
    {
        Positions* pPositions;
        ColorsRGBA* pColors;
        Indices* pIndices;
        int primitiveType;
    };

private:

    DrawRoutine(const DrawRoutine& rhs);
    DrawRoutine& operator=(const DrawRoutine& rhs);

public:

    DrawRoutine();
    virtual ~DrawRoutine() {};
    void MakeAnother(InstancedObject* pAnother);
    void SetColor(unsigned int rgba);
    void Update();
    void Draw(const Mat4& vp);

protected:

    virtual PBError defineGeometry(geometryInfo& info) = 0;

private:

    PBError init();
    void updateTransformAttrib();

protected:

    ColorRGBA* mpColor;

};


class DrawLine : public DrawRoutine
{

public:

    DrawLine() {};
    float LineBetween(const Vec3& p1, const Vec3& p2);

protected:

    PBError defineGeometry(DrawRoutine::geometryInfo& info);

};


};

};

#endif  // PB_REAS_DRAWROUTINES_H
