#include "ShadowMapTextureMaterialShaderProgram.hpp"

#include "ShadowMapTextureMaterial.hpp"
#include "ShaderProgram.hpp"

ShadowMapTextureMaterialShaderProgram::ShadowMapTextureMaterialShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void ShadowMapTextureMaterialShaderProgram::Apply(const ShadowMapTextureMaterial& material)
{
    m_shaderProgram.SetUniform("material.diffuse", material.diffuseTextureId);
    m_shaderProgram.SetUniform("material.specular", material.specularTextureId);
    m_shaderProgram.SetUniform("material.shininess", material.shininess * 128); 
    m_shaderProgram.SetUniform("material.shadowMap", material.shadowMapTextureId); 
}
