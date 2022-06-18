#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include "Texture.hpp"
#include <unordered_map>
#include <string>
#include <memory>

class TextureManager
{
public:
    static TextureManager& instance();
    void LoadTexture(const std::string& path, std::string name);
    const Texture& operator [] (const std::string& name) const;
    
private:
    TextureManager();
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};

#endif //TEXTURE_MANAGER_HPP
