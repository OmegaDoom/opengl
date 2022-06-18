#ifndef SPHERE_DRAWABLE_HPP
#define SPHERE_DRAWABLE_HPP

#include "Drawable.hpp"

class Sphere;

class SphereDrawable : public Drawable
{
public:
    SphereDrawable(Scene& scene, float radius);
    ~SphereDrawable();
private:
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    unsigned m_index_count;
};

#endif //SPHERE_DRAWABLE_HPP
