#ifndef BILLBOARD_MODE__NORMAL_TRANSFORM_DRAWABLE_HPP
#define BILLBOARD_MODE__NORMAL_TRANSFORM_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"

#include <glm/glm.hpp> 

class Scene;

class BillboardModelNormalTransformDrawable : public Drawable, public Updateable
{
public:
    BillboardModelNormalTransformDrawable(Scene& scene, const glm::mat4& model);
    ~BillboardModelNormalTransformDrawable();
private:
    void update() override;
    void render() const override;

    glm::mat4 m_model;
    glm::mat4 m_billboard;
};

#endif // BILLBOARD_MODE__NORMAL_TRANSFORM_DRAWABLE_HPP