#ifndef MODEL_DRAWABLE_HPP
#define MODEL_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include "Bone.hpp"
#include "ModelNode.hpp"
#include "ModelNodeAnimation.hpp"
#include "Mesh.hpp"
#include "Animation.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

class Scene;
class ModelNode;
class Texture2D;
class ModelNodeAnimation;

class ModelDrawable : public Drawable, public Updateable
{
public:
    ModelDrawable(Scene& scene, const ModelNode& rootNode, std::vector<Mesh>&& meshes
                  , const std::vector<Animation>& animations);
    ~ModelDrawable();
private:
    static const unsigned BONES_COUNT = 100;
    void RenderNodes(const ModelNode& node) const;
    void update() override; 
    void render() const override; 
    unsigned m_VAO;
    ModelNode m_rootNode;
    ModelNode m_boneHierarchy;
    std::vector<Mesh> m_meshes;
    std::vector<Animation> m_animations;
    std::vector<std::unordered_map<ModelNode*, const ModelNodeAnimation*>> m_modelNodeToAnimation;
    std::unordered_map<const ModelNode*, unsigned> m_nodesToBoneIndexMapping;
    mutable glm::mat4 m_boneMatrices[BONES_COUNT];
};

#endif //MODEL_DRAWABLE_HPP
