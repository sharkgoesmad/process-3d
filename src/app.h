#ifndef APP_H
#define APP_H


#include <string>
#include "core/pbutil.h"


class SDL_Window;
struct _SDL_GameController;


namespace pb
{

class GLCtx;

class App
{

private:

    App(const App& rhs);
    App& operator=(const App& rhs);

public:

    App(const char* appName);
    virtual ~App();

    bool StatusOk();
    PBError Start();
    void Stop();
    const std::string& GetErrorMsg();
    void ShowErrMsgBox();


private:

    PBError init();
    PBError setError(PBError err);
    PBError setRequiredGLAttribs();

private:

    PBError mStatus;
    std::string mErrorMsg;
    const char* mAppName;
    bool mLoopShouldRun;
    SDL_Window* mpWindow;
    void* mGLContext;
    GLCtx* mpGLctx;
    _SDL_GameController* mpController;

};

}

#endif // APP_H
