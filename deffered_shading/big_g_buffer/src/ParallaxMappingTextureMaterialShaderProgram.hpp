#ifndef PARALLAX_MAPPING_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
#define PARALLAX_MAPPING_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP

class ShaderProgram;
struct ParallaxMappingTextureMaterial;

class ParallaxMappingTextureMaterialShaderProgram
{
public:
    ParallaxMappingTextureMaterialShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const ParallaxMappingTextureMaterial& material);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //PARALLAX_MAPPING_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
