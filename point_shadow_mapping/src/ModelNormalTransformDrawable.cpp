#include "ModelNormalTransformDrawable.hpp"

#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "ModelTransformShaderProgram.hpp"
#include "NormalTransformShaderProgram.hpp"

#include <glm/gtc/type_ptr.hpp>

ModelNormalTransformDrawable::ModelNormalTransformDrawable(Scene& scene, const glm::mat4& model)
    : Drawable(scene)
    , m_model(model)
{
}

ModelNormalTransformDrawable::~ModelNormalTransformDrawable()
{
}

void ModelNormalTransformDrawable::render() const
{
    auto& shaderProgram = *scene().getSceneState().shaderProgram;
    ModelTransformShaderProgram(shaderProgram).Apply(m_model);

    auto viewModel = scene().getCamera().View() * m_model;
    NormalTransformShaderProgram(shaderProgram).Apply(viewModel);
}
