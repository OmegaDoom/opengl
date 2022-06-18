#include "BonesShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

BonesShaderProgram::BonesShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void BonesShaderProgram::Apply(const glm::mat4* boneTransform, int num)
{
    m_shaderProgram.SetUniformMatrices4("bones", num, reinterpret_cast<const float*>(boneTransform)); 
}
