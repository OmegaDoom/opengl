#include "ModelDrawable.hpp"

#include "Scene.hpp"
#include "Texture2D.hpp"
#include "TextureManager.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "ModelShaderProgram.hpp"
#include "NormalShaderProgram.hpp"
#include "TextureMaterialShaderProgram.hpp"
#include "TextureMaterial.hpp"
#include "BoneShaderProgram.hpp"
#include "BonesShaderProgram.hpp"
#include <GL/glew.h>
#include <set>
#include <algorithm>
#include <iostream>

namespace
{
    void UpdateTransforms(float scalar, ModelNode& node
            , const std::unordered_map<ModelNode*, const ModelNodeAnimation*>& modelNodeToAnimation)
    {
        auto animation = modelNodeToAnimation.find(&node);
        if (animation != modelNodeToAnimation.cend())
        {
            const auto transform = animation->second->Transform(scalar);
            node.UpdateWorldTransform(transform);
        }        
        else
        {
            node.UpdateWorldTransform();
        }

        for(auto& child : node.Children())
        {
            UpdateTransforms(scalar, child, modelNodeToAnimation);
        }
    }

    void AssignOffsetTransforms(ModelNode& node, const std::vector<Mesh>& meshes)
    {
        for (auto& mesh : meshes)
        {
            for (auto& bone : mesh.Bones())
            {
                auto foundNode = node.Find(bone.Name());
                if (foundNode)
                {
                    foundNode->SetOffsetTransform(bone.Transform());
                }
            }
        }
    }

    void UpdateBoneTransformations(const ModelNode& node
                                  , const std::unordered_map<const ModelNode*, unsigned>& indexMapping
                                  , glm::mat4 bones[])
    {
        auto index = indexMapping.find(&node);
        if (index != indexMapping.cend())
        {
            bones[index->second] = node.FinalTransform();
        }

        for(auto& child : node.Children())
        {
            UpdateBoneTransformations(child, indexMapping, bones);
        }
    } 

    std::unordered_map<const ModelNode*, unsigned> GetBoneIndexMapping(ModelNode& node, const std::vector<Mesh>& meshes)
    {
        std::unordered_map<const ModelNode*, unsigned> mapping;

        for (auto& mesh : meshes)
        {
            for (auto& bone : mesh.Bones())
            {
                auto foundNode = node.Find(bone.Name());
                if (foundNode)
                {
                    mapping[foundNode] = bone.Index();
                }
            }
        }

        return mapping;
    }

    void GetBoneHierarchy(const ModelNode& modelHierarchy
            , const std::set<const ModelNode*>& boneModelNodes
            , ModelNode& parent)
    {
        auto boneNodeIter = boneModelNodes.find(&modelHierarchy);
        if (boneNodeIter == boneModelNodes.cend())
        {
            return;
        }

        auto& boneNode = modelHierarchy;
        auto modelNode = ModelNode(boneNode.Name(), boneNode.Transform());
        for (auto& child : modelHierarchy.Children())
        {
            GetBoneHierarchy(child, boneModelNodes, modelNode);
        }

        parent.AddChild(modelNode);
    }

    ModelNode GetBoneHierarchy(const ModelNode& modelHierarchy, const std::vector<Mesh>& meshes)
    {
        std::set<const ModelNode*> boneModelNodes;
        for (auto& mesh : meshes)
        {
            for (auto& bone : mesh.Bones())
            {
                boneModelNodes.insert(modelHierarchy.Find(bone.Name()));
            }
        }

        std::set<const ModelNode*> parentBoneModelNodes;
        for (auto bone : boneModelNodes)
        {
            auto parent = bone->Parent();
            while(parent)
            { 
                parentBoneModelNodes.insert(parent);
                parent = parent->Parent();
            }
        }

        for (auto bone : parentBoneModelNodes)
        {
            boneModelNodes.insert(bone);
        }

        auto transform = glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(0.f, 1.0f, 0.f));
        transform = glm::translate(transform, glm::vec3(0.f, -100.f, 0.f));
        auto rootModelNode = ModelNode("root", transform); 
        //auto rootModelNode = ModelNode("root", glm::mat4(1.f)); 
        GetBoneHierarchy(modelHierarchy, boneModelNodes, rootModelNode);
        return rootModelNode;
    }
}

ModelDrawable::ModelDrawable(Scene& scene, const ModelNode& rootNode, std::vector<Mesh>&& meshes
                            , const std::vector<Animation>& animations)
    : Drawable(scene)
    , m_rootNode(rootNode)
    , m_boneHierarchy(GetBoneHierarchy(rootNode, meshes))
    , m_meshes(std::move(meshes))
    , m_animations(animations)
    , m_nodesToBoneIndexMapping(GetBoneIndexMapping(m_boneHierarchy, m_meshes))
{
    for (auto& animation : m_animations)
    {
        std::unordered_map<ModelNode*, const ModelNodeAnimation*> modelNodeAnimationsMapping;
        for (auto& animationNode : animation.ModelNodeAnimations())
        {
            auto modelNode = m_boneHierarchy.Find(animationNode.Name());
            if (modelNode)
            {
                modelNodeAnimationsMapping.insert(std::make_pair(modelNode, &animationNode));
            }
        }

        m_modelNodeToAnimation.push_back(modelNodeAnimationsMapping);
    }

    m_boneHierarchy.UpdateTransformsToInitialPose(); 
    AssignOffsetTransforms(m_boneHierarchy, m_meshes);

    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, coords)); 
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, color)); 
    glVertexAttribBinding(1, 0);
    glEnableVertexAttribArray(1);

    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(vertex, tex_coords)); 
    glVertexAttribBinding(2, 0);
    glEnableVertexAttribArray(2);

    glVertexAttribFormat(3, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, normal_coords)); 
    glVertexAttribBinding(3, 0);
    glEnableVertexAttribArray(3);

    glVertexAttribIFormat(4, 4, GL_INT, offsetof(vertex, bones_info.ids)); 
    glVertexAttribBinding(4, 0);
    glEnableVertexAttribArray(4);

    glVertexAttribFormat(5, 4, GL_FLOAT, GL_FALSE, offsetof(vertex, bones_info.weights)); 
    glVertexAttribBinding(5, 0);
    glEnableVertexAttribArray(5);

    glBindVertexArray(0);
}

ModelDrawable::~ModelDrawable()
{
    glDeleteVertexArrays(1, &m_VAO);
}

void ModelDrawable::update()
{
    const static auto SPEED_FACTOR = 4;
    const static auto ANIMATION_INDEX = 0;

    const auto& animation = m_animations[ANIMATION_INDEX];
    const auto epoch = std::chrono::time_point<std::chrono::steady_clock>{};
    const auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - epoch);
    auto scalar = (SPEED_FACTOR * duration.count() % static_cast<int>(animation.Duration() * 1000));

    UpdateTransforms(scalar, m_boneHierarchy, m_modelNodeToAnimation[ANIMATION_INDEX]); 
}

void ModelDrawable::render() const
{
    BonesShaderProgram bonesShaderProgram(*scene().getSceneState().shaderProgram);
    UpdateBoneTransformations(m_boneHierarchy, m_nodesToBoneIndexMapping, m_boneMatrices);
    bonesShaderProgram.Apply(m_boneMatrices, BONES_COUNT);

    glBindVertexArray(m_VAO);
    RenderNodes(m_rootNode);
}

void ModelDrawable::RenderNodes(const ModelNode& node) const
{
    for(auto meshIndex : node.MeshIndices())
    {
        auto& shaderProgram = *scene().getSceneState().shaderProgram;
        ModelShaderProgram(shaderProgram).Apply(node.WorldTransform());

        auto viewModel = scene().getCamera().View() * node.WorldTransform();
        NormalShaderProgram(shaderProgram).Apply(viewModel);

        m_meshes[meshIndex].Render(shaderProgram);
    }

    for(auto& child : node.Children())
    {
        RenderNodes(child);
    }
}
