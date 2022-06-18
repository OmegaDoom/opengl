#ifndef ENVIRONMENT_TEXTURE_MATERIAL_DRAWABLE_HPP
#define ENVIRONMENT_TEXTURE_MATERIAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include <GL/glew.h>

class Scene;
class Texture;

class EnvironmentTextureMaterialDrawable : public Drawable
{
public:
    EnvironmentTextureMaterialDrawable(Scene& scene, const Texture& diffuse, unsigned texUnit = 0);
private:
    void render() const override;

    unsigned m_texUnit;
    const Texture& m_diffuse;
};

#endif //ENVIRONMENT_TEXTURE_MATERIAL_DRAWABLE_HPP
