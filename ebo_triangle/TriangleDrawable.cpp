#include "TriangleDrawable.hpp"

#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include <GL/glew.h>

namespace
{
    using namespace std::string_literals;

    const auto vertex_shader = R"vertex(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    }
    )vertex"s;

    const auto fragment_shader = R"fragment(
    #version 330 core
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    }
    )fragment"s;
}

TriangleDrawable::TriangleDrawable()
{
    m_shaderProgram = std::make_unique<ShaderProgram>(Shader<GL_VERTEX_SHADER>(vertex_shader)
                                                      , Shader<GL_FRAGMENT_SHADER>(fragment_shader));
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    const float size = 0.8f;
    float vertices[][3] = {{ -size, -size, 0.0f }
                          ,{ 0.0f, size, 0.0f }
                          ,{ size, -size, 0.0f }};

    int indices[] = { 0, 1, 2 };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

TriangleDrawable::~TriangleDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void TriangleDrawable::render() const
{
    m_shaderProgram->Use();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}
