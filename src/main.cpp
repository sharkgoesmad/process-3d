#include "SDL_main.h"
#include "app.h"

int main(int argc, char** argv)
{
    pb::App app("Process 3D");

    if ( app.StatusOk() )
    {
        app.Start();
    }
    else
    {
        app.ShowErrMsgBox();
    }

    return 0;
}
