#include "EnvironmentTextureMaterialDrawable.hpp"

#include "Scene.hpp"
#include "Texture.hpp"
#include "EnvironmentTextureMaterialShaderProgram.hpp"

EnvironmentTextureMaterialDrawable::EnvironmentTextureMaterialDrawable(Scene& scene, const Texture& diffuse, unsigned texUnit)
    : Drawable(scene)
    , m_texUnit{ texUnit }
    , m_diffuse{ diffuse }
{
}

void EnvironmentTextureMaterialDrawable::render() const
{
    m_diffuse.Use(GL_TEXTURE0 + m_texUnit);
    EnvironmentTextureMaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_texUnit);
}
