#include "DiffuseTextureMaterialShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"

DiffuseTextureMaterialShaderProgram::DiffuseTextureMaterialShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void DiffuseTextureMaterialShaderProgram::Apply(const int texUnit)
{
    m_shaderProgram.SetUniform("diffuse", texUnit);
}
