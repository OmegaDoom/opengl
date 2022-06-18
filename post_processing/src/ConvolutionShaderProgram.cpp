#include "ConvolutionShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"

ConvolutionShaderProgram::ConvolutionShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void ConvolutionShaderProgram::Apply(int width, int height, const Texture2D& diffuse)
{
    m_shaderProgram.SetUniform("width", width);
    m_shaderProgram.SetUniform("height", height);
    m_shaderProgram.SetUniform("diffuse", diffuse());
}
