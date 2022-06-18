#ifndef PBR_MATERIAL_SHADER_PROGRAM_HPP
#define PBR_MATERIAL_SHADER_PROGRAM_HPP

class ShaderProgram;
struct PbrMaterial;

class PbrMaterialShaderProgram
{
public:
    PbrMaterialShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const PbrMaterial& material);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //PBR_MATERIAL_SHADER_PROGRAM_HPP
