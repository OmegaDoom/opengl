#ifndef MODEL_DRAWABLE_HPP
#define MODEL_DRAWABLE_HPP

#include "Drawable.hpp"
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
class PointLight;

class ModelDrawable : public Drawable
{
public:
    ModelDrawable(Scene& scene, const ModelNode& rootNode, std::vector<Mesh>&& meshes
                  , const std::vector<Animation>& animations, const std::vector<Bone>& bones);

    ~ModelDrawable();
private:
    static const unsigned BONES_COUNT = 100;
    void RenderNodes(const ModelNode& node) const;
    void render() const override;
    unsigned m_VAO;
    const Texture2D *m_diffuse;
    const Texture2D *m_specular;
    ModelNode m_rootNode;
    std::vector<Mesh> m_meshes;
    ModelNode m_boneHierarchy;
    std::vector<Animation> m_animations;
    std::vector<std::unordered_map<ModelNode*, const ModelNodeAnimation*>> m_modelNodeToAnimation;
    std::unordered_map<const ModelNode*, unsigned> m_nodesToBoneIndexMapping;
    mutable glm::mat4 m_boneMatrices[BONES_COUNT];
};

#endif //MODEL_DRAWABLE_HPP
