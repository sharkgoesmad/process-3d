#include <string>
#include <SDL.h>
#include <SDL_video.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "camera.h"

#include "core/log.h"
#include "core/pberror.h"
#include "core/mathutil.h"
#include "glctx.h"
#include "bulletproxy.h"
#include "reasobject.h"
#include "reas/process.h"
#include "app.h"






namespace pb
{


const unsigned int LOOP_TARGET_FPS = 60;
const float LOOP_TICKS = 1000 / float( LOOP_TARGET_FPS );


App::App(const char* appName) :
    mStatus( PB_ERR_OK ),
    mAppName( appName ),
    mLoopShouldRun( true ),
    mpWindow( NULL ),
    mGLContext( NULL )
{
    mStatus = init();
}

App::~App()
{
    Log::Release();
    SDL_DestroyWindow( mpWindow );
    SDL_Quit();
}

bool App::StatusOk()
{
    return mStatus == PB_ERR_OK;
}

PBError App::init()
{
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0 )
    {
        return setError( PB_ERROR_SDL );
    }

    if ( setRequiredGLAttribs() == PB_ERR )
    {
        return setError( PB_ERROR_SDL );
    }

    mpWindow = SDL_CreateWindow( mAppName, 0, 0, 1280, 720, SDL_WINDOW_OPENGL );
    if ( mpWindow == NULL )
    {
        return setError( PB_ERROR_SDL );
    }

    mGLContext = SDL_GL_CreateContext( mpWindow );
    if ( mGLContext == NULL )
    {
        return setError( PB_ERROR_SDL );
    }

    Log::Info( "App initialized successfully" );
    return PB_ERR_OK;
}

PBError App::setRequiredGLAttribs()
{
    int ret = 0;

    ret |= SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    ret |= SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    ret |= SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    ret |= SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    ret |= SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    ret |= SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    ret |= SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

    ret |= SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    return ret == 0 ? PB_ERR_OK : PB_ERR;
}

PBError App::Start()
{
    SDL_ShowWindow( mpWindow );

    GLCtx ctx( mGLContext, mpWindow );

    Camera cam;
    //SDL_SetRelativeMouseMode( SDL_TRUE );
    //cam.SetPosition( 0.0f, 0.0f, -5.0f );

    Quat quaternion( Vec3(0,1,0), Vec3(0,1,0) );
    Quat quatInverse = glm::inverse( quaternion );
    Vec3 offset;
    float radius = 5.0;
    float cachedRadius = 5.0;

    float th = glm::pi<float>();
    float phi = glm::pi<float>() * 0.5f;

    reas::Process proc;

    uint32_t tElapsed;
    float delta = 1.0f;
    float ticksInv = 1.0f / LOOP_TICKS;

    while ( mLoopShouldRun )
    {
        uint32_t tStart = SDL_GetTicks();

        {
            int x, y;
            uint32_t mouseBtnState = SDL_GetRelativeMouseState( &x, &y );

            if ( ((mouseBtnState & SDL_BUTTON_LMASK) && (x != 0 || y != 0)) || radius != cachedRadius )
            {
                cachedRadius = radius;
                offset = cam.Position() - Vec3(0,0,0);
                offset = glm::rotate( quaternion, offset );
                th = atan2f( offset.x, offset.z );
                phi = atan2f( sqrt(offset.x * offset.x + offset.z * offset.z), offset.y );
                //float radius = glm::length( offset );

                float throttle = 0.001f * delta;
                th += -x * throttle;
                phi += -y * throttle;

                //phi = glm::mod( phi, glm::pi<float>() );
                //th = glm::mod( th, glm::pi<float>() * 2.0f );
                phi = Clamp( phi, 0, glm::pi<float>() );
                //th = Clamp( th, )

//                Vec3 pos(
//                    5 * glm::sin(phi) * glm::sin(th),
//                    5 * glm::cos(phi),
//                    5 * glm::sin(phi) * glm::cos(th));
                offset = Vec3(
                    radius * glm::sin(phi) * glm::sin(th),
                    radius * glm::cos(phi),
                    radius * glm::sin(phi) * glm::cos(th));


                offset = glm::rotate( quatInverse, offset );

                cam.SetPosition( Vec3(0,0,0) + offset );

                cam.LookAt(Vec3(0,0,0));
            }
        }

        SDL_Event event;
        if ( SDL_PollEvent(NULL) )
        {
            unsigned ev_count = 0;
            while ( SDL_PollEvent(&event) )
            {
                if ( event.type == SDL_MOUSEMOTION )
                {
//                    SDL_MouseMotionEvent& ev = event.motion;
//                    if ( ev.state & SDL_BUTTON_LMASK )
//                    {
//
//                    }
                }
                else if ( event.type == SDL_MOUSEWHEEL )
                {
                    SDL_MouseWheelEvent& ev = event.wheel;
                    radius -= ev.y;
                }
                else if ( event.type == SDL_WINDOWEVENT )
                {
                    switch ( event.window.event )
                    {
                        case SDL_WINDOWEVENT_CLOSE:
                            Stop();
                            break;
                    }
                }

                ++ev_count;
            }
        }

        ctx.Clear();


        proc.Update();
        // TODO provide actual time
        BulletProxy::Get()->Step( LOOP_TICKS * 0.001f );

        // ****** drawing ******

        // enable for debug drawing
        proc.Draw( cam.VP() );
        reas::Process::ReasAlpha()->Draw( cam.VP() );

        ctx.Present();




        // ***** calc delta and cap ****

        tElapsed = SDL_GetTicks() - tStart;

        // cap at target fps
        if ( tElapsed < uint32_t(LOOP_TICKS) )
        {
            SDL_Delay( uint32_t(LOOP_TICKS) - tElapsed );
        }

        delta = (SDL_GetTicks() - tStart) * ticksInv;
        delta = Clamp( delta, 0.1f, 5.0f );
    }

    return PB_ERR_OK;
}

void App::Stop()
{
    mLoopShouldRun = false;
}

PBError App::setError(PBError err)
{
    return mStatus = err;
}

const std::string& App::GetErrorMsg()
{
    switch (mStatus)
    {
        case PB_ERR_OK:
            mErrorMsg = std::string( "No Error." );
            break;

        case PB_ERROR_SDL:
            mErrorMsg = std::string( SDL_GetError() );
            break;

        default:
            mErrorMsg = std::string( "Unknown Error (" + std::to_string(mStatus) + ")." );
            break;
    }


    if (mErrorMsg.empty())
    {
        mErrorMsg = std::string( "Unknown Error (" + std::to_string(mStatus) + ")." );
    }


    return mErrorMsg;
}

void App::ShowErrMsgBox()
{
    Log::Debug( GetErrorMsg() );
    SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, mAppName, GetErrorMsg().c_str(), NULL );
}


}

