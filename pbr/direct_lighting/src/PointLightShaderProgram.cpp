#include "PointLightShaderProgram.hpp"

#include "PointLight.hpp"
#include "ShaderProgram.hpp"

namespace
{
    const int LIGHT_INDEX_POSITION = 12;
}

PointLightShaderProgram::PointLightShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
    m_shaderParams.push_back("pointLights[0].position");
    m_shaderParams.push_back("pointLights[0].color");
    m_shaderParams.push_back("pointLights[0].constant");
    m_shaderParams.push_back("pointLights[0].linear");
    m_shaderParams.push_back("pointLights[0].quadratic");
}

void PointLightShaderProgram::Apply(const int lightIndex, const PointLight& light)
{
    for (int i = 0; i < m_shaderParams.size(); ++i)
    {
        m_shaderParams[i][LIGHT_INDEX_POSITION] = '0' + lightIndex;
    }

    m_shaderProgram.SetUniform(m_shaderParams[0].c_str(), light.position.x, light.position.y, light.position.z); 
    m_shaderProgram.SetUniform(m_shaderParams[1].c_str(), light.color.x, light.color.y, light.color.z);
    m_shaderProgram.SetUniform(m_shaderParams[2].c_str(), light.constant);
    m_shaderProgram.SetUniform(m_shaderParams[3].c_str(), light.linear);
    m_shaderProgram.SetUniform(m_shaderParams[4].c_str(), light.quadratic);
}
