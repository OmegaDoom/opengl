#include "ShaderProgramDrawable.hpp"

#include "Scene.hpp"
#include "ShaderManager.hpp"

ShaderProgramDrawable::ShaderProgramDrawable(Scene& scene, const std::string& name)
    : Drawable(scene)
    , m_shaderProgram{ ShaderManager::instance().getShaderProgram(name) }
{
}

void ShaderProgramDrawable::render() const
{
    m_shaderProgram.Use();
    scene().getSceneState().shaderProgram = &m_shaderProgram;
}
