#ifndef ENVIRONMENT_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
#define ENVIRONMENT_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP

class ShaderProgram;
class Texture2D;

class EnvironmentTextureMaterialShaderProgram
{
public:
    EnvironmentTextureMaterialShaderProgram(ShaderProgram& shaderProgram);
    void Apply(int texUnit);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //ENVIRONMENT_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
