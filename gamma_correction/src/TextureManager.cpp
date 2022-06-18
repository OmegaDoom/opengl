#include "TextureManager.hpp"

#include "Texture2D.hpp"
#include "CubemapTexture.hpp"

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
                        , std::forward_as_tuple(std::make_unique<Texture2D>(path.c_str(), GL_RGBA, GL_LINEAR
                        , GL_LINEAR, GL_REPEAT, GL_REPEAT)));
}

void TextureManager::LoadTexture(const std::string& path, std::string name, int wrapS, int wrapT)
{
    m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name))
                        , std::forward_as_tuple(std::make_unique<Texture2D>(path.c_str(), GL_RGBA, GL_LINEAR
                        , GL_LINEAR, wrapS, wrapT)));
}

void TextureManager::LoadTextureS(const std::string& path, std::string name)
{
    m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name))
                        , std::forward_as_tuple(std::make_unique<Texture2D>(path.c_str(), GL_SRGB_ALPHA, GL_LINEAR
                        , GL_LINEAR, GL_REPEAT, GL_REPEAT)));
}

void TextureManager::LoadTextureS(const std::string& path, std::string name, int wrapS, int wrapT)
{
    m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name))
                        , std::forward_as_tuple(std::make_unique<Texture2D>(path.c_str(), GL_SRGB_ALPHA, GL_LINEAR
                        , GL_LINEAR, wrapS, wrapT)));
}
void TextureManager::LoadTexture(const std::array<std::string, 6>& path, std::string name)
{
    m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name))
                        , std::forward_as_tuple(std::make_unique<CubemapTexture>(path, GL_RGBA, GL_LINEAR
                        , GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT)));
}

void TextureManager::LoadTexture(const std::array<std::string, 6>& path, std::string name, int wrapS, int wrapT, int wrapR)
{
    m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name))
                        , std::forward_as_tuple(std::make_unique<CubemapTexture>(path, GL_RGBA, GL_LINEAR
                        , GL_LINEAR, wrapS, wrapT, wrapR)));
}

const Texture& TextureManager::operator [] (const std::string& name) const
{
    return *m_textures.at(name);
}
