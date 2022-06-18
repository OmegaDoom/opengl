#ifndef BONE_SHADER_PROGRAM_HPP
#define BONE_SHADER_PROGRAM_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>

class ShaderProgram;
class Bone;

class BoneShaderProgram
{
public:
    BoneShaderProgram(ShaderProgram& shaderProgram);
    void Apply(unsigned index, const glm::mat4& boneTransform);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //BONE_SHADER_PROGRAM_HPP
