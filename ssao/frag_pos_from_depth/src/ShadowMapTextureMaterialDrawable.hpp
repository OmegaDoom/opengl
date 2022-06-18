#ifndef SHADOW_MAP_TEXTURE_MATERIAL_DRAWABLE_HPP
#define SHADOW_MAP_TEXTURE_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include "ShadowMapTextureMaterial.hpp"

class Scene;
class Texture;

class ShadowMapTextureMaterialDrawable : public Drawable
{
public:
    ShadowMapTextureMaterialDrawable(Scene& scene, const Texture& diffuse
                            , const Texture& specular, float shininess, const Texture& shadowMap);
private:
    void render() const override;

    const Texture& m_diffuse;
    const Texture& m_specular;
    const Texture& m_shadowMap;
    ShadowMapTextureMaterial m_material;
};

#endif //SHADOW_MAP_TEXTURE_MATERIAL_DRAWABLE_HPP
