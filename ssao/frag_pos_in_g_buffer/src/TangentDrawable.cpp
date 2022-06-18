#include "TangentDrawable.hpp"

#include "Scene.hpp"
#include "TangentShaderProgram.hpp"

TangentDrawable::TangentDrawable(Scene& scene, const glm::vec3& tangent)
    : Drawable(scene)
    , m_tangent{tangent}
{
}

TangentDrawable::~TangentDrawable()
{
}

void TangentDrawable::render() const
{
    TangentShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_tangent);
}
