#ifndef POINT_LIGHT_SHADOW_MAP_TRANSFORMATIONS_DRAWABLE_HPP
#define POINT_LIGHT_SHADOW_MAP_TRANSFORMATIONS_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"

#include <array>
#include <glm/glm.hpp> 

class Scene;

class PointLightShadowMapTransformationsDrawable : public Drawable, public Updateable
{
public:
    PointLightShadowMapTransformationsDrawable(Scene& scene, const glm::vec3& lightPos);
private:
    virtual void update() override;
    virtual void render() const override;

    const glm::vec3& m_lightPos;
    std::array<glm::mat4, 6> m_shadowTransforms;
    const float m_far = 160.f;
};

#endif //POINT_LIGHT_SHADOW_MAP_TRANSFORMATIONS_DRAWABLE_HPP
