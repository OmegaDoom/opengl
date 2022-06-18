#ifndef TEXTURE_MATERIAL_DRAWABLE_HPP
#define TEXTURE_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include "TextureMaterial.hpp"

class Scene;
class Texture;

class TextureMaterialDrawable : public Drawable
{
public:
    TextureMaterialDrawable(Scene& scene, const Texture& diffuse
                            , const Texture& specular, float shininess);
private:
    void render() const override;

    const Texture& m_diffuse;
    const Texture& m_specular;
    TextureMaterial m_material;
};

#endif //TEXTURE_MATERIAL_DRAWABLE_HPP
