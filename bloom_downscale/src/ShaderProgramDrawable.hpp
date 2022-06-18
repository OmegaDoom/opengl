#ifndef SHADER_PROGRAM_DRAWABLE_HPP
#define SHADER_PROGRAM_DRAWABLE_HPP

#include "Drawable.hpp"
#include "ShaderProgram.hpp"
#include <string>

class Scene;

class ShaderProgramDrawable : public Drawable
{
public:
    ShaderProgramDrawable(Scene& scene, const std::string& name);
private:
    void render() const override;

    ShaderProgram& m_shaderProgram;
};

#endif //SHADER_PROGRAM_DRAWABLE_HPP
