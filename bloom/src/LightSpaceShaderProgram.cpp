#include "LightSpaceShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

LightSpaceShaderProgram::LightSpaceShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram(shaderProgram)
{
}

void LightSpaceShaderProgram::Apply(const glm::mat4& lightSpace)
{
    m_shaderProgram.SetUniformMatrix4("lightSpace", glm::value_ptr(lightSpace));
}
