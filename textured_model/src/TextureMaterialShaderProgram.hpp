#ifndef TEXURE_MATERIAL_SHADER_PROGRAM_HPP
#define TEXURE_MATERIAL_SHADER_PROGRAM_HPP

class ShaderProgram;
struct TextureMaterial;
class TextureMaterialShaderProgram
{
public:
    TextureMaterialShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const TextureMaterial& material);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //TEXURE_MATERIAL_SHADER_PROGRAM_HPP
