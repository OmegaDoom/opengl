#include "ShadowMatricesShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

ShadowMatricesShaderProgram::ShadowMatricesShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void ShadowMatricesShaderProgram::Apply(const glm::mat4* matrices, int num)
{
    m_shaderProgram.SetUniformMatrices4("shadowMatrices", num, reinterpret_cast<const float*>(matrices)); 
}
