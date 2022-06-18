#ifndef VIEW_PROJECTION_DRAWABLE_HPP
#define VIEW_PROJECTION_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include <glm/glm.hpp>

class Scene;

class ViewProjectionDrawable : public Drawable, public Updateable
{
public:
    ViewProjectionDrawable(Scene& scene, const glm::mat4& view, glm::mat4 projection);
    ~ViewProjectionDrawable();
private:
    void update() override;
    void render() const override;

    unsigned int m_ubo;

    const glm::mat4& m_view;
    glm::mat4 m_projection;
};

#endif //VIEW_PROJECTION_DRAWABLE_HPP
