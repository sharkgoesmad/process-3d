#ifndef PBCONFIG_H
#define PBCONFIG_H

namespace pb
{


#ifdef PB_DEBUG
    #define PB_RESOURCE_PATH_PREFIX "../resources"
#else
    #define PB_RESOURCE_PATH_PREFIX "./resources"
#endif

#define PB_RESOURCE_PATH(res) (PB_RESOURCE_PATH_PREFIX "/" res)

    
};

#endif  // PBCONFIG_H