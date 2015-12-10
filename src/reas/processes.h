#ifndef PB_PROCESSES_H
#define PB_PROCESSES_H

#include "drawroutines.h"
#include "globalconfig.h"
#include "process.h"

namespace pb
{

namespace reas
{

enum Processes
{
    Proc4 = 0,
    Proc6,
    Proc10,
    Proc12,
    Proc17,
    Proc18,
    ProcCount
};

class Process4 : public Process< Element1 >
{

private:

    Process4(const Process4& rhs);
    Process4& operator=(const Process4& rhs);

public:

    Process4();
    virtual ~Process4();
    void HandleCollision(Element1* el1, Element1* el2);
    unsigned int NumElements() { return 300; };
    void CustomDraw( const Mat4& vp);

protected:

    void init();
    uint32_t color(float dist);

private:

    DrawLine mDrawLine;

};




class Process6 : public Process< Element1_B9 >
{

private:

    Process6(const Process6& rhs);
    Process6& operator=(const Process6& rhs);

public:

    Process6();
    virtual ~Process6();
    void Update(float tScale);
    void HandleCollision(Element1_B9* el1, Element1_B9* el2);
    unsigned int NumElements() { return 60; };
    void CustomDraw( const Mat4& vp);

protected:

    void init();
    uint32_t color(float dist);

private:

    DrawLine mDrawLine;

};




class Process10 : public Process< Element1_B9 >
{

private:

    Process10(const Process10& rhs);
    Process10& operator=(const Process10& rhs);

public:

    Process10();
    virtual ~Process10();
    void Update(float tScale);
    void HandleCollision(Element1_B9* el1, Element1_B9* el2);
    unsigned int NumElements() { return 220; };
    void CustomDraw( const Mat4& vp);

protected:

    void init();
    uint32_t color(float dist);

private:

    DrawLine mDrawLine;

};




class Process12 : public Process< Element3 >
{

private:

    Process12(const Process12& rhs);
    Process12& operator=(const Process12& rhs);

public:

    Process12();
    virtual ~Process12();
    void HandleCollision(Element3* el1, Element3* el2);
    unsigned int NumElements() { return 400; };
    void CustomDraw( const Mat4& vp);

protected:

    void init();
    uint32_t color(float dist);

private:

    DrawLine mDrawLine;

};




class Process17 : public Process< Element5 >
{

private:

    Process17(const Process17& rhs);
    Process17& operator=(const Process17& rhs);

public:

    Process17();
    virtual ~Process17();
    void HandleCollision(Element5* el1, Element5* el2);
    unsigned int NumElements() { return 200; };
    void CustomDraw( const Mat4& vp);

protected:

    void init();
    uint32_t color(float dist);

private:

    DrawLine mDrawLine;

};




class Process18 : public Process< Element5 >
{

private:

    Process18(const Process18& rhs);
    Process18& operator=(const Process18& rhs);

public:

    Process18();
    virtual ~Process18();
    void HandleCollision(Element5* el1, Element5* el2);
    unsigned int NumElements() { return 200; };
    void CustomDraw( const Mat4& vp);

protected:

    void init();
    uint32_t color(float dist);

private:

    DrawLine mDrawLine;

};



}

}

#endif // PB_PROCESSES_H
