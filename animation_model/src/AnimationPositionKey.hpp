#include "AnimationKey.hpp"

#include <glm/glm.hpp>

class AnimationPositionKey : public AnimationKey
{
public:
    AnimationPositionKey(float time, glm::vec3 position);
    const glm::vec3& Position() const;
private:
    glm::vec3 m_position;
};
