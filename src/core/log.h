#ifndef LOG_H
#define LOG_H

#include <string>


namespace pb
{

    
    
class Log
{

private:

    Log(const Log& rhs);
    Log& operator=(const Log& rhs);
    Log();
    ~Log();

public:

    static void Release();

    static void Debug(const char* msg);
    static void Error(const char* msg);
    static void Info(const char* msg);
    static void Warn(const char* msg);
    
    static inline void Debug(const std::string& msg) { Debug( msg.c_str() ); };
    static inline void Error(const std::string& msg) { Error( msg.c_str() ); };
    static inline void Info(const std::string& msg) { Info( msg.c_str() ); };
    static inline void Warn(const std::string& msg) { Warn( msg.c_str() ); };
    
private:
    
    static Log* instance();

private:

    static Log* mInstance;

};



}

#endif // LOG_H
