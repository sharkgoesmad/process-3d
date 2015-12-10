#include <glm/gtc/random.hpp>
#include <glm/gtx/color_space.hpp>
#include "processes.h"

namespace pb
{

using namespace reas;


static const float gDefaultElementSpeed = 0.02f;
static const float gDefaultElementAngularSpeed = gDefaultElementSpeed * glm::pi<float>();


Process4::Process4()
{
    init();
}

Process4::~Process4() {}

void Process4::init()
{
    DrawRoutine::Make( &mDrawLine, 500000 );

    mpBounds = new SphereBoundsVolume( 15.0f );
    mpProgress = new Progress();

    //mEls.emplace_back( mpBounds, Vec3(0.3f, -2.0f, 0.0f), Vec3(0, 1, 0) );
    //mEls.emplace_back( mpBounds, Vec3(0.0f, 2.0f, 0.0f), Vec3(0, -1, 0) );

//    mEls.insert(
//        mEls.begin(),
//        NumElements(),
//        mpBounds,
//        glm::ballRand( 8.0f ),
//        glm::sphericalRand( 1.0f ),
//        glm::linearRand( 0.1f, 2.0f ));

    for (int idx = 0; idx < NumElements(); ++idx)
    {
        mEls.push_back(
            Element1(
                mpProgress,
                mpBounds,
                glm::ballRand( mpBounds->HalfExtent() * 0.7 ),
                glm::sphericalRand( 1.0f ),
                gDefaultElementSpeed,
                gDefaultElementAngularSpeed,
                glm::linearRand( 0.1f, 2.0f )));
    }


    Process::init();
}

void Process4::HandleCollision(Element1* e1, Element1* e2)
{
    Form* pForm1 = e1->Form();
    Form* pForm2 = e2->Form();

    DrawLine drawLine;
    mDrawLine.MakeAnother( &drawLine );
    float len = drawLine.LineBetween( pForm1->Position(), pForm2->Position() );

    drawLine.SetColor( color(len) );
    drawLine.Update();
}

void Process4::CustomDraw( const Mat4& vp)
{
    mDrawLine.Draw( vp );
}

uint32_t Process4::color(float dist)
{
    float t = ( dist * GlobalConfig::Get()->distanceFactor );

    uint8_t i = t * 256.0f;

    Vec3 c( glm::rgbColor<float>(Vec3(i, 1.0f, GlobalConfig::Get()->lightness)) );
    uint8_t b = c.b * 255u;
    uint8_t g = c.g * 255u;
    uint8_t r = c.r * 255u;
    return 0x01000000 | b << 16 | g << 8 | r;
}









Process6::Process6()
{
    init();
}

Process6::~Process6() {}

void Process6::init()
{
    DrawRoutine::Make( &mDrawLine, 500000 );

    mpBounds = new SphereBoundsVolume( 10.0f );
    mpProgress = new Progress();

    for (int idx = 0; idx < NumElements(); ++idx)
    {
        mEls.push_back(
            Element1_B9(
                mpProgress,
                mpBounds,
                glm::ballRand( mpBounds->HalfExtent() * 0.05f ),
                glm::sphericalRand( 1.0f ),
                gDefaultElementSpeed,
                gDefaultElementAngularSpeed,
                glm::linearRand( 0.5f, 2.0f )));
    }


    Process::init();
}

void Process6::Update(float tScale)
{
    mpProgress->tProgress += 0.0002f;
    Process::Update( tScale );
}

void Process6::HandleCollision(Element1_B9* e1, Element1_B9* e2)
{
    Form* pForm1 = e1->Form();
    Form* pForm2 = e2->Form();

    DrawLine drawLine;
    mDrawLine.MakeAnother( &drawLine );
    float len = drawLine.LineBetween( pForm1->Position(), pForm2->Position() );

    drawLine.SetColor( color(len) );
    drawLine.Update();
}

void Process6::CustomDraw( const Mat4& vp)
{
    mDrawLine.Draw( vp );
}

uint32_t Process6::color(float dist)
{
    float t = ( dist * GlobalConfig::Get()->distanceFactor );

    float i = t * 256.0f;

    Vec3 c( glm::rgbColor<float>(Vec3(i, 1.0f, GlobalConfig::Get()->lightness)) );
    uint8_t b = c.b * 255u;
    uint8_t g = c.g * 255u;
    uint8_t r = c.r * 255u;
    return 0x01000000 | b << 16 | g << 8 | r;
}







Process10::Process10()
{
    init();
}

Process10::~Process10() {}

void Process10::init()
{
    DrawRoutine::Make( &mDrawLine, 500000 );

    mpBounds = new SphereBoundsVolume( 14.0f );
    mpProgress = new Progress();

    for (int idx = 0; idx < NumElements(); ++idx)
    {
        Vec3 pos( glm::ballRand(mpBounds->HalfExtent() * 0.8f) );
        mEls.push_back(
            Element1_B9(
                mpProgress,
                mpBounds,
                pos,
                glm::normalize( pos - mpBounds->Origin() ),
                gDefaultElementSpeed,
                gDefaultElementAngularSpeed,
                glm::linearRand( 1.0f, 1.0f )));
    }


    Process::init();
}

void Process10::Update(float tScale)
{
    mpProgress->tProgress += 0.001f;
    Process::Update( tScale );
}

void Process10::HandleCollision(Element1_B9* e1, Element1_B9* e2)
{
    Form* pForm1 = e1->Form();
    Form* pForm2 = e2->Form();

    DrawLine drawLine;
    mDrawLine.MakeAnother( &drawLine );
    float len = drawLine.LineBetween( pForm1->Position(), pForm2->Position() );

    drawLine.SetColor( color(len) );
    drawLine.Update();
}

void Process10::CustomDraw( const Mat4& vp)
{
    mDrawLine.Draw( vp );
}

uint32_t Process10::color(float dist)
{
    float t = ( dist * GlobalConfig::Get()->distanceFactor );

    float i = t * 256.0f;

    Vec3 c( glm::rgbColor<float>( Vec3(i, 1.0f, GlobalConfig::Get()->lightness)) );
    uint8_t b = c.b * 255u;
    uint8_t g = c.g * 255u;
    uint8_t r = c.r * 255u;
    return 0x01000000 | b << 16 | g << 8 | r;
}








Process12::Process12()
{
    init();
}

Process12::~Process12() {}

void Process12::init()
{
    DrawRoutine::Make( &mDrawLine, 1000000 );

    mpBounds = new SphereBoundsVolume( 8.0f );
    mpProgress = new Progress();

    for (int idx = 0; idx < NumElements(); ++idx)
    {
        mEls.push_back(
            Element3(
                mpProgress,
                mpBounds,
                glm::ballRand( mpBounds->HalfExtent() * 0.6f ),
                glm::sphericalRand( 1.0f ),
                0.005f,
                gDefaultElementAngularSpeed * 0.005f,
                glm::linearRand( 1.0f, 3.0f )));
    }


    Process::init();
}

void Process12::HandleCollision(Element3* e1, Element3* e2)
{
    Form* pForm1 = e1->Form();
    Form* pForm2 = e2->Form();

    DrawLine drawLine1;
    DrawLine drawLine2;
    DrawLine drawLine3;
    DrawLine drawLine4;
    DrawLine drawLine5;
    mDrawLine.MakeAnother( &drawLine1 );
    mDrawLine.MakeAnother( &drawLine2 );
    mDrawLine.MakeAnother( &drawLine3 );
    mDrawLine.MakeAnother( &drawLine4 );
    mDrawLine.MakeAnother( &drawLine5 );

    Vec3 p1h = pForm1->Position() + pForm1->direction * pForm1->HalfSize();
    Vec3 p2h = pForm2->Position() + pForm2->direction * pForm2->HalfSize();
    Vec3 p1l = pForm1->Position() - pForm1->direction * pForm1->HalfSize();
    Vec3 p2l = pForm2->Position() - pForm2->direction * pForm2->HalfSize();

    float len1 = drawLine1.LineBetween( pForm1->Position(), pForm2->Position() );
    float len2 = drawLine2.LineBetween( p1h, p2h );
    float len3 = drawLine3.LineBetween( p1l, p2l );
    float len4 = drawLine3.LineBetween( p1h, p1l );
    float len5 = drawLine3.LineBetween( p2h, p2l );

    drawLine1.SetColor( color(len1) );
    drawLine2.SetColor( color(len2) );
    drawLine3.SetColor( color(len3) );
    drawLine4.SetColor( color(len4) );
    drawLine5.SetColor( color(len5) );

    drawLine1.Update();
    drawLine2.Update();
    drawLine3.Update();
    drawLine4.Update();
    drawLine5.Update();
}

void Process12::CustomDraw( const Mat4& vp)
{
    mDrawLine.Draw( vp );
}

uint32_t Process12::color(float dist)
{
    float t = ( dist * GlobalConfig::Get()->distanceFactor );

    float i = t * 256.0f;

    Vec3 c( glm::rgbColor<float>(Vec3(i, 1.0f, t * GlobalConfig::Get()->lightness)) );
    uint8_t b = c.b * 255u;
    uint8_t g = c.g * 255u;
    uint8_t r = c.r * 255u;
    return 0x01000000 | b << 16 | g << 8 | r;
}








Process17::Process17()
{
    init();
}

Process17::~Process17() {}

void Process17::init()
{
    DrawRoutine::Make( &mDrawLine, 1000000 );

    mpBounds = new SphereBoundsVolume( 6.0f );
    mpProgress = new Progress();

    for (int idx = 0; idx < NumElements(); ++idx)
    {
        mEls.push_back(
            Element5(
                mpProgress,
                mpBounds,
                glm::ballRand( mpBounds->HalfExtent() * 0.6f ),
                glm::sphericalRand( 1.0f ),
                0.005f,
                gDefaultElementAngularSpeed * 2.0f,
                glm::linearRand( 2.5f, 3.0f )));
    }


    Process::init();
}

void Process17::HandleCollision(Element5* e1, Element5* e2)
{
    Form* pForm1 = e1->Form();
    Form* pForm2 = e2->Form();

    DrawLine drawLine;
    mDrawLine.MakeAnother( &drawLine );
    float len = drawLine.LineBetween( pForm1->Position(), pForm2->Position() );

    drawLine.SetColor( color(len) );
    drawLine.Update();
}

void Process17::CustomDraw( const Mat4& vp)
{
    mDrawLine.Draw( vp );
}

uint32_t Process17::color(float dist)
{
    float t = ( dist * GlobalConfig::Get()->distanceFactor );

    float i = t * 256.0f;

    Vec3 c( glm::rgbColor<float>(Vec3(i, 1.0f, t * GlobalConfig::Get()->lightness)) );
    uint8_t b = c.b * 255u;
    uint8_t g = c.g * 255u;
    uint8_t r = c.r * 255u;
    return 0x01000000 | b << 16 | g << 8 | r;
}








Process18::Process18()
{
    init();
}

Process18::~Process18() {}

void Process18::init()
{
    DrawRoutine::Make( &mDrawLine, 1000000 );

    mpBounds = new SphereBoundsVolume( 6.0f );
    mpProgress = new Progress();

    for (int idx = 0; idx < NumElements(); ++idx)
    {
        mEls.push_back(
            Element5(
                mpProgress,
                mpBounds,
                glm::ballRand( mpBounds->HalfExtent() * 0.6f ),
                glm::sphericalRand( 1.0f ),
                0.005f,
                gDefaultElementAngularSpeed * 0.1f,
                glm::linearRand( 2.5f, 3.0f )));
    }


    Process::init();
}

void Process18::HandleCollision(Element5* e1, Element5* e2)
{
    Form* pForm1 = e1->Form();
    Form* pForm2 = e2->Form();

    //DrawLine drawLine1;
    DrawLine drawLine2;
    DrawLine drawLine3;
    DrawLine drawLine4;
    DrawLine drawLine5;
    //mDrawLine.MakeAnother( &drawLine1 );
    mDrawLine.MakeAnother( &drawLine2 );
    mDrawLine.MakeAnother( &drawLine3 );
    mDrawLine.MakeAnother( &drawLine4 );
    mDrawLine.MakeAnother( &drawLine5 );

    Vec3 p1h = pForm1->Position() + pForm1->direction * pForm1->HalfSize();
    Vec3 p2h = pForm2->Position() + pForm2->direction * pForm2->HalfSize();
    Vec3 p1l = pForm1->Position() - pForm1->direction * pForm1->HalfSize();
    Vec3 p2l = pForm2->Position() - pForm2->direction * pForm2->HalfSize();

    //float len1 = drawLine1.LineBetween( pForm1->Position(), pForm2->Position() );
    float len2 = drawLine2.LineBetween( p1h, p2h );
    float len3 = drawLine3.LineBetween( p1l, p2l );
    float len4 = drawLine3.LineBetween( p1h, p1l );
    float len5 = drawLine3.LineBetween( p2h, p2l );

    //drawLine1.SetColor( color(len1) );
    drawLine2.SetColor( color(len2) );
    drawLine3.SetColor( color(len3) );
    drawLine4.SetColor( color(len4) );
    drawLine5.SetColor( color(len5) );

    //drawLine1.Update();
    drawLine2.Update();
    drawLine3.Update();
    drawLine4.Update();
    drawLine5.Update();
}

void Process18::CustomDraw( const Mat4& vp)
{
    mDrawLine.Draw( vp );
}

uint32_t Process18::color(float dist)
{
    float t = ( dist * GlobalConfig::Get()->distanceFactor );

    float i = t * 256.0f;

    Vec3 c( glm::rgbColor<float>(Vec3(i, 1.0f, GlobalConfig::Get()->lightness)) );
    uint8_t b = c.b * 255u;
    uint8_t g = c.g * 255u;
    uint8_t r = c.r * 255u;
    return 0x01000000 | b << 16 | g << 8 | r;
}


}

