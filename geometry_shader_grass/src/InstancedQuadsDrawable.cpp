#include "InstancedQuadsDrawable.hpp"

#include "Scene.hpp"
#include "Vertex.hpp"
#include "ShaderProgram.hpp"
#include "ModelTransformShaderProgram.hpp"
#include "NormalTransformShaderProgram.hpp"
#include "Texture2D.hpp"
#include "TextureMaterialShaderProgram.hpp"
#include "TextureManager.hpp"
#include "TextureMaterial.hpp"

#include <iostream>
#include <vector>
#include <GL/glew.h>

namespace
{
    void genIndices(std::vector<unsigned>& indices, int x0, int x1, int y0, int y1, int width, int cacheSize)
    {
        if (x1 - x0 + 1 < cacheSize)
        {
            if (2 * (x1 - x0) + 1 > cacheSize)
            {
                for (int x = x0; x < x1; x++)
                {
                    indices.push_back(x + 0);
                    indices.push_back(x + 0);
                    indices.push_back(x + 1);
                }
            }

            for (int y = y0; y < y1; y++)
            {
                for (int x = x0; x < x1; x++)
                {
                    indices.push_back((width + 1) * (y + 0) + (x + 0));
                    indices.push_back((width + 1) * (y + 1) + (x + 0));
                    indices.push_back((width + 1) * (y + 0) + (x + 1));

                    indices.push_back((width + 1) * (y + 0) + (x + 1));
                    indices.push_back((width + 1) * (y + 1) + (x + 0));
                    indices.push_back((width + 1) * (y + 1) + (x + 1));
                }
            }
        }
        else
        {
            int xm = x0 + cacheSize - 2;
            genIndices(indices, x0, xm, y0, y1, width, cacheSize);
            genIndices(indices, xm, x1, y0, y1, width, cacheSize);
        }
    }
}

InstancedQuadsDrawable::InstancedQuadsDrawable(Scene& scene, float width, float length
                                                , const std::vector<glm::mat4>& instances)
    : Drawable(scene)
    , m_instances(instances)
{
    std::vector<vertex> vertices;
    const int segments = 1;
    for (int y = 0; y < segments + 1; ++y)
    {
        for (int x = 0; x < segments + 1; ++x)
        {
            vertices.emplace_back(vertex{{ -width * 0.5f + x * width / segments, 0.f
                                          , -length * 0.5f + y * length / segments }
                                          , { 0.f, 0.f, 0.f }
                                          , { x * 1.f / static_cast<float>(segments)
                                          , y * 1.f / static_cast<float>(segments) }
                                          , { 0.f, 1.f, 0.f}});
        }
    }

    std::vector<unsigned> indices;

    genIndices(indices, 0, segments, 0, segments, segments, 8); 

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_instancedVbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, coords)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, color)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex)
                            , reinterpret_cast<void*>(offsetof(vertex, tex_coords))); 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex)
                            , reinterpret_cast<void*>(offsetof(vertex, normal_coords)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, m_instancedVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instances.size(), 0, GL_STREAM_DRAW);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 0);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(4 * sizeof(float)));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(8 * sizeof(float)));
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(12 * sizeof(float)));

    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glEnableVertexAttribArray(7);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);

    m_indexCount = indices.size();
}

InstancedQuadsDrawable::~InstancedQuadsDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_instancedVbo);
    glDeleteBuffers(1, &m_ebo);
}

void InstancedQuadsDrawable::update()
{
    static bool called = false;
    if (called)
        return;

    called = true;
    glBindBuffer(GL_ARRAY_BUFFER, m_instancedVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_instances.size(), 0, GL_STREAM_DRAW);

    auto view = scene().getCamera().View();
    view[3][0] = 0.f;
    view[3][1] = 0.f;
    view[3][2] = 0.f;

    auto inversedView = inverse(view);
    glBindBuffer(GL_ARRAY_BUFFER, m_instancedVbo);
    glm::mat4 *p = reinterpret_cast<glm::mat4*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    
    for (int i = 0; i < m_instances.size(); ++i)
    {
        auto& mat = m_instances[i];
        auto billboard = inversedView;
        billboard[3][0] = mat[3][0]; 
        billboard[3][1] = mat[3][1]; 
        billboard[3][2] = mat[3][2]; 

        auto model = mat;
        model[3][0] = 0.f;
        model[3][1] = 0.f;
        model[3][2] = 0.f;

        p[i] = billboard * model;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void InstancedQuadsDrawable::render() const
{
    glBindVertexArray(m_vao);
    glDrawElementsInstanced(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0, m_instances.size());
}
