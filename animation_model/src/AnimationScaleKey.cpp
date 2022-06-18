#include "AnimationScaleKey.hpp"

AnimationScaleKey::AnimationScaleKey(float time, glm::vec3 scale)
    : AnimationKey(time)
    , m_scale(scale)
{
}

const glm::vec3& AnimationScaleKey::Scale() const
{
    return m_scale;
}
