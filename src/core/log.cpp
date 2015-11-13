#include <string>
#include <SDL_log.h>
#include "pbutil.h"
#include "log.h"

namespace pb
{

Log* Log::mInstance = NULL;

Log::Log()
{
}

Log::~Log()
{
}

Log* Log::instance()
{
    if ( mInstance == NULL )
    {
        mInstance = new Log();
    }
    
    return mInstance;
}

void Log::Release()
{
    if ( mInstance != NULL )
    {
        PB_DELETE( mInstance );
    }
}


// TODO Implement file output log

void Log::Debug(const char* msg)
{
#ifdef PB_DEBUG
    SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, msg );
#endif
}

void Log::Error(const char* msg)
{
    SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, msg );
}

void Log::Info(const char* msg)
{
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, msg );
}

void Log::Warn(const char* msg)
{
    SDL_LogWarn( SDL_LOG_CATEGORY_APPLICATION, msg );
}


}

