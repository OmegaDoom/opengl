#ifndef LIGHTING_MAPS_SHADER_PROGRAM_HPP
#define LIGHTING_MAPS_SHADER_PROGRAM_HPP

class ShaderProgram;
struct LightingMapsMaterial;
class LightingMapsShaderProgram
{
public:
    LightingMapsShaderProgram(ShaderProgram& shaderProgram);
    void Apply(const LightingMapsMaterial& material);
private:
    ShaderProgram& m_shaderProgram;
};


#endif // LIGHTING_MAPS_SHADER_PROGRAM_HPP
