#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include "core/log.h"
#include "core/io.h"
#include "shadercompiler.h"


namespace pb
{

void showShaderInfoLog(GLuint id, bool success)
{
    int msgLen;
    glGetShaderiv( id, GL_INFO_LOG_LENGTH, &msgLen );

    if ( msgLen > 1 )
    {
        std::vector<char> msg( msgLen );
        glGetShaderInfoLog( id, msgLen, NULL, &msg[0] );

        success == true ? Log::Info( std::string(&msg[0]) ) : Log::Error( std::string(&msg[0]) );
    }
}

void showProgramInfoLog(GLuint id, bool success)
{
    int msgLen;
    glGetProgramiv( id, GL_INFO_LOG_LENGTH, &msgLen );

    if ( msgLen > 0 )
    {
        std::vector<char> msg( msgLen );
        glGetProgramInfoLog( id, msgLen, NULL, &msg[0] );

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
        idShader = PB_GL_INVALID_ID;
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
            idShader = PB_GL_INVALID_ID;
            // LOG
            Log::Error( "Unknown shader type" );
            break;

    }

    return idShader;
}

unsigned int ShaderCompiler::CompileFromFile(const std::string& path, ShaderType type)
{
    std::string strShader;
    GLuint idShader = PB_GL_INVALID_ID;

    if (PB_SUCCESS( IO::ReadText( path, strShader ) ))
    {
        idShader = Compile( strShader, type );
    }

    return idShader;
}

unsigned int ShaderCompiler::Program(unsigned int idVertexShader, unsigned int idFragmentShader)
{
    GLuint idProgram;
    GLint success;

    if ( idVertexShader == PB_GL_INVALID_ID || idFragmentShader == PB_GL_INVALID_ID )
    {
        return PB_GL_INVALID_ID;
    }

    idProgram = glCreateProgram();
    glAttachShader( idProgram, idVertexShader );
    glAttachShader( idProgram, idFragmentShader );
    glLinkProgram( idProgram );

    glGetProgramiv( idProgram, GL_LINK_STATUS, &success );
    showProgramInfoLog( idProgram, success == GL_TRUE );
    if ( success != GL_TRUE )
    {
        idProgram = PB_GL_INVALID_ID;
    }

    return idProgram;
}

void ShaderCompiler::DeleteShader(unsigned int& idShader)
{
    glDeleteShader( idShader );
    idShader = PB_GL_INVALID_ID;
}


};

