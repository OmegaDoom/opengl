#ifndef PBR_MATERIAL_DRAWABLE_HPP
#define PBR_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include "PbrMaterial.hpp"

class Scene;

class PbrMaterialDrawable : public Drawable
{
public:
    PbrMaterialDrawable(Scene& scene, const PbrMaterial& material);
private:
    void render() const override;

    PbrMaterial m_material;
};

#endif //PBR_MATERIAL_DRAWABLE_HPP
