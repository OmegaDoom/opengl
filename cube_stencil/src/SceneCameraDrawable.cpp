#include "SceneCameraDrawable.hpp"

#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>

SceneCameraDrawable::SceneCameraDrawable(Scene& scene)
    : Drawable(scene)
{
}

void SceneCameraDrawable::render() const
{
    auto& shaderProgram = *scene().getSceneState().shaderProgram;

    shaderProgram.SetUniformMatrix4("view", glm::value_ptr(scene().getCamera().View()));
    shaderProgram.SetUniformMatrix4("projection", glm::value_ptr(scene().Projection()));
}
