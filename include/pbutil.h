#ifndef PBUTIL_H
#define PBUTIL_H

namespace pb
{

typedef int PBError;

#define PB_ERR_OK 0
#define PB_ERR 1

#define PB_SUCCESS(x) (x == PB_ERR_OK)
#define PB_FAILED(x) (x != PB_ERR_OK)

#define PB_DELETE(ptr) do { delete ptr; ptr = NULL; } while (0)
    

/* MATH */
float Clamp(float x, float min, float max);
    
}

#endif  // PBUTIL_H