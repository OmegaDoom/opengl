#include "ModelDrawable.hpp"

#include "Scene.hpp"
#include "Texture2D.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "ModelTransformShaderProgram.hpp"
#include "NormalTransformShaderProgram.hpp"
#include <GL/glew.h>

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
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(tangent_vertex, coords));
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(tangent_vertex, color));
    glVertexAttribBinding(1, 0);
    glEnableVertexAttribArray(1);

    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(tangent_vertex, tex_coords));
    glVertexAttribBinding(2, 0);
    glEnableVertexAttribArray(2);

    glVertexAttribFormat(3, 3, GL_FLOAT, GL_FALSE, offsetof(tangent_vertex, normal_coords));
    glVertexAttribBinding(3, 0);
    glEnableVertexAttribArray(3);

    glVertexAttribFormat(4, 3, GL_FLOAT, GL_FALSE, offsetof(tangent_vertex, tangent));
    glVertexAttribBinding(4, 0);
    glEnableVertexAttribArray(4);

    glVertexAttribFormat(5, 3, GL_FLOAT, GL_FALSE, offsetof(tangent_vertex, bitangent));
    glVertexAttribBinding(5, 0);
    glEnableVertexAttribArray(5);

    glBindVertexArray(0);
}

void ModelDrawable::SetTransform(const glm::mat4& transform)
{
    m_rootNode.SetTransform(transform);
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
        ModelTransformShaderProgram(shaderProgram).Apply(node.WorldTransform());

        auto viewModel = scene().getCamera().View() * node.WorldTransform();
        NormalTransformShaderProgram(shaderProgram).Apply(viewModel);

        m_meshes[meshIndex].Render(shaderProgram);
    }

    for(auto& child : node.Children())
    {
        RenderNodes(child);
    }
}

