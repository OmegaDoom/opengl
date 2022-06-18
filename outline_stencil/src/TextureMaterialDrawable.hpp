#ifndef TEXTURE_MATERIAL_DRAWABLE_HPP
#define TEXTURE_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include "TextureMaterial.hpp"
#include <string>

class Texture;
class TextureMaterialDrawable : public Drawable
{
public:
    TextureMaterialDrawable(Scene& scene, const std::string& diffuse_map_name, const std::string& specular_map_name, float shiness);

private:
    void render() const override;
    const Texture &m_diffuse;
    const Texture &m_specular;
    TextureMaterial m_material;
};

#endif // TEXTURE_MATERIAL_DRAWABLE_HPP
