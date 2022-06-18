#include "NormalMapTextureMaterialShaderProgram.hpp"

#include "NormalMapTextureMaterial.hpp"
#include "ShaderProgram.hpp"

NormalMapTextureMaterialShaderProgram::NormalMapTextureMaterialShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void NormalMapTextureMaterialShaderProgram::Apply(const NormalMapTextureMaterial& material)
{
    m_shaderProgram.SetUniform("material.diffuse", material.diffuseTextureId);
    m_shaderProgram.SetUniform("material.specular", material.specularTextureId);
    m_shaderProgram.SetUniform("material.shininess", material.shininess * 128); 
    m_shaderProgram.SetUniform("material.normalmap", material.normalMapTextureId);
}
