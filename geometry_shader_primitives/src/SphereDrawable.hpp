#ifndef SPHERE_DRAWABLE_HPP
#define SPHERE_DRAWABLE_HPP

#include "Drawable.hpp"

class Scene;

class SphereDrawable : public Drawable
{
public:
    SphereDrawable(Scene& scene);
    ~SphereDrawable();
private:
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;
};

#endif // SPHERE_DRAWABLE_HPP
