#ifndef MATERIAL_SHADER_PROGRAM_HPP
#define MATERIAL_SHADER_PROGRAM_HPP

class ShaderProgram;
struct Material;

class MaterialShaderProgram
{
public:
    MaterialShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const Material& material);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //MATERIAL_SHADER_PROGRAM_HPP
