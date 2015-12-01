#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL_video.h>
#include "core/log.h"
#include "glctx.h"

namespace pb
{


GLCtx::GLCtx(void* pSDLGLContext, SDL_Window* pSDLWindow) :
    mpGLContext( pSDLGLContext ),
    mpWindow( pSDLWindow )
{
    mStatus = init();
}

GLCtx::~GLCtx()
{
}


PBError GLCtx::init()
{
    if ( mpWindow == NULL || mpGLContext == NULL )
    {
        Log::Error( "Invalid window or context" );
        return PB_ERR;
    }

    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK )
    {
        Log::Error( "Could not initialize GLEW" );
        return PB_ERR;
    }

    // TODO check GL version
    int val;
    SDL_GL_GetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, &val );
    SDL_GL_GetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, &val );
    int mask = SDL_GL_CONTEXT_PROFILE_CORE;
    SDL_GL_GetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, &val );

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    //glEnable( GL_DEPTH_TEST );
    glEnable( GL_DOUBLEBUFFER );
    //glEnable( GL_ALPHA_TEST );
    glEnable( GL_BLEND );

    // traditional blending
    //glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
    //glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO );
    glBlendFunc(GL_ONE, GL_ONE);

    glLineWidth( 5.0f );
    glEnable( GL_LINE_SMOOTH );
    //glEnable( GL_POLYGON_SMOOTH );
    //glDisable( GL_DITHER );
    //glDisable( GL_MULTISAMPLE );

    return PB_ERR_OK;
}


void GLCtx::Present()
{
    SDL_GL_SwapWindow( mpWindow );
}

void GLCtx::Clear()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


}

