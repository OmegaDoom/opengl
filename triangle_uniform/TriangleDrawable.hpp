#ifndef TRIANGLE_DRAWABLE_HPP
#define TRIANGLE_DRAWABLE_HPP

#include "Updateable.hpp"
#include "Drawable.hpp"
#include <memory>

class ShaderProgram;

class TriangleDrawable : public Drawable, public Updateable
{
public:
    TriangleDrawable();
    ~TriangleDrawable() override;
private:
    void update() override;
    void render() const override;

    std::unique_ptr<ShaderProgram> m_shaderProgram;
    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;
    float m_scalar;
};

#endif //TRIANGLE_DRAWABLE
