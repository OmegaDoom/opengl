#ifndef QUAD_DRAWABLE_HPP
#define QUAD_DRAWABLE_HPP

#include "Drawable.hpp"
#include <glm/glm.hpp>

class Scene;
class Texture2D;

class QuadDrawable : public Drawable
{
public:
    QuadDrawable(Scene& scene, float width, float length, int segments);
    ~QuadDrawable();

    glm::vec3 Normal() const;
    glm::vec3 Tangent() const;

private:
    void render() const;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    unsigned m_index_count;

    glm::vec3 m_normal;
    glm::vec3 m_tangent;
};

#endif //QUAD_DRAWABLE_HPP
