#include "BoneShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace
{
    const int BONE_INDEX_POSITION = 6;
    std::vector<std::string> shaderParams{ "bones[0]", "bones[00]", "bones[000]" };
}

BoneShaderProgram::BoneShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void BoneShaderProgram::Apply(const unsigned index, const glm::mat4& boneTransform)
{
    auto str = std::to_string(index);
    const unsigned digits = index < 10 ? 1 : index < 100 ? 2 : 3;
    
    for (int i = 0; i < digits; ++i)
    {
        shaderParams[digits - 1][BONE_INDEX_POSITION + i] = str[i];     
    }

    m_shaderProgram.SetUniformMatrix4(shaderParams[digits - 1].c_str(), glm::value_ptr(boneTransform)); 
}
