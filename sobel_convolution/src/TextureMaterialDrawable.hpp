#ifndef TEXTURE_MATERIAL_DRAWABLE_HPP
#define TEXTURE_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include "TextureMaterial.hpp"

class Scene;
class Texture2D;

class TextureMaterialDrawable : public Drawable
{
public:
    TextureMaterialDrawable(Scene& scene, const Texture2D& diffuse
                            , const Texture2D& specular, float shininess);
private:
    void render() const override;

    const Texture2D& m_diffuse;
    const Texture2D& m_specular;
    TextureMaterial m_material;
};

#endif //TEXTURE_MATERIAL_DRAWABLE_HPP
