#include "Shader.hpp"

#include <GL/glew.h>

template<unsigned ShaderType>
Shader<ShaderType>::Shader(const std::string& source)
{
    m_shader = glCreateShader(ShaderType);
    const auto src = source.c_str();
    glShaderSource(m_shader, 1, &src, nullptr);
    glCompileShader(m_shader);

    int success;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(m_shader, sizeof(infoLog), nullptr, infoLog);
        throw exception(infoLog);
    }
}

template<unsigned ShaderType>
Shader<ShaderType>::~Shader()
{
    glDeleteShader(m_shader);
}

template<unsigned ShaderType>
unsigned Shader<ShaderType>::ShaderId() const
{
    return m_shader;
}

template class Shader<GL_VERTEX_SHADER>;
template class Shader<GL_FRAGMENT_SHADER>;
template class Shader<GL_GEOMETRY_SHADER>;
