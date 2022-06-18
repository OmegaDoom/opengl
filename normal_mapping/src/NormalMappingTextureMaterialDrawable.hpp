#ifndef NORMAL_MAPPING_TEXTURE_MATERIAL_DRAWABLE_HPP
#define NORMAL_MAPPING_TEXTURE_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include "NormalMappingTextureMaterial.hpp"

class Scene;
class Texture;

class NormalMappingTextureMaterialDrawable : public Drawable
{
public:
    NormalMappingTextureMaterialDrawable(Scene& scene, const Texture& diffuse
                            , const Texture& specular, float shininess, const Texture& normalMappping);
private:
    void render() const override;

    const Texture& m_diffuse;
    const Texture& m_specular;
    const Texture& m_normalMapping;
    NormalMappingTextureMaterial m_material;
};

#endif //NORMAL_MAPPING_TEXTURE_MATERIAL_DRAWABLE_HPP
