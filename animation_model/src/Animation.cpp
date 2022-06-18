#include "Animation.hpp"

Animation::Animation(std::string name, float duration)
    : m_name(std::move(name))
    , m_duration(duration)
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

