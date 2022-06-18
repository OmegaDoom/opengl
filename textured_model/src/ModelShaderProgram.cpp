#include "ModelShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

ModelShaderProgram::ModelShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram(shaderProgram)
{
}

void ModelShaderProgram::Apply(const glm::mat4& model)
{
    m_shaderProgram.SetUniformMatrix4("model", glm::value_ptr(model));
}
