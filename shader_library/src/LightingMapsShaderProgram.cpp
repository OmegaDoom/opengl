#include "LightingMapsShaderProgram.hpp"

#include "LightingMapsMaterial.hpp"
#include "ShaderProgram.hpp"

LightingMapsShaderProgram::LightingMapsShaderProgram(ShaderProgram& shaderProgram)
        : m_shaderProgram{ shaderProgram }
{
}

void LightingMapsShaderProgram::Apply(const LightingMapsMaterial& material)
{
    m_shaderProgram.SetUniform("material.diffuse", material.diffuse);
    m_shaderProgram.SetUniform("material.specular", material.specular);
    m_shaderProgram.SetUniform("material.shininess", material.shininess);
}
