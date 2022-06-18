#include "ModelNormalDrawable.hpp"

#include "Scene.hpp"
#include "MaterialManager.hpp"
#include "ModelShaderProgram.hpp"
#include "NormalShaderProgram.hpp"

ModelDrawable::ModelDrawable(Scene& scene, glm::mat4x4 model)
        : Drawable(scene)
        , m_model(model)
{
}

void ModelDrawable::render() const
{
    auto& shaderProgram = *scene().getSceneState().shaderProgram;
    ModelShaderProgram(shaderProgram).Apply(m_model);

    auto viewModel = scene().getCamera().View() * m_model;
    NormalShaderProgram(shaderProgram).Apply(viewModel);
}
