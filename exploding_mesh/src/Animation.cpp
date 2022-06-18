#include "Animation.hpp"

Animation::Animation(std::string name, float duration, unsigned framesPerSecond)
    : m_name(std::move(name))
    , m_duration(duration)
    , m_framesPerSecond(framesPerSecond)
{
}

void Animation::AddModelNodeAnimation(const ModelNodeAnimation& modelNodeAnimation)
{
    m_modelNodeAnimations.push_back(modelNodeAnimation);
}

const std::vector<ModelNodeAnimation>& Animation::ModelNodeAnimations() const
{
    return m_modelNodeAnimations;
}

float Animation::Duration() const
{
    return m_duration;
}

unsigned Animation::FramesPerSecond() const
{
    return m_framesPerSecond;
}
