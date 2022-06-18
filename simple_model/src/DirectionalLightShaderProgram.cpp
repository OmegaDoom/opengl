#include "DirectionalLightShaderProgram.hpp"

#include "DirectionalLight.hpp"
#include "ShaderProgram.hpp"

DirectionalLightShaderProgram::DirectionalLightShaderProgram(ShaderProgram& shaderProgram)
    : m_shaderProgram{ shaderProgram }
{
}

void DirectionalLightShaderProgram::Apply(const DirectionalLight& directionalLight)
{
    m_shaderProgram.SetUniform("directionalLight.direction"
                                , directionalLight.direction.x, directionalLight.direction.y, directionalLight.direction.z); 
    m_shaderProgram.SetUniform("directionalLight.ambient"
                                , directionalLight.ambient.x, directionalLight.ambient.y, directionalLight.ambient.z);
    m_shaderProgram.SetUniform("directionalLight.diffuse"
                                , directionalLight.diffuse.x, directionalLight.diffuse.y, directionalLight.diffuse.z);
    m_shaderProgram.SetUniform("directionalLight.specular"
                                ,directionalLight.specular.x, directionalLight.specular.y, directionalLight.specular.z); 
}
