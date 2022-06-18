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
    m_shaderParams.push_back("pointLights[0].ambient");
    m_shaderParams.push_back("pointLights[0].diffuse");
    m_shaderParams.push_back("pointLights[0].specular");
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

    m_shaderProgram.SetUniform(m_shaderParams[1].c_str(), light.ambient.x, light.ambient.y, light.ambient.z);
    m_shaderProgram.SetUniform(m_shaderParams[2].c_str(), light.diffuse.x, light.diffuse.y, light.diffuse.z);
    m_shaderProgram.SetUniform(m_shaderParams[3].c_str(), light.specular.x, light.specular.y, light.specular.z); 

    m_shaderProgram.SetUniform(m_shaderParams[4].c_str(), light.constant); 
    m_shaderProgram.SetUniform(m_shaderParams[5].c_str(), light.linear); 
    m_shaderProgram.SetUniform(m_shaderParams[6].c_str(), light.quadratic); 
}
