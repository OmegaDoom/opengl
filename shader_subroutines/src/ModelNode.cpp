#include "ModelNode.hpp"


ModelNode::ModelNode(const std::string& name, std::vector<unsigned> meshIndices, const glm::mat4& transform)
    : m_name(name)
    , m_meshIndices(meshIndices)
    , m_parent(nullptr)
    , m_transform(transform)
    , m_worldTransform(transform)
{
}

ModelNode::ModelNode(const ModelNode& rhs)
    : m_name(rhs.m_name)
    , m_meshIndices(rhs.m_meshIndices)
    , m_parent(rhs.m_parent)
    , m_transform(rhs.m_transform)
    , m_worldTransform(rhs.m_worldTransform)
    , m_children(rhs.m_children)
{
    for(auto& child : m_children)
    {
        child.m_parent = this;
    }
}

ModelNode& ModelNode::operator = (const ModelNode& rhs)
{
    m_name = rhs.m_name;
    m_meshIndices = rhs.m_meshIndices;
    m_parent = rhs.m_parent;
    m_transform = rhs.m_transform;
    m_worldTransform = rhs.m_worldTransform;
    m_children = rhs.m_children;

    for(auto& child : m_children)
    {
        child.m_parent = this;
    }

    return *this;
}

const std::string& ModelNode::Name() const
{
    return m_name;
}

void ModelNode::AddChild(const ModelNode& child)
{
    m_children.push_back(child);
    m_children.back().m_parent = this;
}

const std::vector<unsigned>& ModelNode::MeshIndices() const
{
    return m_meshIndices;
}

void ModelNode::UpdateWorldTransform()
{
    if (m_parent)
    {
        m_worldTransform = m_parent->WorldTransform() * m_transform;
    }
}

const glm::mat4& ModelNode::WorldTransform() const
{
    return m_worldTransform;
}

const std::vector<ModelNode>& ModelNode::Children() const
{
    return m_children;
}

std::vector<ModelNode>& ModelNode::Children()
{
    return m_children;
}
