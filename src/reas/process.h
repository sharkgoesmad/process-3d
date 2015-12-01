#ifndef PB_REAS_PROCESS_H
#define PB_REAS_PROCESS_H

#include "element.h"

namespace pb
{

class ReasObject;

namespace reas
{

class BoundsVolume;

class Process
{

private:

    Process(const Process& rhs);
    Process& operator=(const Process& rhs);

public:

    Process();
    virtual ~Process();
    static ReasObject* ReasAlpha();
    void Draw(const Mat4& vp);
    void Update();

private:

    BoundsVolume* mpBounds;
    Element1* mpElement;
    std::vector<Element1> mEls;

};

}

}

#endif // PB_REAS_PROCESS_H
