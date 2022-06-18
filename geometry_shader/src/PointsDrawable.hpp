#ifndef POINTS_DRAWABLE_HPP
#define POINTS_DRAWABLE_HPP

#include "Drawable.hpp"

class Scene;

class PointsDrawable : public Drawable
{
public:
    PointsDrawable(Scene& scene);
    ~PointsDrawable();
private:
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;
};

#endif //POINTS_DRAWABLE_HPP
