#include "PbrMaterialShaderProgram.hpp"

#include "PbrMaterial.hpp"
#include "ShaderProgram.hpp"

PbrMaterialShaderProgram::PbrMaterialShaderProgram(ShaderProgram& shaderProgram)
        : m_shaderProgram{ shaderProgram }
{
}

void PbrMaterialShaderProgram::Apply(const PbrMaterial& material)
{
    m_shaderProgram.SetUniform("material.albedo", material.albedo);
    m_shaderProgram.SetUniform("material.metallic", material.metallic);
    m_shaderProgram.SetUniform("material.roughness", material.roughness);
    m_shaderProgram.SetUniform("material.ao", material.ao);
    m_shaderProgram.SetUniform("material.normal", material.normal);
}
