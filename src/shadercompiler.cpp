#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include "core/log.h"
#include "core/io.h"
#include "shadercompiler.h"

namespace pb
{

#define PB_SHADERCOMPILER_INVALID_ID 0
    
void showShaderInfoLog(GLuint id, bool success)
{
    int msgLen;
    glGetShaderiv( id, GL_INFO_LOG_LENGTH, &msgLen );
    
    if ( msgLen > 0 )
    {
        std::vector<char> msg( msgLen );
        glGetShaderInfoLog( id, msgLen, NULL, &msg[0] );

        success == true ? Log::Info( std::string(&msg[0]) ) : Log::Error( std::string(&msg[0]) );
    }
}    

unsigned int compile(const std::string& strShader, GLenum type)
{
    GLuint idShader = glCreateShader( type );
    const char* pStrShader = strShader.c_str();
    
    glShaderSource( idShader, 1, &pStrShader, NULL );
    glCompileShader( idShader );
    
    
    GLint success;
    
    glGetShaderiv( idShader, GL_COMPILE_STATUS, &success );
    showShaderInfoLog( idShader, success == GL_TRUE );
    if ( success != GL_TRUE )
    {
        glDeleteShader( idShader );
        idShader = PB_SHADERCOMPILER_INVALID_ID;
    }
    
    return idShader;
}

unsigned int ShaderCompiler::Compile(const std::string& strShader, ShaderType type)
{
    GLuint idShader;
    
    switch ( type )
    {
        
        case ShaderType::Shader_Vertex:
            idShader = compile( strShader, GL_VERTEX_SHADER );
            break;
            
        case ShaderType::Shader_Fragment:
            idShader = compile( strShader, GL_FRAGMENT_SHADER );
            break;
            
        default:
            idShader = PB_SHADERCOMPILER_INVALID_ID;
            // LOG
            printf( "Unknown shader type.\n" );
            break;

    }
    
    return idShader;
}

unsigned int ShaderCompiler::CompileFromFile(const std::string& path, ShaderType type)
{
    std::string strShader;
    GLuint idShader = PB_SHADERCOMPILER_INVALID_ID;
    
    if (PB_SUCCESS( IO::ReadText( path, strShader ) ))
    {
        idShader = Compile( strShader, type );
    }
    
    return idShader;
}

}

