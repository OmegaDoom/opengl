#ifndef BONES_SHADER_PROGRAM_HPP
#define BONES_SHADER_PROGRAM_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>

class ShaderProgram;
class Bone;

class BonesShaderProgram
{
public:
    BonesShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const glm::mat4* boneTransform, int num);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //BONES_SHADER_PROGRAM_HPP
