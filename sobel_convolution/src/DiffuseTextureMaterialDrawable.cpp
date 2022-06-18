#include "DiffuseTextureMaterialDrawable.hpp"

#include "Scene.hpp"
#include "Texture2D.hpp"
#include "DiffuseTextureMaterialShaderProgram.hpp"

DiffuseTextureMaterialDrawable::DiffuseTextureMaterialDrawable(Scene& scene, const Texture2D& diffuse)
    : Drawable(scene)
    , m_diffuse{ diffuse }
{
}

void DiffuseTextureMaterialDrawable::render() const
{
    DiffuseTextureMaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(GL_TEXTURE0);

    m_diffuse.Use(GL_TEXTURE0);
}
