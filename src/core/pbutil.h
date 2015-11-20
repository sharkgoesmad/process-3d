#ifndef PB_PBUTIL_H
#define PB_PBUTIL_H

#include "log.h"

namespace pb
{

typedef int PBError;

#define PB_ERR_OK 0
#define PB_ERR 1

#define PB_SUCCESS(x) (x == PB_ERR_OK)
#define PB_FAILED(x) (x != PB_ERR_OK)

#define PB_DELETE(ptr) do { delete ptr; ptr = NULL; } while (0)

#define PB_GL_INVALID_ID 0x0

#define XSTRINGIFY(s) STRINGIFY(s)
#define STRINGIFY(s) #s

#define PB_ALLOCFAIL_RETURN() ({ Log::Error(__FILE__ ": line " XSTRINGIFY(__LINE__) ": Alloc failed"); return PB_ERR; })


template <typename T>
class scoped_ptr
{
    
private:

    scoped_ptr(const scoped_ptr& rhs);
    scoped_ptr& operator=(const scoped_ptr& rhs);

public:

    scoped_ptr(T* ptr) : mpManaged( ptr ) {};
    ~scoped_ptr() { PB_DELETE( mpManaged ); };
    T* Disown() {
        T* ptr = mpManaged;
        mpManaged = NULL;
        return ptr;
    };
    
    inline bool IsNull() { return mpManaged == NULL; };
    inline operator bool() { return !IsNull(); };
    
private:
    
    T* mpManaged;

};


    
}

#endif  // PB_PBUTIL_H