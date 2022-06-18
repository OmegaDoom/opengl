#ifndef TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
#define TEXTURE_MATERIAL_SHADER_PROGRAM_HPP

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

#endif //TEXTURE_MATERIAL_SHADER_PROGRAM_HPP
