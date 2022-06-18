#ifndef ANIMATION_KEY_HPP
#define ANIMATION_KEY_HPP

class AnimationKey
{
public:
    AnimationKey(float time);
    float Time() const;
private:
    float m_time;
};

#endif //ANIMATION_KEY_HPP
