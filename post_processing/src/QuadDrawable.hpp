#ifndef QUAD_DRAWABLE_HPP
#define QUAD_DRAWABLE_HPP

#include "Drawable.hpp"
#include <glm/glm.hpp>

class Scene;
class Texture2D;

class QuadDrawable : public Drawable
{
public:
    QuadDrawable(Scene& scene, float width, float length
                , const glm::mat4& model = glm::mat4{ 1.f });
    ~QuadDrawable();

private:
    void update();
    void render() const;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    unsigned m_index_count;

    glm::mat4 m_model;
};

#endif //QUAD_DRAWABLE_HPP
