#ifndef SHADOW_MATRICES_SHADER_PROGRAM_HPP
#define SHADOW_MATRICES_SHADER_PROGRAM_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>

class ShaderProgram;

class ShadowMatricesShaderProgram
{
public:
    ShadowMatricesShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const glm::mat4* matrices, int num);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //SHADOW_MATRICES_SHADER_PROGRAM_HPP
