#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <GL/glew.h>
#include "Texture2D.hpp"

class TextureManager
{
public:
    static TextureManager& instance();
    void LoadTexture(const std::string& path, std::string name, unsigned internalFormat = GL_RGBA);
    template <typename ...Ts>
    void AddEmplace(const std::string name, Ts&&... args)
    {
        m_textures.try_emplace(std::move(name) , args...);
    }
    const Texture2D& operator [] (const std::string& name) const;
    
private:
    TextureManager();
    std::unordered_map<std::string, Texture2D> m_textures;
};

#endif //TEXTURE_MANAGER_HPP