#include "TriangleDrawable.hpp"

#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include <chrono>
#include <GL/glew.h>
#include <iostream>

namespace
{
    using namespace std::string_literals;

    const auto vertex_shader = R"vertex(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 vColor;
    
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
        vColor = aColor; 
    }
    )vertex"s;

    const auto fragment_shader = R"fragment(
    #version 330 core
    in vec3 vColor;
    out vec4 fragColor;
    uniform float scalar;

    void main()
    {
        float multScalar = scalar * 3.0f;
        float normScalar = multScalar - floor(multScalar);
        int colorIndex = int(floor(multScalar));
        int index0 = colorIndex % 3;
        int index1 = (colorIndex + 1) % 3;
        int index2 = (colorIndex + 2) % 3;
        float invScalar = 1.0f - normScalar;
        float color0 = vColor[index0] * invScalar + vColor[index1] * normScalar; 
        float color1 = vColor[index1] * invScalar + vColor[index2] * normScalar;
        float color2 = vColor[index2] * invScalar + vColor[index0] * normScalar;
        fragColor = vec4(color0, color1, color2, 1.0f);
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
    float vertices[][6] = {{ -size, -size, 0.0f, 1.0f, 0.0f, 0.0f }
                          ,{ 0.0f, size, 0.0f, 0.0f, 1.0f, 0.0f }
                          ,{ size, -size, 0.0f, 0.0f, 0.0f, 1.0f }};

    int indices[] = { 0, 1, 2 };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(m_vao);
}

TriangleDrawable::~TriangleDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void TriangleDrawable::update()
{
    const auto epoch = std::chrono::time_point<std::chrono::steady_clock>{};
    const auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - epoch);
    m_scalar = ((duration.count()) % 1000) / 1000.f;
}

void TriangleDrawable::render() const
{
    m_shaderProgram->Use();
    m_shaderProgram->SetUniform("scalar", m_scalar);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}
