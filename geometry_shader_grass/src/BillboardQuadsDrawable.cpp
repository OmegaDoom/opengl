#include "BillboardQuadsDrawable.hpp"

#include "Scene.hpp"
#include "BillboardVertex.hpp"
#include "ShaderProgram.hpp"

#include "NormalTransformShaderProgram.hpp"
#include "Texture2D.hpp"
#include "TextureMaterialShaderProgram.hpp"
#include "TextureManager.hpp"
#include "TextureMaterial.hpp"

#include <iostream>
#include <vector>
#include <GL/glew.h>

BillboardQuadsDrawable::BillboardQuadsDrawable(Scene& scene, float width, float length
                                                , const std::vector<glm::vec3>& instances)
    : Drawable(scene)
    , m_instances(instances)
{
    std::vector<billboard_vertex> vertices;
    
    for (auto& instance : instances)
    {
        vertices.push_back(billboard_vertex{ instance.x, instance.y, instance.z });
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(billboard_vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(billboard_vertex), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

BillboardQuadsDrawable::~BillboardQuadsDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void BillboardQuadsDrawable::render() const
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_instances.size());
}
