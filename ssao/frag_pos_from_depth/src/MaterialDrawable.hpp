#ifndef MATERIAL_DRAWABLE_HPP
#define MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"

class Scene;
struct Material;

class MaterialDrawable : public Drawable
{
public:
    MaterialDrawable(Scene& scene, const Material& material);
private:
    void render() const override;

    const Material& m_material;
};

#endif //MATERIAL_DRAWABLE_HPP
