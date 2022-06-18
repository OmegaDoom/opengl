#ifndef PLANE_DRAWABLE_HPP
#define PLANE_DRAWABLE_HPP

#include "Drawable.hpp"
#include <glm/glm.hpp>

class Scene;
class Texture2D;

class PlaneDrawable : public Drawable
{
public:
    PlaneDrawable(Scene& scene, float width, float length, unsigned segments);
    ~PlaneDrawable();

private:
    void update();
    void render() const;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    unsigned m_index_count;

    glm::mat4 m_model;
};

#endif //PLANE_DRAWABLE_HPP
