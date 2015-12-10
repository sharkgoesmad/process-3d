#include <string>
#include <SDL.h>
#include <SDL_video.h>
#include <AntTweakBar.h>

#include "camera.h"
#include "core/log.h"
#include "core/pberror.h"
#include "core/mathutil.h"
#include "core/time.h"
#include "glctx.h"
#include "reas/processes.h"
#include "app.h"



namespace pb
{

#define PB_APP_WINDOWSIZE_W 1920
#define PB_APP_WINDOWSIZE_H 1080

const unsigned int LOOP_TARGET_FPS = 60;
const float LOOP_TICKS = 1000 / float( LOOP_TARGET_FPS );


App::App(const char* appName) :
    mStatus( PB_ERR_OK ),
    mAppName( appName ),
    mLoopShouldRun( true ),
    mpWindow( NULL ),
    mpController( NULL ),
    mGLContext( NULL ),
    mpGLctx( NULL )
{
    mStatus = init();
}

App::~App()
{
    BulletProxy::Release();
    PB_DELETE( mpGLctx );
    TwTerminate();
    Log::Release();
    SDL_GameControllerClose( mpController );
    SDL_DestroyWindow( mpWindow );
    SDL_QuitSubSystem( SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER );
    SDL_VideoQuit();
    SDL_Quit();
}

bool App::StatusOk()
{
    return mStatus == PB_ERR_OK;
}

PBError App::init()
{
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER ) != 0 )
    {
        return setError( PB_ERROR_SDL );
    }

    if ( setRequiredGLAttribs() == PB_ERR )
    {
        return setError( PB_ERROR_SDL );
    }

    mpWindow = SDL_CreateWindow( mAppName, 0, 0, PB_APP_WINDOWSIZE_W, PB_APP_WINDOWSIZE_H, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN );
    if ( mpWindow == NULL )
    {
        return setError( PB_ERROR_SDL );
    }

    mGLContext = SDL_GL_CreateContext( mpWindow );
    if ( mGLContext == NULL )
    {
        return setError( PB_ERROR_SDL );
    }

    // TODO check error
    mpGLctx = new GLCtx( mGLContext, mpWindow );

    TwDefine( "GLOBAL fontscaling=1.1" );
    if ( TwInit(TW_OPENGL_CORE, NULL) == 0 )
    {
        return setError( PB_ERROR_TW );
    }

    TwWindowSize( PB_APP_WINDOWSIZE_W, PB_APP_WINDOWSIZE_H );

    for ( int idx = 0; idx < SDL_NumJoysticks(); ++idx )
    {
        if ( SDL_IsGameController(idx) )
        {
            mpController = SDL_GameControllerOpen( idx );

            if ( mpController != NULL )
            {
                break;
            }
        }
    }

    if ( mpController == NULL )
    {
        return setError( PB_ERROR_CONTROLLER );
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

struct ProcState
{
    reas::IProcess* pproc;
    reas::Processes selectedProc;
    ScopedTimer< unsigned int > procTimer;
};

void TW_CALL selectedProcCallback(void* pData)
{
    ProcState* pProcState = static_cast< ProcState* >( pData );
    PB_DELETE( pProcState->pproc );

    switch ( pProcState->selectedProc )
    {
        case reas::Processes::Proc4:
            pProcState->pproc = new reas::Process4();
            break;
        case reas::Processes::Proc6:
            pProcState->pproc = new reas::Process6();
            break;
        case reas::Processes::Proc10:
            pProcState->pproc = new reas::Process10();
            break;
        case reas::Processes::Proc12:
            pProcState->pproc = new reas::Process12();
                break;
        case reas::Processes::Proc17:
            pProcState->pproc = new reas::Process17();
            break;
        case reas::Processes::Proc18:
            pProcState->pproc = new reas::Process18();
            break;
        default:
            Log::Error( "Invalid Proc identifier: "+STR(pProcState->selectedProc) );
            break;
    }

    // reset generation timer
    pProcState->procTimer.Reset();
}

void TW_CALL resetColorConfigCallback(void* pData)
{
    reas::GlobalConfig* pGlobalConfig = static_cast< reas::GlobalConfig* >( pData );
    pGlobalConfig->Reset();
}

void TW_CALL toggleDebugModeCallback(void* pData)
{
    ProcState* pProcState = static_cast< ProcState* >( pData );
    pProcState->pproc->ToggleDebugMode();
}

void TW_CALL exitCallback(void* pData)
{
    App* pApp = static_cast< App* >( pData );
    pApp->Stop();
}

PBError App::Start()
{
    SDL_ShowWindow( mpWindow );

    TwDefine( "GLOBAL fontsize=3 buttonalign=left" );
    TwBar* pTweakBar( TwNewBar("Settings") );
    bool barFocused = false;
    TwDefine( "Settings position='0 0' size='330 200' valueswidth=120 visible=false" );
    ProcState procState = { new reas::Process4(), reas::Processes::Proc4, ScopedTimer< unsigned int >( "", false ) };

    // autoplay stuff
    bool autoPlay = true;
    int playDuartion = 60 * 1000;   // 60 seconds
    ScopedTimer< double > autoPlayTimer( "", false );

    // max progress duration
    int maxProgressDuration = 60 * 1000; // 1 minute;

    {
        TwEnumVal procsEv[] = {

            { reas::Processes::Proc4, "Process 4" },
            { reas::Processes::Proc6, "Process 6" },
            { reas::Processes::Proc10, "Process 10" },
            { reas::Processes::Proc12, "Process 12" },
            { reas::Processes::Proc17, "Process 17" },
            { reas::Processes::Proc18, "Process 18" },

        };
        TwType procsT = TwDefineEnum( "ProcessTypes", procsEv, 6 );

        TwAddVarRW( pTweakBar, "Processes", procsT, &procState.selectedProc, NULL );
        TwAddButton( pTweakBar, "Run Selected", selectedProcCallback, &procState, NULL );
        TwAddVarRW( pTweakBar, "Autoplay", TW_TYPE_BOOL8, &autoPlay, NULL );    // TODO set type based on bool size

        TwAddSeparator( pTweakBar, NULL, NULL );

        TwAddVarRW( pTweakBar, "Distance Factor", TW_TYPE_FLOAT, &reas::GlobalConfig::Get()->distanceFactor, "min=0.1 max=8.0 step=0.1" );
        TwAddVarRW( pTweakBar, "Color Lightness", TW_TYPE_FLOAT, &reas::GlobalConfig::Get()->lightness, "min=0.001 max=0.1 step=0.001" );
        TwAddButton( pTweakBar, "Reset", resetColorConfigCallback, reas::GlobalConfig::Get(), NULL );

        TwAddSeparator( pTweakBar, NULL, NULL );

        TwAddButton( pTweakBar, "Toggle Debug Mode", toggleDebugModeCallback, &procState, NULL );

        TwAddSeparator( pTweakBar, NULL, NULL );

        TwAddButton( pTweakBar, "Exit", exitCallback, this, NULL );
    }



    Camera cam;
    //SDL_SetRelativeMouseMode( SDL_TRUE );
    cam.SetPosition( 0.0f, 0.0f, 8.0f );

    Quat quaternion( Vec3(0,1,0), Vec3(0,1,0) );
    Quat quatInverse = glm::inverse( quaternion );
    Vec3 offset;
    float radius = 8.0;
    float cachedRadius = 8.0;

    float th = glm::pi<float>();
    float phi = glm::pi<float>() * 0.5f;

    uint32_t tElapsed;
    float tScale = 1.0f;
    float ticksInv = 1.0f / LOOP_TICKS;

    while ( mLoopShouldRun )
    {
        uint32_t tStart = SDL_GetTicks();

        {
            int x = 0, y = 0;
            //uint32_t mouseBtnState = SDL_GetRelativeMouseState( &x, &y );

            SDL_Event event;
            if ( SDL_PollEvent(NULL) )
            {
                unsigned ev_count = 0;
                while ( SDL_PollEvent(&event) )
                {
                    if ( event.type != SDL_MOUSEMOTION && event.type != SDL_MOUSEBUTTONDOWN )
                    {
                        TwEventSDL( &event, 2, 0 );
                    }

                    if ( event.type == SDL_CONTROLLERBUTTONDOWN )
                    {
                        switch ( event.cbutton.button )
                        {
                            case SDL_CONTROLLER_BUTTON_START:
                                if ( !barFocused )
                                {
                                    TwDefine( "Settings visible=true" );
                                    TwMouseMotion( 1, 1 );
                                    barFocused = true;
                                }
                                else
                                {
                                    TwMouseMotion( -100, -100 );
                                    TwDefine( "Settings visible=false" );
                                    barFocused = false;
                                }
                                //barFocused = !barFocused;
                                break;

                            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                                TwKeyPressed( TW_KEY_UP, 0 );
                                break;

                            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                                TwKeyPressed( TW_KEY_DOWN, 0 );
                                break;

                            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                                TwKeyPressed( TW_KEY_LEFT, 0 );
                                break;

                            case SDL_CONTROLLER_BUTTON_A:
                            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                                TwKeyPressed( TW_KEY_RIGHT, 0 );
                                break;
                        }
                    }

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

            if ( mpController )
            {
                x = -1 * SDL_GameControllerGetAxis( mpController, SDL_CONTROLLER_AXIS_RIGHTX ) >> 10;
                y = -1 * SDL_GameControllerGetAxis( mpController, SDL_CONTROLLER_AXIS_RIGHTY ) >> 10;
                int val = SDL_GameControllerGetAxis( mpController, SDL_CONTROLLER_AXIS_LEFTY ) >> 12;
                radius += Clamp( val, -7, 7 ) * 0.02f;
            }

            // sanitize input
            radius = Clamp( radius, 2.0f, 35.0f );

            //if ( (mouseBtnState & SDL_BUTTON_LMASK && (x != 0 || y != 0)) || radius != cachedRadius )
            if ( (x != 0 || y != 0) || radius != cachedRadius )
            {
                cachedRadius = radius;
                offset = cam.Position() - Vec3(0,0,0);
                offset = glm::rotate( quaternion, offset );
                th = atan2f( offset.x, offset.z );
                phi = atan2f( sqrt(offset.x * offset.x + offset.z * offset.z), offset.y );

                float throttle = 0.001f * tScale;
                th += -x * throttle;
                phi += -y * throttle;

                phi = Clamp( phi, 0.001f, glm::pi<float>() - 0.001f );

                offset = Vec3(
                    radius * glm::sin(phi) * glm::sin(th),
                    radius * glm::cos(phi),
                    radius * glm::sin(phi) * glm::cos(th));


                offset = glm::rotate( quatInverse, offset );

                cam.SetPosition( Vec3(0,0,0) + offset );
                cam.LookAt(Vec3(0,0,0));
            }
        }



        mpGLctx->Clear();

        // ****** updates ******

        // throttle
        if ( procState.procTimer.Check() < maxProgressDuration )
        {
            procState.pproc->Update( tScale );
            BulletProxy::Get()->Step( LOOP_TICKS * 0.001f * tScale );
        }

        // ****** drawing ******

        procState.pproc->Draw( cam.VP() );
        TwDraw();



        mpGLctx->Present();



        // ****** late updates ******

        if ( autoPlay )
        {
            // must sanitize limited precision
            if ( std::isnan(autoPlayTimer.Check()) )
            {
                autoPlayTimer.Reset();
            }

            if ( autoPlayTimer.Check() > playDuartion )
            {
                procState.selectedProc = (reas::Processes)((procState.selectedProc+1) % (reas::Processes::ProcCount));
                selectedProcCallback( &procState );
                autoPlayTimer.Reset();
            }
        }


        // ***** calc delta and cap ****

        tElapsed = SDL_GetTicks() - tStart;

        // cap at target fps
        if ( tElapsed < uint32_t(LOOP_TICKS) )
        {
            SDL_Delay( uint32_t(LOOP_TICKS) - tElapsed );
        }

        tScale = (SDL_GetTicks() - tStart) * ticksInv;
        tScale = Clamp( tScale, 0.1f, 5.0f );
    }

    PB_DELETE( procState.pproc );
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

        case PB_ERROR_TW:
            mErrorMsg = std::string( TwGetLastError() );
            break;

        case PB_ERROR_CONTROLLER:
            mErrorMsg = std::string( "No game controller detected." );
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

