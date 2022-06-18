#ifndef BILLBOARD_QUAD_DRAWABLE_HPP
#define BILLBOARD_QUAD_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include <glm/glm.hpp>

class Scene;
class Texture2D;

class BillboardQuadDrawable : public Drawable, public Updateable
{
public:
    BillboardQuadDrawable(Scene& scene, float width, float length
                , const glm::mat4& model = glm::mat4{ 1.f });
    ~BillboardQuadDrawable();

private:
    void update() override;
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    unsigned m_index_count;

    glm::mat4 m_model;
    glm::mat4 m_billboard;
};

#endif //BILLBOARD_QUAD_DRAWABLE_HPP
