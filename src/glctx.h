#ifndef GLCTX_H
#define GLCTX_H

#include "core/pbutil.h"


class SDL_Window;


namespace pb
{

class GLCtx
{
    
private:

    GLCtx(const GLCtx& rhs);
    GLCtx& operator=(const GLCtx& rhs);

public:

    GLCtx(void* pSDLGLContext, SDL_Window* pSDLWindow);
    ~GLCtx();
    
    void Present();
    void Clear();
    
private:
    
    PBError init();
    
private:
    
    PBError mStatus;
    void* mpGLContext;
    SDL_Window* mpWindow;

};

}

#endif // GLCTX_H
