#include "PointLightShadowMapTransformationsDrawable.hpp"

#include "Scene.hpp"
#include "ShadowMatricesShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp> 

PointLightShadowMapTransformationsDrawable::PointLightShadowMapTransformationsDrawable(Scene& scene, const glm::vec3& lightPos)
    : Drawable{ scene }
    , m_lightPos{ lightPos }
{
}

void PointLightShadowMapTransformationsDrawable::update()
{
    const float SHADOW_WIDTH = 10.f;
    const float SHADOW_HEIGHT = 10.f;
    const float aspect = SHADOW_WIDTH/SHADOW_HEIGHT;
    const float near = 1.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, m_far);

    m_shadowTransforms[0] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
    m_shadowTransforms[1] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
    m_shadowTransforms[2] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    m_shadowTransforms[3] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0));
    m_shadowTransforms[4] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0));
    m_shadowTransforms[5] = shadowProj * glm::lookAt(m_lightPos, m_lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0));
}

void PointLightShadowMapTransformationsDrawable::render() const
{
    auto& shaderProgram = *scene().getSceneState().shaderProgram;
    ShadowMatricesShaderProgram(shaderProgram).Apply(m_shadowTransforms.data(), 6);
    shaderProgram.SetUniform("lightPos", m_lightPos.x, m_lightPos.y, m_lightPos.z);
    shaderProgram.SetUniform("far_plane", m_far);
}
