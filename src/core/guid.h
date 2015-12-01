#ifndef PB_GUID_H
#define PB_GUID_H

namespace pb
{

typedef unsigned int guid_t;

class GUID
{

public:

    static const guid_t InvalidGUID;
    guid_t Get();

};

};

#endif  // PB_GUID_H
