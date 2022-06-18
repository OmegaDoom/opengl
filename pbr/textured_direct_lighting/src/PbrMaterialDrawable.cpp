#include "PbrMaterialDrawable.hpp"

#include "Scene.hpp"
#include "PbrMaterialShaderProgram.hpp"

PbrMaterialDrawable::PbrMaterialDrawable(Scene& scene, const PbrMaterial& material)
    : Drawable(scene)
    , m_material(material)
{
}

void PbrMaterialDrawable::render() const
{
    //setting material
    PbrMaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_material);
}
