#include "BillboardModelNormalTransformDrawable.hpp"

#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "ModelTransformShaderProgram.hpp"
#include "NormalTransformShaderProgram.hpp"

#include <glm/gtc/type_ptr.hpp>

BillboardModelNormalTransformDrawable::BillboardModelNormalTransformDrawable(Scene& scene, const glm::mat4& model)
    : Drawable(scene)
    , m_model(model)
    , m_billboard(1.f)
{
}

BillboardModelNormalTransformDrawable::~BillboardModelNormalTransformDrawable()
{
}

void BillboardModelNormalTransformDrawable::update()
{
    auto view = scene().getCamera().View();
    view[3][0] = 0.f;
    view[3][1] = 0.f;
    view[3][2] = 0.f;

    m_billboard = inverse(view);
    m_billboard[3][0] = m_model[3][0]; 
    m_billboard[3][1] = m_model[3][1]; 
    m_billboard[3][2] = m_model[3][2]; 
}

void BillboardModelNormalTransformDrawable::render() const
{
    auto model = m_model;
    model[3][0] = 0.f;
    model[3][1] = 0.f;
    model[3][2] = 0.f;

    auto& shaderProgram = *scene().getSceneState().shaderProgram;
    ModelTransformShaderProgram(shaderProgram).Apply(m_billboard * model);

    auto viewModel = scene().getCamera().View() * m_billboard * model;
    NormalTransformShaderProgram(shaderProgram).Apply(viewModel);
}
