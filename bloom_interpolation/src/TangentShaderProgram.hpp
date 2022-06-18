#ifndef TANGENT_SHADER_PROGRAM_HPP
#define TANGENT_SHADER_PROGRAM_HPP

#include <glm/glm.hpp>
class ShaderProgram;

class TangentShaderProgram
{
public:
    TangentShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const glm::vec3& tangent);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //TANGENT_SHADER_PROGRAM_HPP
