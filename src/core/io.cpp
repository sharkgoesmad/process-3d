#include <fstream>
#include "io.h"

namespace pb
{

PBError IO::ReadText(const std::string& path, std::string& text)
{
    PBError err = PB_ERR;
    std::ifstream file( path );

    if ( file.good() )
    {
        // set marker at the end of the stream
        file.seekg( 0, std::ios::end );
        // allocate buffer
        text.reserve( file.tellg() );
        // reset the marker
        file.seekg( 0, std::ios::beg );

        // initialize the string with file contents
        text.assign( (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>() );
        
        err = PB_ERR_OK;
    }
    
    return err;
}

}

