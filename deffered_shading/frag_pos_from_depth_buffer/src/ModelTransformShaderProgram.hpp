#ifndef MODEL_TRANSFORM_SHADER_PROGRAM_HPP
#define MODEL_TRANSFORM_SHADER_PROGRAM_HPP

#include <glm/mat4x4.hpp>
class ShaderProgram;

class ModelTransformShaderProgram
{
public:
    ModelTransformShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const glm::mat4& model);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //MODEL_TRANSFORM_SHADER_PROGRAM_HPP
