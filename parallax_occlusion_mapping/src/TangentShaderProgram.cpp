#include "TangentShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include "Constants.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

TangentShaderProgram::TangentShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram(shaderProgram)
{
}

void TangentShaderProgram::Apply(const glm::vec3& tangent)
{
    m_shaderProgram.SetUniform(TANGENT, tangent.x, tangent.y, tangent.z);
}
