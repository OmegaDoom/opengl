#include "ModelTransformShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

ModelTransformShaderProgram::ModelTransformShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram(shaderProgram)
{
}

void ModelTransformShaderProgram::Apply(const glm::mat4& model)
{
    m_shaderProgram.SetUniformMatrix4("model", glm::value_ptr(model));
}
