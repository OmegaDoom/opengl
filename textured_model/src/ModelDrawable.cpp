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
#include <GL/glew.h>
#include <iostream>

namespace
{
    void UpdateTransforms(ModelNode& node, const std::vector<Mesh>& meshes)
    {
        node.UpdateWorldTransform();
        for(auto& child : node.Children())
        {
            UpdateTransforms(child, meshes);
        }
    }
}

ModelDrawable::ModelDrawable(Scene& scene, const ModelNode& rootNode, std::vector<Mesh>&& meshes)
    : Drawable(scene)
    , m_rootNode(rootNode)
    , m_meshes(std::move(meshes))
{
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

    glBindVertexArray(0);
}

ModelDrawable::~ModelDrawable()
{
    glDeleteVertexArrays(1, &m_VAO);
}

void ModelDrawable::update()
{
    UpdateTransforms(m_rootNode, m_meshes); 
}

void ModelDrawable::render() const
{
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

