#ifndef POINT_LIGHT_SHADER_PROGRAM_HPP
#define POINT_LIGHT_SHADER_PROGRAM_HPP

#include <vector>
#include <string>

class ShaderProgram;
struct PointLight;

class PointLightShaderProgram
{
public:
    PointLightShaderProgram(ShaderProgram& shaderProgram);
    void Apply(int lightIndex, const PointLight& light);
private:
    ShaderProgram& m_shaderProgram;
    std::vector<std::string> m_shaderParams;
};

#endif //POINT_LIGHT_SHADER_PROGRAM_HPP
