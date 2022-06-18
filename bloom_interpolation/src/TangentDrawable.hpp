#ifndef TANGENT_DRAWABLE_HPP
#define TANGENT_DRAWABLE_HPP

#include "Drawable.hpp"
#include <glm/glm.hpp>

class Scene;

class TangentDrawable : public Drawable
{
public:
    TangentDrawable(Scene& scene, const glm::vec3& tangent);
    ~TangentDrawable();
private:
    void render() const override;

    const glm::vec3 m_tangent;
};

#endif //TANGENT_DRAWABLE_HPP
