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

ModelDrawable::ModelDrawable(Scene& scene)
    : Drawable(scene)
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

    const float size = 0.8f;
    std::vector<vertex> vertices = {{ -size, -size, -size, 1.0f, 0.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, -1.f }
                        ,{ -size, size, -size, 0.0f, 1.0f, 0.0f, 0.f, 1.f, 0.f, 0.f, -1.f }
                        ,{ size, size, -size, 1.0f, 1.0f, 0.0f, 1.f, 1.f, 0.f, 0.f, -1.f }
                        ,{ size, -size, -size, 0.0f, 0.0f, 1.0f, 1.f, 0.f, 0.f, 0.f, -1.f }
                        ,{ -size, -size, size, 1.0f, 0.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 1.f }
                        ,{ -size, size, size, 0.0f, 1.0f, 0.0f, 0.f, 1.f, 0.f, 0.f, 1.f }
                        ,{ size, size, size, 1.0f, 1.0f, 0.0f, 1.f, 1.f, 0.f, 0.f, 1.f }
                        ,{ size, -size, size, 0.0f, 0.0f, 1.0f, 1.f, 0.f, 0.f, 0.f, 1.f }
                        ,{ -size, size, -size, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f }
                        ,{ -size, size, size, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f }
                        ,{ size, size, size, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f }
                        ,{ size, size, -size, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f }
                        ,{ -size, -size, -size, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f }
                        ,{ -size, -size, size, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, -1.f, 0.f }
                        ,{ size, -size, size, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, -1.f, 0.f }
                        ,{ size, -size, -size, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, -1.f, 0.f }
                        ,{ -size, -size, size, 1.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f }
                        ,{ -size, size, size, 0.f, 1.f, 0.f, 0.f, 1.f, -1.f, 0.f, 0.f }
                        ,{ -size, size, -size, 0.f, 1.f, 0.f, 1.f, 1.f, -1.f, 0.f, 0.f }
                        ,{ -size, -size, -size, 1.f, 0.f, 0.f, 1.f, 0.f, -1.f, 0.f, 0.f }
                        ,{ size, -size, size, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f }
                        ,{ size, size, size, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f }
                        ,{ size, size, -size, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f }
                        ,{ size, -size, -size, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f }};

    std::vector<unsigned> indices = { 0, 1, 2
                    , 2, 3, 0
                    , 7, 6, 5
                    , 5, 4, 7
                    , 8, 9, 10
                    , 10, 11, 8
                    , 15, 14, 13 
                    , 13, 12, 15
                    , 16, 17, 18 
                    , 18, 19, 16
                    , 23, 22, 21 
                    , 21, 20, 23 }; 
    
    m_meshes.emplace_back(vertices, indices);
}

ModelDrawable::~ModelDrawable()
{
    glDeleteVertexArrays(1, &m_VAO);
}

void ModelDrawable::render() const
{
    glBindVertexArray(m_VAO);

    auto& shaderProgram = *scene().getSceneState().shaderProgram;
    ModelShaderProgram(shaderProgram).Apply(glm::mat4(1.f));

    auto viewModel = scene().getCamera().View() * glm::mat4(1.f);
    NormalShaderProgram(shaderProgram).Apply(viewModel);

    for (auto& mesh : m_meshes)
    {
        mesh.Render();
    }
}
