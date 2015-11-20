#ifndef SHADERCOMPILER_H
#define SHADERCOMPILER_H

namespace pb
{

class ShaderCompiler
{

public:

    enum ShaderType
    {
        Shader_Vertex = 0,
        Shader_Fragment
    };

private:

    ShaderCompiler();
    ShaderCompiler(const ShaderCompiler& rhs);
    ShaderCompiler& operator=(const ShaderCompiler& rhs);

public:

    static unsigned int Compile(const std::string& strShader, ShaderType type);
    static unsigned int CompileFromFile(const std::string& path, ShaderType type);
    static unsigned int Program(unsigned int idVertexShader, unsigned int idFragmentShader);

};

}

#endif // SHADERCOMPILER_H
