#include "SceneLightingDrawable.hpp"

#include "Scene.hpp"
#include "DirectionalLightShaderProgram.hpp"
#include "SpotLightShaderProgram.hpp"
#include "PointLightShaderProgram.hpp"

SceneLightingDrawable::SceneLightingDrawable(Scene& scene)
    : Drawable(scene)
{
}

void SceneLightingDrawable::render() const
{
    auto& shaderProgram = *scene().getSceneState().shaderProgram;

    DirectionalLightShaderProgram(shaderProgram).Apply(scene().getDirectionalLight());
    SpotLightShaderProgram(shaderProgram).Apply(scene().getSpotLight());

    PointLightShaderProgram pointLightShaderProgram(shaderProgram);
    auto& pointLights = scene().getPointLights();
    for (unsigned i = 0; i < pointLights.size(); ++i)
    {
        pointLightShaderProgram.Apply(i, pointLights[i]);
    } 

    shaderProgram.SetUniform("pointLightCount", static_cast<int>(pointLights.size()));
}
