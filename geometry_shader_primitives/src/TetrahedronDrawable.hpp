#ifndef TETRAHEDRON_DRAWABLE_HPP
#define TETRAHEDRON_DRAWABLE_HPP

#include "Drawable.hpp"

class Scene;

class TetrahedronDrawable : public Drawable
{
public:
    TetrahedronDrawable(Scene& scene);
    ~TetrahedronDrawable();
private:
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;
};

#endif // TETRAHEDRON_DRAWABLE_HPP
