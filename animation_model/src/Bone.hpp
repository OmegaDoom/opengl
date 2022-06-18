#ifndef BONE_HPP
#define BONE_HPP

#include "VertexWeight.hpp"
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Bone
{
public:
    Bone(std::string name, unsigned index, const std::vector<VertexWeight>& weights, const glm::mat4& transform);
    const std::string& Name() const;
    const glm::mat4& Transform() const;
    unsigned Index() const;
    const std::vector<VertexWeight>& Weights() const;
private:
    std::string m_name;
    glm::mat4 m_transform;
    unsigned m_index;
    std::vector<VertexWeight> m_weights;
};

#endif //BONE_HPP

