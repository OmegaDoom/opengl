#ifndef SPOT_LIGHT_SHADER_PROGRAM_HPP
#define SPOT_LIGHT_SHADER_PROGRAM_HPP

class ShaderProgram;
class SpotLight;

class SpotLightShaderProgram
{
public:
    SpotLightShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const SpotLight& material);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //SPOT_LIGHT_SHADER_PROGRAM_HPP
