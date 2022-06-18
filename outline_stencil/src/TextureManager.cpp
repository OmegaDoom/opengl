#include "TextureManager.hpp"

#include "NoneTexture.hpp"
#include "Texture2D.hpp"

#include <memory>
#include <GL/glew.h>

TextureManager::TextureManager()
{
    m_textures.insert(std::make_pair("none", std::make_unique<NoneTexture>()));
}

TextureManager& TextureManager::instance()
{
    static TextureManager instance;
    return instance; 
}


void TextureManager::LoadTexture(const std::string& path, std::string name)
{
    m_textures.insert(std::make_pair(std::move(name), std::make_unique<Texture2D>(path.c_str(), GL_RGBA, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT)));
}

const Texture& TextureManager::operator [] (const std::string& name) const
{
    return *m_textures.at(name);
}
