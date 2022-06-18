#include "Bone.hpp"

Bone::Bone(std::string name, unsigned index, const glm::mat4& transform)
    : m_name(std::move(name))
    , m_transform(transform)
    , m_index(index)
{
}

const std::string& Bone::Name() const
{
    return m_name;
}

const glm::mat4& Bone::Transform() const
{
    return m_transform;
}

unsigned Bone::Index() const
{
    return m_index;
}

const std::vector<VertexWeight>& Bone::Weights() const
{
    return m_weights;
}

void Bone::AddWeight(const VertexWeight& weight)
{
    m_weights.push_back(weight);
}
