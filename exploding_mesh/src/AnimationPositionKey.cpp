#include "AnimationPositionKey.hpp"

AnimationPositionKey::AnimationPositionKey(float time, glm::vec3 position)
    : AnimationKey(time)
    , m_position(position)
{
}

const glm::vec3& AnimationPositionKey::Position() const
{
    return m_position;
}
