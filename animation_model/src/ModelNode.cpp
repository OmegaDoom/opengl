#include "ModelNode.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

ModelNode::ModelNode(const std::string& name, std::vector<unsigned> meshIndices, const glm::mat4& transform)
    : m_name(name)
    , m_meshIndices(meshIndices)
    , m_parent(nullptr)
    , m_transform(transform)
    , m_offsetTransform(1.f)
    , m_worldTransform(transform)
    , m_finalTransform(transform)
{
}

ModelNode::ModelNode(const std::string& name, const glm::mat4& transform)
    : m_name(name)
    , m_parent(nullptr)
    , m_transform(transform)
    , m_offsetTransform(1.f)
    , m_worldTransform(transform)
    , m_finalTransform(transform)
{
}

ModelNode::ModelNode(const ModelNode& rhs)
    : m_name(rhs.m_name)
    , m_meshIndices(rhs.m_meshIndices)
    , m_parent(rhs.m_parent)
    , m_transform(rhs.m_transform)
    , m_offsetTransform(rhs.m_offsetTransform)
    , m_worldTransform(rhs.m_worldTransform)
    , m_finalTransform(rhs.m_finalTransform)
    , m_children(rhs.m_children)
{
    for(auto& child : m_children)
    {
        child.m_parent = this;
    }
}

ModelNode::ModelNode(ModelNode&& rhs)
    : m_name(std::move(rhs.m_name))
    , m_meshIndices(std::move(rhs.m_meshIndices))
    , m_parent(rhs.m_parent)
    , m_transform(std::move(rhs.m_transform))
    , m_offsetTransform(std::move(rhs.m_offsetTransform))
    , m_worldTransform(std::move(rhs.m_worldTransform))
    , m_finalTransform(std::move(rhs.m_finalTransform))
    , m_children(std::move(rhs.m_children))
{
    for(auto& child : m_children)
    {
        child.m_parent = this;
    }

    rhs.m_parent = nullptr;
}

ModelNode& ModelNode::operator = (const ModelNode& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    m_name = rhs.m_name;
    m_meshIndices = rhs.m_meshIndices;
    m_parent = rhs.m_parent;
    m_transform = rhs.m_transform;
    m_offsetTransform = rhs.m_offsetTransform;
    m_worldTransform = rhs.m_worldTransform;
    m_finalTransform = rhs.m_finalTransform;
    m_children = rhs.m_children;

    for(auto& child : m_children)
    {
        child.m_parent = this;
    }

    return *this;
}

ModelNode& ModelNode::operator = (ModelNode&& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    m_name = std::move(rhs.m_name);
    m_meshIndices = std::move(rhs.m_meshIndices);
    m_parent = std::move(rhs.m_parent);
    m_transform = rhs.m_transform;
    m_offsetTransform = rhs.m_offsetTransform;
    m_worldTransform = std::move(rhs.m_worldTransform);
    m_offsetTransform = std::move(rhs.m_offsetTransform);
    m_children = std::move(rhs.m_children);

    for(auto& child : m_children)
    {
        child.m_parent = this;
    }

    rhs.m_parent = nullptr;

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

const ModelNode* ModelNode::Parent() const
{
    return m_parent;
}

ModelNode* ModelNode::Find(const std::string& name)
{
    if (name == m_name)
    {
        return this;
    }

    for (auto& child : m_children)
    {
        auto result = child.Find(name);
        if (result)
        {
            return result;
        }
    }

    return nullptr;
}

const ModelNode* ModelNode::Find(const std::string& name) const
{
    if (name == m_name)
    {
        return this;
    }

    for (auto& child : m_children)
    {
        auto result = child.Find(name);
        if (result)
        {
            return result;
        }
    }

    return nullptr;
}

const std::vector<unsigned>& ModelNode::MeshIndices() const
{
    return m_meshIndices;
}

void ModelNode::UpdateWorldTransform()
{
    m_worldTransform = m_parent ? m_parent->m_worldTransform * m_transform : m_transform;
    m_finalTransform = m_worldTransform * m_offsetTransform;
}

void ModelNode::UpdateWorldTransform(const glm::mat4& localTransform)
{
    m_worldTransform = m_parent ? m_parent->m_worldTransform * localTransform : localTransform;
    m_finalTransform = m_worldTransform * m_offsetTransform;
}

void ModelNode::UpdateTransformsToInitialPose()
{
    UpdateWorldTransform();
    for(auto& child : Children())
    {
        child.UpdateTransformsToInitialPose();
    }
}

void ModelNode::SetOffsetTransform(const glm::mat4& offsetTransform)
{
    m_offsetTransform = offsetTransform; 
    //m_offsetTransform = glm::inverse(m_worldTransform);
}

const glm::mat4& ModelNode::Transform() const
{
    return m_transform;
}

const glm::mat4& ModelNode::FinalTransform() const
{
    return m_finalTransform;
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
