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
                        , GL_LINEAR, GL_REPEAT, GL_REPEAT));
}

void TextureManager::LoadTexture(const std::string& path, std::string name, int wrapS, int wrapT)
{
    m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name))
                        , std::forward_as_tuple(path.c_str(), GL_RGBA, GL_LINEAR
                        , GL_LINEAR, wrapS, wrapT));
}

Texture2D& TextureManager::operator [] (const std::string& name)
{
    return m_textures.at(name);
}
