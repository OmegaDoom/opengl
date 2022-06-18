#ifndef SPHERE_DRAWABLE_HPP
#define SPHERE_DRAWABLE_HPP

#include "Drawable.hpp"
#include <glm/glm.hpp>

class Sphere;

class SphereDrawable : public Drawable
{
public:
    SphereDrawable(Scene& scene, float radius, const glm::mat4& model = glm::mat4{1.f });
    ~SphereDrawable();
private:
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    unsigned m_index_count;

    glm::mat4 m_model;
};

#endif //SPHERE_DRAWABLE_HPP
