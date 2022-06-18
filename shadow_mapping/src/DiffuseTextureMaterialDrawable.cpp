#include "DiffuseTextureMaterialDrawable.hpp"

#include "Scene.hpp"
#include "Texture.hpp"
#include "DiffuseTextureMaterialShaderProgram.hpp"

DiffuseTextureMaterialDrawable::DiffuseTextureMaterialDrawable(Scene& scene, const Texture& diffuse, unsigned texUnit)
    : Drawable(scene)
    , m_texUnit{ texUnit }
    , m_diffuse{ diffuse }
{
}

void DiffuseTextureMaterialDrawable::render() const
{
    m_diffuse.Use(GL_TEXTURE0 + m_texUnit);
    DiffuseTextureMaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_texUnit);
}
