#include "Texture2D.hpp"

#include <iostream>
#include <GL/glew.h>
#include "stb_image.h"

Texture2D::Texture2D(const char* filename, unsigned internalFormat, unsigned minFilter, unsigned magFilter
                     , unsigned wrapS, unsigned wrapT)
    : m_minFilter(minFilter)
    , m_magFilter(magFilter)
    , m_wrapS(wrapS)
    , m_wrapT(wrapT)
{
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data)
    {
       const auto msg = std::string("can't load texture ") + filename;
       throw exception(msg); 
    }


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_tex);
}

void Texture2D::Use(unsigned samplerId) const
{
    glActiveTexture(samplerId);
    glBindTexture(GL_TEXTURE_2D, m_tex);
}
