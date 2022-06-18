#ifndef QUAD_DRAWABLE_HPP
#define QUAD_DRAWABLE_HPP

#include "Drawable.hpp"
#include <memory>

class ShaderProgram;
class Texture2D;

class QuadDrawable : public Drawable
{
public:
    QuadDrawable();
    ~QuadDrawable();
private:
    void render() const override;

    std::unique_ptr<ShaderProgram> m_shaderProgram;
    std::unique_ptr<Texture2D> m_texture;
    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;
};

#endif //QUAD_DRAWABLE
