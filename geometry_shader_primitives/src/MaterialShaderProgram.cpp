#include "MaterialShaderProgram.hpp"

#include "Material.hpp"
#include "ShaderProgram.hpp"

MaterialShaderProgram::MaterialShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void MaterialShaderProgram::Apply(const Material& material)
{
    m_shaderProgram.SetUniform("material.ambient", material.ambient.x, material.ambient.y, material.ambient.z);
    m_shaderProgram.SetUniform("material.diffuse", material.diffuse.x, material.diffuse.y, material.diffuse.z);
    m_shaderProgram.SetUniform("material.specular",material.specular.x, material.specular.y, material.specular.z); 
    m_shaderProgram.SetUniform("material.shininess", material.shininess * 128);
}
