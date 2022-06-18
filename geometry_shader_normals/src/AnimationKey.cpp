#include "AnimationKey.hpp"

AnimationKey::AnimationKey(float time)
    : m_time(time)
{
}

float AnimationKey::Time() const
{
    return m_time;
}
