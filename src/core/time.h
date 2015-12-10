#ifndef PB_TIME_H
#define PB_TIME_H

#include <chrono>
#include <string>

namespace pb
{

template < typename T >
struct ScopedTimer
{
    std::chrono::time_point< std::chrono::steady_clock > start;
    const std::string msg;
    bool disabled;
    bool showMsg;

    ScopedTimer(const std::string& message, bool showMsg = true);
    ScopedTimer(const ScopedTimer& rhs);
    ~ScopedTimer();

    T Check();
    T Mark();
    void Reset();

};


}

#endif  // PB_TIME_H
