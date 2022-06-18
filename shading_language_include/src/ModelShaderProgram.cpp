#include "ModelShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include "Constants.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

ModelShaderProgram::ModelShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram(shaderProgram)
{
}

void ModelShaderProgram::Apply(const glm::mat4& model)
{
    m_shaderProgram.SetUniformMatrix4(STR(MODEL_TRANSFORM), glm::value_ptr(model));
}
