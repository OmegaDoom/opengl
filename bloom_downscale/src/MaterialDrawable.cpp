#include "MaterialDrawable.hpp"

#include "Scene.hpp"
#include "MaterialManager.hpp"
#include "MaterialShaderProgram.hpp"

MaterialDrawable::MaterialDrawable(Scene& scene, const Material& material)
    : Drawable(scene)
    , m_material(material)
{
}

void MaterialDrawable::render() const
{
    //setting material
    MaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_material);
}
