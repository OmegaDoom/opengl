#ifndef QUAD_DRAWABLE_HPP
#define QUAD_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include <memory>
#include <chrono>

class ShaderProgram;
class Texture2D;

class QuadDrawable : public Drawable, public Updateable
{
public:
    QuadDrawable();
    ~QuadDrawable();
private:
    void update() override;
    void render() const override;

    std::unique_ptr<ShaderProgram> m_shaderProgram;
    std::unique_ptr<Texture2D> m_texture;
    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    std::chrono::steady_clock::time_point start_time;
};

#endif //QUAD_DRAWABLE
