#include "SpotLightShaderProgram.hpp"

#include "SpotLight.hpp"
#include "ShaderProgram.hpp"

SpotLightShaderProgram::SpotLightShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void SpotLightShaderProgram::Apply(const SpotLight& light)
{
    m_shaderProgram.SetUniform("spotLight.position", light.position.x, light.position.y, light.position.z); 
    m_shaderProgram.SetUniform("spotLight.direction", light.direction.x, light.direction.y, light.direction.z); 
    m_shaderProgram.SetUniform("spotLight.cutOff", light.cutOff); 
    m_shaderProgram.SetUniform("spotLight.outerCutOff", light.outerCutOff); 

    m_shaderProgram.SetUniform("spotLight.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
    m_shaderProgram.SetUniform("spotLight.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
    m_shaderProgram.SetUniform("spotLight.specular",light.specular.x, light.specular.y, light.specular.z); 

    m_shaderProgram.SetUniform("spotLight.constant", light.constant); 
    m_shaderProgram.SetUniform("spotLight.linear", light.linear); 
    m_shaderProgram.SetUniform("spotLight.quadratic", light.quadratic); 
}
