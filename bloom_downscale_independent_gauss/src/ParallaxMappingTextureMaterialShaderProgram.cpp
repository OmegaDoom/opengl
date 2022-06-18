#include "ParallaxMappingTextureMaterialShaderProgram.hpp"

#include "ParallaxMappingTextureMaterial.hpp"
#include "ShaderProgram.hpp"

ParallaxMappingTextureMaterialShaderProgram::ParallaxMappingTextureMaterialShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void ParallaxMappingTextureMaterialShaderProgram::Apply(const ParallaxMappingTextureMaterial& material)
{
    m_shaderProgram.SetUniform("material.diffuse", material.diffuseTextureId);
    m_shaderProgram.SetUniform("material.specular", material.specularTextureId);
    m_shaderProgram.SetUniform("material.normalMapping", material.normalMappingTextureId);
    m_shaderProgram.SetUniform("material.disp", material.dispTextureId);
    m_shaderProgram.SetUniform("material.shininess", material.shininess * 128); 
}
