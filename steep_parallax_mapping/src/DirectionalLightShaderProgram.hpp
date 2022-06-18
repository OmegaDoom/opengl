#ifndef DIRECTIONAL_LIGHT_SHADER_PROGRAM_HPP
#define DIRECTIONAL_LIGHT_SHADER_PROGRAM_HPP

class ShaderProgram;
class DirectionalLight;

class DirectionalLightShaderProgram
{
public:
    DirectionalLightShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const DirectionalLight& material);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //DIRECTIONAL_LIGHT_SHADER_PROGRAM_HPP
