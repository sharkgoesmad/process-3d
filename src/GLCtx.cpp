#include <GL/gl.h>
#include <SDL_video.h>

#include "GLCtx.h"

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

void GLCtx::Present()
{
    SDL_GL_SwapWindow( mpWindow );
}

void GLCtx::Clear()
{
    glClear( GL_COLOR_BUFFER_BIT );
}


}

