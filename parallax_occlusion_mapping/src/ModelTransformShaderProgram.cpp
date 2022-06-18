#include "ModelTransformShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include "Constants.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

ModelTransformShaderProgram::ModelTransformShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram(shaderProgram)
{
}

void ModelTransformShaderProgram::Apply(const glm::mat4& model)
{
    m_shaderProgram.SetUniformMatrix4(MODEL_TRANSFORM, glm::value_ptr(model));
}
