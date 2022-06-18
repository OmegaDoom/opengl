#include "CubeDrawable.hpp"

#include "Scene.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include <chrono>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
    using namespace std::string_literals;

    struct vertex
    {
        float x;
        float y;
        float z;
        float r;
        float g;
        float b;
    };

    const auto vertex_shader = R"vertex(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    smooth out vec3 vColor;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    void main()
    {
        gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
        vColor = aColor; 
    }
    )vertex"s;

    const auto fragment_shader = R"fragment(
    #version 330 core
    smooth in vec3 vColor;
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(vColor.xyz, 1.0f);
    }
    )fragment"s;
}

CubeDrawable::CubeDrawable(const Scene& scene)
    : Drawable(scene)
    , m_model(1.f)
{
    m_shaderProgram = std::make_unique<ShaderProgram>(Shader<GL_VERTEX_SHADER>(vertex_shader)
                                                      , Shader<GL_FRAGMENT_SHADER>(fragment_shader));
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    const float size = 0.6f;
    vertex vertices[] = {{ -size, -size, -size, 1.0f, 0.0f, 0.0f }
                        ,{ -size, size, -size, 0.0f, 1.0f, 0.0f }
                        ,{ size, size, -size, 1.0f, 1.0f, 0.0f }
                        ,{ size, -size, -size, 0.0f, 0.0f, 1.0f }
                        ,{ -size, -size, size, 1.0f, 0.0f, 0.0f }
                        ,{ -size, size, size, 0.0f, 1.0f, 0.0f }
                        ,{ size, size, size, 1.0f, 1.0f, 0.0f }
                        ,{ size, -size, size, 0.0f, 0.0f, 1.0f }};

    int indices[] = { 0, 1, 2
                    , 2, 3, 0
                    , 7, 6, 5
                    , 5, 4, 7
                    , 4, 5, 1
                    , 1, 0, 4
                    , 3, 2, 6
                    , 6, 7, 3
                    , 1, 5, 6
                    , 6, 2, 1
                    , 4, 0, 3
                    , 3, 7, 4 }; 

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(3 * sizeof(float)));


    glBindVertexArray(0);
}

CubeDrawable::~CubeDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void CubeDrawable::update()
{
    const auto epoch = std::chrono::time_point<std::chrono::steady_clock>{};
    const auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - epoch);
    auto scalar = duration.count() / 600.f;
    m_model = glm::rotate(glm::mat4(1.0), scalar, glm::vec3(1.0f, 1.0f, 0.0f));
}

void CubeDrawable::render() const
{
    m_shaderProgram->Use();
    m_shaderProgram->SetUniformMatrix4("model", glm::value_ptr(m_model));
    m_shaderProgram->SetUniformMatrix4("view", glm::value_ptr(scene().View()));
    m_shaderProgram->SetUniformMatrix4("projection", glm::value_ptr(scene().Projection()));
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
