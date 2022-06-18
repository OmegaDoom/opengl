#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <unordered_map>
#include <string>
#include "Texture2D.hpp"

class TextureManager
{
public:
    static TextureManager& instance();
    void LoadTexture(const std::string& path, std::string name);
    void LoadTexture(const std::string& path, std::string name, int wrapS, int wrapT);
    
    template <typename ...Ts>
    void AddEmplace(const std::string name, Ts&&... args)
    {
        m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name))
                            , std::forward_as_tuple(args...));
    }

    Texture2D& operator [] (const std::string& name);
    
private:
    TextureManager();
    std::unordered_map<std::string, Texture2D> m_textures;
};

#endif //TEXTURE_MANAGER_HPP
