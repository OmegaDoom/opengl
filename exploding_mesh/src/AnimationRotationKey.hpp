#ifndef ANIMATION_ROTATION_KEY_HPP
#define ANIMATION_ROTATION_KEY_HPP

#include "AnimationKey.hpp"

#include <glm/gtc/quaternion.hpp>

class AnimationRotationKey : public AnimationKey
{
public:
    AnimationRotationKey(float time, glm::quat rotation);
    const glm::quat& Rotation() const;
private:
    glm::quat m_rotation;
};

#endif //ANIMATION_ROTATION_KEY_HPP
