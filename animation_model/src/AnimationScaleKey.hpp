#ifndef ANIMATION_SCALE_KEY_HPP
#define ANIMATION_SCALE_KEY_HPP

#include "AnimationKey.hpp"

#include <glm/glm.hpp>

class AnimationScaleKey : public AnimationKey
{
public:
    AnimationScaleKey(float time, glm::vec3 scale);
    const glm::vec3& Scale() const;
private:
    glm::vec3 m_scale;
};

#endif //ANIMATION_SCALE_KEY_HPP
