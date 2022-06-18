#include "MaterialDrawable.hpp"

#include "Scene.hpp"
#include "MaterialManager.hpp"
#include "MaterialShaderProgram.hpp"

MaterialDrawable::MaterialDrawable(Scene& scene, int material)
    : Drawable(scene)
    , m_material(material)
{
}

void MaterialDrawable::render() const
{
    //setting material
    auto& material = MaterialManager::instance()[static_cast<MATERIAL_ID>(m_material)];
    MaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(material);
}
