#include "TriangleShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include "Texture2D.hpp"

TriangleShaderProgram::TriangleShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void TriangleShaderProgram::Apply(int screen_width, int screen_height, glm::vec2 point0, glm::vec2 point1, glm::vec2 point2, glm::vec2 tex0, glm::vec2 tex1, glm::vec2 tex2, const Texture2D& diffuse)
{
    m_shaderProgram.SetUniform("screen_width", screen_width);
    m_shaderProgram.SetUniform("screen_height", screen_height);
    m_shaderProgram.SetUniform("diffuse", diffuse());
    m_shaderProgram.SetUniform("point0", point0.x, point0.y);
    m_shaderProgram.SetUniform("point1", point1.x, point1.y);
    m_shaderProgram.SetUniform("point2", point2.x, point2.y);
    m_shaderProgram.SetUniform("tex0", tex0.x, tex0.y);
    m_shaderProgram.SetUniform("tex1", tex1.x, tex1.y);
    m_shaderProgram.SetUniform("tex2", tex2.x, tex2.y);
}
