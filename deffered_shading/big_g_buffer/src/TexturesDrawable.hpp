#ifndef TEXTURES_DRAWABLE_HPP
#define TEXTURES_DRAWABLE_HPP

#include <initializer_list>
#include <functional>
#include "Drawable.hpp"
#include "Textures.hpp"

class Scene;
class Texture;

class TexturesDrawable : public Drawable
{
public:
    template<typename T>
    TexturesDrawable(Scene& scene, std::initializer_list<T> textures)
        : Drawable(scene)
        , m_textures{0, {0, 1, 2, 3, 4, 5, 6, 7}}
    {
        int texture_count = 0;
        for (auto&& texture : textures)
        {
            m_tex[texture_count] = &static_cast<const Texture&>(texture);
            ++texture_count;
        }
        m_textures.count = texture_count;
    }
    
private:
    void render() const override;
    Textures m_textures;    
    const Texture* m_tex[8];
};

#endif //TEXTURES_DRAWABLE_HPP
