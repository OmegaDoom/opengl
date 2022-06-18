#ifndef NORMAL_TRANSFORM_SHADER_PROGRAM_HPP
#define NORMAL_TRANSFORM_SHADER_PROGRAM_HPP

#include <glm/mat4x4.hpp>
class ShaderProgram;

class NormalTransformShaderProgram
{
public:
    NormalTransformShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const glm::mat4& viewModel);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //NORMAL_TRANSFORM_SHADER_PROGRAM_HPP
