#include "DirectionalLightShaderProgram.hpp"

#include "DirectionalLight.hpp"
#include "ShaderProgram.hpp"

DirectionalLightShaderProgram::DirectionalLightShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void DirectionalLightShaderProgram::Apply(const DirectionalLight& light)
{
    m_shaderProgram.SetUniform("light.direction", light.direction.x, light.direction.y, light.direction.z); 
    m_shaderProgram.SetUniform("light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
    m_shaderProgram.SetUniform("light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
    m_shaderProgram.SetUniform("light.specular",light.specular.x, light.specular.y, light.specular.z); 
}
