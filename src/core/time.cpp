#include <ratio>
#include "pbutil.h"
#include "time.h"

namespace pb
{

typedef std::ratio<1,1000> TMillis;

template < typename T >
ScopedTimer< T >::ScopedTimer(const std::string& message, bool showMsg) :
    msg( message ),
    start( std::chrono::steady_clock::now() ),
    disabled( false ),
    showMsg( showMsg )
{}

template < typename T >
ScopedTimer< T >::ScopedTimer(const ScopedTimer& rhs) :
    msg( rhs.msg ),
    start( rhs.start ),
    disabled( rhs.disabled ),
    showMsg( rhs.showMsg )
{}

template < typename T >
ScopedTimer< T >::~ScopedTimer()
{
    if ( !disabled )
    {
        Check();
    }
}

template < typename T >
T ScopedTimer< T >::Check()
{
    std::chrono::time_point< std::chrono::steady_clock > end( std::chrono::steady_clock::now() );
    std::chrono::duration< T, TMillis > duration =
        std::chrono::duration_cast< std::chrono::duration<T,TMillis> > ( end - start );

    if ( showMsg )
    {
        Log::Info( msg + ": " + STR(duration.count()) );
    }

    return duration.count();
}

template < typename T >
T ScopedTimer< T >::Mark()
{
    T time = Check();
    disabled = true;

    return time;
}

template < typename T >
void ScopedTimer< T >::Reset()
{
    start = std::chrono::steady_clock::now();
}

template struct ScopedTimer< float >;
template struct ScopedTimer< double >;
template struct ScopedTimer< int >;
template struct ScopedTimer< unsigned int >;

}
