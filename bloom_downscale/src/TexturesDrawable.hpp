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
    TexturesDrawable(Scene& scene, std::initializer_list<std::reference_wrapper<const Texture>> textures);
private:
    void render() const override;

    Textures m_textures;    
    const Texture* m_tex[8];
};

#endif //TEXTURES_DRAWABLE_HPP
