#include "QuadDrawable.hpp"

#include "Scene.hpp"
#include "Vertex.hpp"
#include "ShaderProgram.hpp"
#include "ModelShaderProgram.hpp"
#include "NormalShaderProgram.hpp"
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

QuadDrawable::QuadDrawable(Scene& scene, float width, float length, const Texture2D& texture, const glm::mat4& model)
    : Drawable(scene)
    , m_model(model)
    , m_diffuse(texture)
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
                                          , { static_cast<float>(x), static_cast<float>(y) }
                                          , { 0.f, 1.f, 0.f}});
        }
    }

    std::vector<unsigned> indices;

    genIndices(indices, 0, segments, 0, segments, segments, 8); 

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
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

    glBindVertexArray(0);

    m_index_count = indices.size();
}

QuadDrawable::~QuadDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void QuadDrawable::update()
{
}

void QuadDrawable::render() const
{
    auto& shaderProgram = *scene().getSceneState().shaderProgram;
    ModelShaderProgram(shaderProgram).Apply(m_model);

    auto viewModel = scene().getCamera().View() * m_model;
    NormalShaderProgram(shaderProgram).Apply(viewModel);
    
    TextureMaterial textureMaterial{ 0, 1, 1.f };
    TextureMaterialShaderProgram(shaderProgram).Apply(textureMaterial);

    glDisable(GL_CULL_FACE);
    m_diffuse.Use(GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}
