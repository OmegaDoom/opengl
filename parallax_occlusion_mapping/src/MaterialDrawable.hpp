#ifndef MATERIAL_DRAWABLE_HPP
#define MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"

class Scene;

class MaterialDrawable : public Drawable
{
public:
    MaterialDrawable(Scene& scene, int material);
private:
    void render() const override;

    int m_material;
};

#endif //MATERIAL_DRAWABLE_HPP
