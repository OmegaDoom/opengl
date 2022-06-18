#ifndef SHADOW_MAP_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
#define SHADOW_MAP_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP

class ShaderProgram;
struct ShadowMapTextureMaterial;

class ShadowMapTextureMaterialShaderProgram
{
public:
    ShadowMapTextureMaterialShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const ShadowMapTextureMaterial& material);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //SHADOW_MAP_TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
