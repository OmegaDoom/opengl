#ifndef DIFFUSE_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
#define DIFFUSE_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP

class ShaderProgram;
class Texture2D;

class DiffuseTextureMaterialShaderProgram
{
public:
    DiffuseTextureMaterialShaderProgram(ShaderProgram& shaderProgram);
    void Apply(int texUnit);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //DIFFUSE_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
