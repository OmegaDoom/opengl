#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP
#include <GL/glew.h>
#include <exception>
#include <string>

class ShaderProgram
{
public:
    class exception : public std::exception
    {
    public:
        exception(std::string name)
            : m_name(std::move(name))
        {
        }

        virtual const char* what() const noexcept
        {
            return m_name.c_str();
        }

    private:
        std::string m_name;
    };

    template <typename ...Ts>
    ShaderProgram(Ts&&... args)
    {
        m_shaderProgram = glCreateProgram();
        link(args...);
        glLinkProgram(m_shaderProgram);

        int success;
        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(m_shaderProgram, sizeof(infoLog), nullptr, infoLog);
            throw exception(infoLog);
        }
    }

    ~ShaderProgram()
    {
        glDeleteProgram(m_shaderProgram);
    }

    void Use() const
    {
        glUseProgram(m_shaderProgram);
    }

    bool SetUniformMatrix4(const char* name, const float *matrix)
    {
        int location = glGetUniformLocation(m_shaderProgram, name);
        if (-1 == location)
            return false;
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix); 
        return true;
    }

    bool SetUniform(const char* name, float a, float b, float c, float d)
    {
        int location = glGetUniformLocation(m_shaderProgram, name);
        if (-1 == location)
            return false;
        glUniform4f(location, a, b, c, d);
        return true;
    }

    bool SetUniform(const char* name, float a, float b, float c)
    {
        int location = glGetUniformLocation(m_shaderProgram, name);
        if (-1 == location)
            return false;
        glUniform3f(location, a, b, c);
        return true;
    }

    bool SetUniform(const char* name, int value)
    {
        int location = glGetUniformLocation(m_shaderProgram, name);
        if (-1 == location)
            return false;
        glUniform1i(location, value); 
        return true;
    }

    bool SetUniform(const char* name, unsigned value)
    {
        int location = glGetUniformLocation(m_shaderProgram, name);
        if (-1 == location)
            return false;
        glUniform1ui(location, value); 
        return true;
    }

    bool SetUniform(const char* name, float value)
    {
        int location = glGetUniformLocation(m_shaderProgram, name);
        if (-1 == location)
            return false;
        glUniform1f(location, value); 
        return true;
    }

private:
    template <typename T, typename ...Us>
    void link(T&& arg, Us&&... args)
    {
        link(arg);
        link(args...);
    }

    template <typename T>
    void link(T&& arg)
    {
        glAttachShader(m_shaderProgram, arg.ShaderId());
    }

    unsigned m_shaderProgram;
};

#endif //SHADER_PROGRAM_HPP
