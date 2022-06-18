#include "CubeDrawable.hpp"

#include "Scene.hpp"
#include "ModelShaderProgram.hpp"
#include "NormalShaderProgram.hpp"
#include "Vertex.hpp"
#include <chrono>
#include <GL/glew.h>

namespace
{
    using namespace std::string_literals;
}

CubeDrawable::CubeDrawable(Scene& scene)
    : Drawable{ scene }
    , m_model{ 1.f }
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    const float size = 0.8f;
    vertex vertices[] = {{ -size, -size, -size, 1.0f, 0.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, -1.f }
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

    int indices[] = { 0, 1, 2
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

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
}

CubeDrawable::~CubeDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void CubeDrawable::SetModel(const glm::mat4& model)
{
    m_model = model;
}

void CubeDrawable::update()
{
    const auto epoch = std::chrono::time_point<std::chrono::steady_clock>{};
    const auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - epoch);
    auto scalar = duration.count() / 1000.f;
}

void CubeDrawable::render() const
{
    auto& shaderProgram = *scene().getSceneState().shaderProgram;
    ModelShaderProgram(shaderProgram).Apply(m_model);

    auto viewModel = scene().getCamera().View() * m_model;
    NormalShaderProgram(shaderProgram).Apply(viewModel);
    
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
