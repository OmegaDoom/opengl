#ifndef NORMAL_SHADER_PROGRAM_HPP
#define NORMAL_SHADER_PROGRAM_HPP

#include <glm/mat4x4.hpp>

class ShaderProgram;
class NormalShaderProgram
{
public:
    NormalShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const glm::mat4& viewModel);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //NORMAL_SHADER_PROGRAM_HPP
