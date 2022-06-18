#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include "Texture.hpp"

#include <exception>
#include <string>
#include <GL/glew.h>

class Texture2D : public Texture
{
public:
    class exception : public std::exception
    {
    public:
        exception(const char* name)
            : m_name(name)
        {
        }

        virtual const char* what() const noexcept
        {
            return m_name.c_str();
        }

    private:
        std::string m_name;
    };

    Texture2D();
    Texture2D(const char* filename, unsigned internalFormat
              , unsigned minFilter, unsigned magFilter
              , unsigned wrapS, unsigned wrapT);
    Texture2D(unsigned width, unsigned height
              , unsigned internalFormat
              , unsigned minFilter, unsigned magFilter
              , unsigned wrapS, unsigned wrapT);
    Texture2D(Texture2D&& rhs);
    Texture2D& operator = (Texture2D&& rhs);
    void Resize(int width, int height);
    unsigned operator ()() const override;
    ~Texture2D();
    void Use(unsigned samplerId) const override;
    void Update(const void *data, GLenum format, GLenum type = GL_UNSIGNED_BYTE);
    void Update(unsigned pboId);
private:
    Texture2D(unsigned format, unsigned minFilter, unsigned magFilter
              , unsigned wrapS, unsigned wrapT);
    unsigned m_format;
    unsigned m_tex;
};

#endif //TEXTURE_2D_HPP
