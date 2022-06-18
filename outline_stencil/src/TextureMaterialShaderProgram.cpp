#include "TextureMaterialShaderProgram.hpp"

#include "TextureMaterial.hpp"
#include "ShaderProgram.hpp"

TextureMaterialShaderProgram::TextureMaterialShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void TextureMaterialShaderProgram::Apply(const TextureMaterial& material)
{
    m_shaderProgram.SetUniform("material.diffuse", material.diffuseTextureId);
    m_shaderProgram.SetUniform("material.specular", material.specularTextureId);
    m_shaderProgram.SetUniform("material.shininess", material.shininess);
}
