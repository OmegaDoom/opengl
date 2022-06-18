#ifndef LIGHTING_MAPS_DRAWABLE_HPP
#define LIGHTING_MAPS_DRAWABLE_HPP

#include "Drawable.hpp"
#include "LightingMapsMaterial.hpp"
#include <string>

class Texture2D;
class TextureMaterialDrawable : public Drawable
{
public:
    TextureMaterialDrawable(Scene& scene, const std::string& diffuse_map_name, const std::string& specular_map_name, float shiness);

private:
    void render() const override;
    const Texture2D &m_diffuse;
    const Texture2D &m_specular;
    LightingMapsMaterial m_material;
};

#endif // LIGHTING_MAPS_DRAWABLE_HPP
