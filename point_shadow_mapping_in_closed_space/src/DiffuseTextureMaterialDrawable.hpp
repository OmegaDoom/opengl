#ifndef DIFFUSE_TEXTURE_MATERIAL_DRAWABLE_HPP
#define DIFFUSE_TEXTURE_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include <GL/glew.h>

class Scene;
class Texture;

class DiffuseTextureMaterialDrawable : public Drawable
{
public:
    DiffuseTextureMaterialDrawable(Scene& scene, const Texture& diffuse, unsigned texUnit = 0);
private:
    void render() const override;

    unsigned m_texUnit;
    const Texture& m_diffuse;
};

#endif //DIFFUSE_TEXTURE_MATERIAL_DRAWABLE_HPP
