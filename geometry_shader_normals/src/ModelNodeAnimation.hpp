#ifndef MODEL_NODE_ANIMATION
#define MODEL_NODE_ANIMATION

#include "AnimationPositionKey.hpp" 
#include "AnimationRotationKey.hpp" 
#include "AnimationScaleKey.hpp" 

#include <string>
#include <vector>
#include <glm/glm.hpp>

class ModelNodeAnimation
{
public:
    ModelNodeAnimation(std::string name);
    const std::string& Name() const;
    void AddPositionKey(const AnimationPositionKey& key);
    void AddRotationKey(const AnimationRotationKey& key);
    void AddScaleKey(const AnimationScaleKey& key);
    glm::mat4 Transform(double time) const;
private:
    std::string m_name;

    std::vector<AnimationPositionKey> m_positionKeys;
    std::vector<AnimationRotationKey> m_rotationKeys;
    std::vector<AnimationScaleKey> m_scaleKeys;
};

#endif //MODEL_NODE_ANIMATIOn
