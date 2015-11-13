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
    glClearColor( 0.1f, 0.1f, 0.15f, 1.0f );
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
    
    if ( glewInit() != GLEW_OK )
    {
        Log::Error( "Could not initialize GLEW" );
        return PB_ERR;
    }
    
    return PB_ERR_OK;
}


void GLCtx::Present()
{
    SDL_GL_SwapWindow( mpWindow );
}

void GLCtx::Clear()
{
    glClear( GL_COLOR_BUFFER_BIT );
}


}

