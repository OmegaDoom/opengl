#ifndef DIFFUSE_TEXTURE_MATERIAL_DRAWABLE_HPP
#define DIFFUSE_TEXTURE_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"

class Scene;
class Texture2D;

class DiffuseTextureMaterialDrawable : public Drawable
{
public:
    DiffuseTextureMaterialDrawable(Scene& scene, const Texture2D& diffuse);
private:
    void render() const override;

    const Texture2D& m_diffuse;
};

#endif //DIFFUSE_TEXTURE_MATERIAL_DRAWABLE_HPP
