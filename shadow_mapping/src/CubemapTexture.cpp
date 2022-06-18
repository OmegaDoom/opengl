#include "CubemapTexture.hpp"

#include <iostream>
#include <GL/glew.h>
#include "stb_image.h"

CubemapTexture::CubemapTexture(unsigned width, unsigned height
                               , unsigned internalFormat, unsigned format
                               , unsigned minFilter, unsigned magFilter
                               , unsigned wrapS, unsigned wrapT, unsigned wrapR)
{
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);


    for (int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height
                , 0, format, GL_UNSIGNED_BYTE, nullptr);

    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubemapTexture::CubemapTexture(const std::array<std::string, 6>& filenames , unsigned internalFormat
                               , unsigned minFilter, unsigned magFilter
                               , unsigned wrapS, unsigned wrapT, unsigned wrapR)
{
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);

    stbi_set_flip_vertically_on_load(false);

    for (int i = 0; i < 6; ++i)
    {
        int width, height, channels;
        unsigned char *data = stbi_load(filenames[i].c_str(), &width, &height, &channels, 0);
        if (!data)
        {
           const auto msg = std::string("can't load texture ") + filenames[i];
           throw exception(msg); 
        }

        const auto format = 4 == channels ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height
                    , 0, format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubemapTexture::~CubemapTexture()
{
    glDeleteTextures(1, &m_tex);
}

void CubemapTexture::Use(unsigned samplerId) const
{
    glActiveTexture(samplerId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);
}

unsigned CubemapTexture::operator ()() const
{
    return m_tex;
}

