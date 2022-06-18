#include "NormalShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include "Constants.hpp"
#include "Macro.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

NormalShaderProgram::NormalShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram(shaderProgram)
{
}

void NormalShaderProgram::Apply(const glm::mat4& viewModel)
{
    auto inverseViewModel = glm::inverse(viewModel);
    auto normalTransform = glm::transpose(inverseViewModel);

    m_shaderProgram.SetUniformMatrix4(STR(NORMAL_TRANSFORM), glm::value_ptr(normalTransform));
}
