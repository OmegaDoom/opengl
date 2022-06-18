#include "Texture2D.hpp"

#include <iostream>
#include <GL/glew.h>
#include "FileSystem.hpp"
#include "stb_image.h"
#include "nv_dds.h"

Texture2D::Texture2D(const char* filename, unsigned internalFormat, unsigned minFilter, unsigned magFilter
        , unsigned wrapS, unsigned wrapT)
{
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    bool isDDS = FileSystem::IsDDSExtension(filename);
    if (!isDDS)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        auto data = stbi_load(filename, &width, &height, &channels, 0);
        if (!data)
        {
            const auto msg = std::string("can't load texture ") + filename;
            throw exception(msg); 
        }

	const auto format = 4 == channels ? GL_RGBA : 3 == channels ? GL_RGB :  GL_RED;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        nv_dds::CDDSImage image;
        image.load(filename);

        if (!image.is_compressed())
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.get_width()
                    , image.get_height(), 0, image.get_format(), GL_UNSIGNED_BYTE, image);

            for (int i = 0; i < image.get_num_mipmaps(); i++)
            {
                glTexImage2D(GL_TEXTURE_2D, i + 1, internalFormat 
                        , image.get_mipmap(i).get_width(), image.get_mipmap(i).get_height()
                        , 0, image.get_format(), GL_UNSIGNED_BYTE, image.get_mipmap(i));
            }
        }
        else
        {
            glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalFormat 
                    ,image.get_width(), image.get_height(), 0, image.get_size(),
                    image);

            for (int i = 0; i < image.get_num_mipmaps(); i++)
            {
                nv_dds::CSurface mipmap = image.get_mipmap(i);
                glCompressedTexImage2D(GL_TEXTURE_2D, i+1, internalFormat,
                        mipmap.get_width(), mipmap.get_height(), 0, mipmap.get_size(),
                        mipmap);
            }
        }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(Texture2D&& rhs)
    : m_tex(rhs.m_tex)
{
    rhs.m_tex = 0;
}

Texture2D& Texture2D::operator = (Texture2D&& rhs)
{
    m_tex = rhs.m_tex;

    rhs.m_tex = 0;

    return *this;
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
