#ifndef MODEL_NODE_HPP
#define MODEL_NODE_HPP

#include <vector>
#include <glm/glm.hpp>

class ModelNode
{
public:
    ModelNode(std::vector<unsigned> meshIndices, const glm::mat4& transform);
    ModelNode(const ModelNode& rhs);
    ModelNode& operator = (const ModelNode& rhs) = delete;
    void AddChild(const ModelNode& child);
    const std::vector<unsigned>& MeshIndices() const;
    void UpdateWorldTransform();
    const glm::mat4& WorldTransform() const;
    const std::vector<ModelNode>& Children() const;
    std::vector<ModelNode>& Children();
private:
    std::vector<unsigned> m_meshIndices;
    const ModelNode* m_parent;
    glm::mat4 m_transform;
    glm::mat4 m_worldTransform;
    std::vector<ModelNode> m_children;
};

#endif //MODEL_NODE_HPP
