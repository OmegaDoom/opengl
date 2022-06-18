#ifndef MODEL_SHADER_PROGRAM_HPP
#define MODEL_SHADER_PROGRAM_HPP

#include <glm/mat4x4.hpp>
class ShaderProgram;

class ModelShaderProgram
{
public:
    ModelShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const glm::mat4& model);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //MODEL_SHADER_PROGRAM_HPP
