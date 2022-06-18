#include "AnimationRotationKey.hpp"

AnimationRotationKey::AnimationRotationKey(float time, glm::quat rotation)
    : AnimationKey(time)
    , m_rotation(rotation)
{
}

const glm::quat& AnimationRotationKey::Rotation() const
{
    return m_rotation;
}
