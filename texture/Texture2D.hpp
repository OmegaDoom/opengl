#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <exception>
#include <string>

class Texture2D
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

    Texture2D(const char* filename, unsigned internalFormat, unsigned minFilter, unsigned magFilter
              , unsigned wrapS, unsigned wrapT);
    ~Texture2D();
    void Use(unsigned samplerId) const;
private:
    unsigned m_tex;
};

#endif //TEXTURE_2D_HPP
