#include "QuadDrawable.hpp"

#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"
#include <GL/glew.h>

namespace
{
    using namespace std::string_literals;

    const auto vertex_shader = R"vertex(
    #version 420 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTex;

    out vec2 texCoords;
    
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
        texCoords = aTex;     
    }
    )vertex"s;

    const auto fragment_shader = R"fragment(
    #version 420 core
    #extension GL_ARB_bindless_texture : require
    layout (location = 0) in vec3 aPos;
    out vec4 fragColor;
    in vec2 texCoords;
    
    layout(bindless_sampler) uniform sampler2D texture0;

    void main()
    {
        vec4 tex = texture(texture0, texCoords);
        fragColor = vec4(tex.rgb, 1.0f);
    }
    )fragment"s;
}

QuadDrawable::QuadDrawable()
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
    float vertices[][5] = {{ -size, -size, 0.0f, 0.0f, 0.0f }
                          ,{ -size, size, 0.0f, 0.0f, 1.0f }
                          ,{ size, size, 0.0f, 1.0f, 1.0f }
                          ,{ size, -size, 0.0f, 1.0f, 0.0f }};

    int indices[] = { 0, 1, 2, 0, 2, 3 };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    m_texture = std::make_unique<Texture2D>("./wall.jpg", GL_RGB, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR   
                                              , GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    m_texture->MakeResident();
}

QuadDrawable::~QuadDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void QuadDrawable::render() const
{
    m_shaderProgram->Use();
    const auto tex_handle = m_texture->GetHandle();
    m_shaderProgram->SetUniformHandle("texture0", tex_handle);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}