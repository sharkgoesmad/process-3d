#ifndef GLCTX_H
#define GLCTX_H


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
    
    void* mpGLContext;
    SDL_Window* mpWindow;

};

}

#endif // GLCTX_H
