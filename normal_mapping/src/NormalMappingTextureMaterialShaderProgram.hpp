#ifndef NORMAL_MAPPING_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
#define NORMAL_MAPPING_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP

class ShaderProgram;
struct NormalMappingTextureMaterial;

class NormalMappingTextureMaterialShaderProgram
{
public:
    NormalMappingTextureMaterialShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const NormalMappingTextureMaterial& material);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //NORMAL_MAPPING_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
