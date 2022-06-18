#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "ModelNodeAnimation.hpp"
#include <vector>
#include <string>

class Animation
{
public:
    Animation(std::string name, float duration);
    float Duration() const;
    void AddModelNodeAnimation(const ModelNodeAnimation& modelNodeAnimation);
    const std::vector<ModelNodeAnimation>& ModelNodeAnimations() const; 
private:
    std::string m_name;
    float m_duration;
    std::vector<ModelNodeAnimation> m_modelNodeAnimations;
};

#endif //ANIMATION_HPP
