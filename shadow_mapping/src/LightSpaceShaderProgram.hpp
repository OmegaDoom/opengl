#ifndef LIGHT_SPACE_SHADER_PROGRAM_HPP
#define LIGHT_SPACE_SHADER_PROGRAM_HPP

#include <glm/mat4x4.hpp>
class ShaderProgram;

class LightSpaceShaderProgram
{
public:
    LightSpaceShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const glm::mat4& lightSpace);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //LIGHT_SPACE_SHADER_PROGRAM_HPP
