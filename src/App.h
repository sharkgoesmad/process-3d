#ifndef APP_H
#define APP_H


#include <string>
#include "pbutil.h"


class SDL_Window;


namespace pb
{

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

};

}

#endif // APP_H
