#ifndef MODEL_DRAWABLE_HPP
#define MODEL_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include "ModelNode.hpp"
#include "Mesh.hpp"
#include <vector>
#include <memory>

class Scene;
class ModelNode;
class Texture2D;

class ModelDrawable : public Drawable, public Updateable
{
public:
    ModelDrawable(Scene& scene, const ModelNode& rootNode, std::vector<Mesh>&& meshes);
    void SetTransform(const glm::mat4& transform);
    ~ModelDrawable();
private:
    void RenderNodes(const ModelNode& node) const;
    void update() override; 
    void render() const override; 
    unsigned m_VAO;
    ModelNode m_rootNode;
    std::vector<Mesh> m_meshes;
};

#endif //MODEL_DRAWABLE_HPP