#ifndef IO_H
#define IO_H

#include <string>
#include "pbutil.h"

namespace pb
{

class IO
{
    
private:

    IO(){};
    IO(const IO& rhs);
    IO& operator=(const IO& rhs);

public:

    static PBError ReadText(const std::string& path, std::string& outText);

};

}

#endif // IO_H
