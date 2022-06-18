#include "EnvironmentTextureMaterialShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"

EnvironmentTextureMaterialShaderProgram::EnvironmentTextureMaterialShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void EnvironmentTextureMaterialShaderProgram::Apply(int texUnit)
{
    m_shaderProgram.SetUniform("environment", texUnit);
}
