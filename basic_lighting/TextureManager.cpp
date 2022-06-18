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


void TextureManager::LoadTexture(const std::string& path, std::string name)
{
    m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name))
                        , std::forward_as_tuple(path.c_str(), GL_RGBA, GL_LINEAR
                        , GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER));
}

const Texture2D& TextureManager::operator [] (const std::string& name) const
{
    return m_textures.at(name);
}
