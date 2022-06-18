#ifndef PARALLAX_MAPPING_TEXTURE_MATERIAL_DRAWABLE_HPP
#define PARALLAX_MAPPING_TEXTURE_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include "ParallaxMappingTextureMaterial.hpp"

class Scene;
class Texture;

class ParallaxMappingTextureMaterialDrawable : public Drawable
{
public:
    ParallaxMappingTextureMaterialDrawable(Scene& scene, const Texture& diffuse
                            , const Texture& specular, const Texture& normalMappping, const Texture& disp, const float shineness);
private:
    void render() const override;

    const Texture& m_diffuse;
    const Texture& m_specular;
    const Texture& m_normalMapping;
    const Texture& m_disp;
    ParallaxMappingTextureMaterial m_material;
};

#endif //PARALLAX_MAPPING_TEXTURE_MATERIAL_DRAWABLE_HPP
