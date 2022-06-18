#ifndef TRIANGLE_SHADER_PROGRAM_HPP
#define TRIANGLE_SHADER_PROGRAM_HPP

#include <glm/glm.hpp>

class ShaderProgram;
class Texture2D;

class TriangleShaderProgram
{
public:
    TriangleShaderProgram(ShaderProgram& shaderProgram);
    void Apply(int screen_width, int screen_height, glm::vec2 point0, glm::vec2 point1, glm::vec2 point2, glm::vec2 tex0, glm::vec2 tex1, glm::vec2 tex2, const Texture2D& diffuse);

private:
    ShaderProgram& m_shaderProgram;
};

#endif // TRIANGLE_SHADER_PROGRAM_HPP
