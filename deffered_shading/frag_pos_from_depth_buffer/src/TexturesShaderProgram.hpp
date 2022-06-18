#ifndef TEXTURES_SHADER_PROGRAM_HPP
#define TEXTURES_SHADER_PROGRAM_HPP

class ShaderProgram;
struct Textures;

class TexturesShaderProgram
{
public:
    TexturesShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const Textures& textures);
private:
    ShaderProgram& m_shaderProgram;
};

#endif //TEXTURES_SHADER_PROGRAM_HPP
