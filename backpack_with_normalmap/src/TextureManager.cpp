#include "TextureManager.hpp"

#include <GL/glew.h>

TextureManager::TextureManager()
{
}

TextureManager& TextureManager::instance()
{
    static TextureManager instance;
    return instance; 
}


void TextureManager::LoadTexture(const std::string& path, std::string name, unsigned internalFormat)
{
    m_textures.try_emplace(std::move(name), path.c_str(), internalFormat, GL_LINEAR
            , GL_LINEAR, GL_REPEAT, GL_REPEAT);
}

const Texture2D& TextureManager::operator [] (const std::string& name) const
{
    return m_textures.at(name);
}
