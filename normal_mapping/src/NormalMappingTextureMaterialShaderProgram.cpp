#include "NormalMappingTextureMaterialShaderProgram.hpp"

#include "NormalMappingTextureMaterial.hpp"
#include "ShaderProgram.hpp"

NormalMappingTextureMaterialShaderProgram::NormalMappingTextureMaterialShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void NormalMappingTextureMaterialShaderProgram::Apply(const NormalMappingTextureMaterial& material)
{
    m_shaderProgram.SetUniform("material.diffuse", material.diffuseTextureId);
    m_shaderProgram.SetUniform("material.specular", material.specularTextureId);
    m_shaderProgram.SetUniform("material.normalMapping", material.normalMappingTextureId);
    m_shaderProgram.SetUniform("material.shininess", material.shininess * 128); 
}
