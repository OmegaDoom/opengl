#ifndef TRIANGLE_DRAWABLE_HPP
#define TRIANGLE_DRAWABLE_HPP

#include "Drawable.hpp"
#include <memory>

class ShaderProgram;

class TriangleDrawable : public Drawable
{
public:
    TriangleDrawable();
    ~TriangleDrawable();
private:
    void render() const override;

    std::unique_ptr<ShaderProgram> m_shaderProgram;
    unsigned m_vao;
    unsigned m_vbo;
};

#endif //TRIANGLE_DRAWABLE
