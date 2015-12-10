#ifndef PB_REAS_GLOBALCONFIG_H
#define PB_REAS_GLOBALCONFIG_H

namespace pb
{

namespace reas
{

class GlobalConfig
{

    static GlobalConfig* mpInstance;

private:

    GlobalConfig(const GlobalConfig& rhs);
    GlobalConfig& operator=(const GlobalConfig& rhs);

public:

    static GlobalConfig* Get();
    static void Release();

    void Reset();

private:

    GlobalConfig();
    ~GlobalConfig();

public:

    float distanceFactor;
    float lightness;

};


inline GlobalConfig* GlobalConfig::Get()
{
    if ( mpInstance == NULL )
    {
        mpInstance = new GlobalConfig();
        mpInstance->Reset();
    }

    return mpInstance;
}

}

}

#endif  // PB_REAS_GLOBALCONFIG_H
