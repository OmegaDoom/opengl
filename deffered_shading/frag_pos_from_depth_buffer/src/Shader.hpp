#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <exception>

template <unsigned ShaderType>
class Shader
{
public:
    class exception : public std::exception
    {
    public:
        exception(std::string name)
            : m_name(std::move(name))
        {
        }

        const char* what() const noexcept override
        {
            return m_name.c_str();
        }

    private:
        std::string m_name;
    };

    Shader(const std::string& source);
    ~Shader();
    unsigned ShaderId() const;
private:
    unsigned m_shader;
};

#endif //SHADER_HPP
