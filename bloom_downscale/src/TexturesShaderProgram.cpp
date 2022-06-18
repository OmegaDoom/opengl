#include "TexturesShaderProgram.hpp"

#include "Textures.hpp"
#include "ShaderProgram.hpp"

TexturesShaderProgram::TexturesShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void TexturesShaderProgram::Apply(const Textures& textures)
{
    m_shaderProgram.SetUniform("textures.count", textures.count);
    m_shaderProgram.SetUniform("textures.textures[0]", textures.textures[0]);
    m_shaderProgram.SetUniform("textures.textures[1]", textures.textures[1]);
    m_shaderProgram.SetUniform("textures.textures[2]", textures.textures[2]);
    m_shaderProgram.SetUniform("textures.textures[3]", textures.textures[3]);
    m_shaderProgram.SetUniform("textures.textures[4]", textures.textures[4]);
    m_shaderProgram.SetUniform("textures.textures[5]", textures.textures[5]);
    m_shaderProgram.SetUniform("textures.textures[6]", textures.textures[6]);
    m_shaderProgram.SetUniform("textures.textures[7]", textures.textures[7]);
}
