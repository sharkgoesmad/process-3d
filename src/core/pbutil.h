#ifndef PBUTIL_H
#define PBUTIL_H

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

#define STRINGIFY(s) #s
#define PB_ALLOCFAIL_RETURN(src,ln) ({ Log::Error(src ": line " STRINGIFY(ln) " Alloc failed"); return PB_ERR; })
    
}

#endif  // PBUTIL_H