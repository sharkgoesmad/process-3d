#include <string>
#include <SDL.h>
#include <SDL_video.h>
#include "core/log.h"
#include "core/pberror.h"
#include "core/mathutil.h"
#include "glctx.h"
#include "reasobject.h"
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
        return setError(PB_ERROR_SDL);
    }

    if ( setRequiredGLAttribs() == PB_ERR )
    {
        return setError(PB_ERROR_SDL);
    }

    mpWindow = SDL_CreateWindow( mAppName, 0, 0, 1280, 720, SDL_WINDOW_OPENGL );
    if ( mpWindow == NULL )
    {
        return setError(PB_ERROR_SDL);
    }

    mGLContext = SDL_GL_CreateContext( mpWindow );
    if ( mGLContext == NULL )
    {
        return setError(PB_ERROR_SDL);
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
    ReasObject reasObj;

    uint32_t tElapsed;
    float delta = 1.0f;
    float ticksInv = 1.0f / LOOP_TICKS;

    while ( mLoopShouldRun )
    {
        uint32_t tStart = SDL_GetTicks();

        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {
            if ( event.type == SDL_WINDOWEVENT )
            {
                switch ( event.window.event )
                {
                    case SDL_WINDOWEVENT_CLOSE:
                        Stop();
                        break;
                }
            }
        }


        // ****** drawing ******

        ctx.Clear();

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

