#ifndef MODEL_NODE_HPP
#define MODEL_NODE_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>

class ModelNode
{
public:
    ModelNode(const std::string& name, std::vector<unsigned> meshIndices, const glm::mat4& transform);
    ModelNode(const std::string& name, const glm::mat4& transform);
    ModelNode(const ModelNode& rhs);
    ModelNode& operator = (const ModelNode& rhs);
    ModelNode(ModelNode&& rhs);
    ModelNode& operator = (ModelNode&& rhs);
    const std::string& Name() const;
    void AddChild(const ModelNode& child);
    const ModelNode* Parent() const;
    ModelNode* Find(const std::string& name);
    const ModelNode* Find(const std::string& name) const;
    const std::vector<unsigned>& MeshIndices() const;
    void UpdateWorldTransform();
    void UpdateWorldTransform(const glm::mat4& localTransform);
    void SetOffsetTransform(const glm::mat4& offsetTransform);
    const glm::mat4& Transform() const;
    const glm::mat4& WorldTransform() const;
    const glm::mat4& FinalTransform() const;
    const std::vector<ModelNode>& Children() const;
    std::vector<ModelNode>& Children();
    void UpdateTransformsToInitialPose();
private:
    std::string m_name;
    std::vector<unsigned> m_meshIndices;
    const ModelNode* m_parent;
    glm::mat4 m_transform;
    glm::mat4 m_offsetTransform;
    glm::mat4 m_worldTransform;
    glm::mat4 m_finalTransform;
    std::vector<ModelNode> m_children;
};

#endif //MODEL_NODE_HPP
