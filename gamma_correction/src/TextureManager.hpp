#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include "Texture.hpp"
#include "Texture2D.hpp"

#include <unordered_map>
#include <string>
#include <memory>

class TextureManager
{
public:
    static TextureManager& instance();
    void LoadTexture(const std::string& path, std::string name);
    void LoadTexture(const std::string& path, std::string name, int wrapS, int wrapT);
    void LoadTextureS(const std::string& path, std::string name);
    void LoadTextureS(const std::string& path, std::string name, int wrapS, int wrapT);
    void LoadTexture(const std::array<std::string, 6>& path, std::string name);
    void LoadTexture(const std::array<std::string, 6>& path, std::string name, int wrapS, int wrapT, int wrapR);

    template <typename ...Ts>
    void AddEmplace(const std::string name, Ts&&... args)
    {
        m_textures.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name))
                            , std::forward_as_tuple(std::make_unique<Texture2D>(args... )));
    }

    const Texture& operator [] (const std::string& name) const;
    
private:
    TextureManager();
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};

#endif //TEXTURE_MANAGER_HPP
