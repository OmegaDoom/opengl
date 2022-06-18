#ifndef CONVOLUTION_SHADER_PROGRAM_HPP
#define CONVOLUTION_SHADER_PROGRAM_HPP

class ShaderProgram;
class Texture2D;

class ConvolutionShaderProgram
{
public:
    ConvolutionShaderProgram(ShaderProgram& shaderProgram);
    void Apply(int width, int height, const Texture2D& diffuse);

private:
    ShaderProgram& m_shaderProgram;
};

#endif //CONVOLUTION_SHADER_PROGRAM_HPP
