#ifndef NORMAL_MAP_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
#define NORMAL_MAP_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP

class ShaderProgram;
struct NormalMapTextureMaterial;

class NormalMapTextureMaterialShaderProgram
{
public:
    NormalMapTextureMaterialShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const NormalMapTextureMaterial& material);
private:
    ShaderProgram& m_shaderProgram;
};

#endif // NORMAL_MAP_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
