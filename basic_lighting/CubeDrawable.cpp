#include "CubeDrawable.hpp"

#include "Scene.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "TextureManager.hpp"
#include <chrono>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
    using namespace std::string_literals;

    struct vertex_coords
    {
        float x;
        float y;
        float z;
    };

    struct vertex_color 
    {
        float r;
        float g;
        float b;
    };

    struct vertex_tex_coords
    {
        float s;
        float t;
    };

    struct vertex_normal_coords
    {
        float nx;
        float ny;
        float nz;
    };

    struct vertex
    {
        vertex_coords coords;
        vertex_color color;
        vertex_tex_coords tex_coords;
        vertex_normal_coords normal_coords;
    };

    const auto vertex_shader = R"vertex(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexCoords;
    layout (location = 3) in vec3 aNormalCoords;

    out vec3 vColor;
    out vec2 texCoords;
    out vec3 normalCoords;
    out vec3 fragPos;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 normal;
    
    void main()
    {
        gl_Position = projection * view * model * vec4(aPos.xyz, 1.0f);
        vColor = aColor; 
        texCoords = aTexCoords;
        normalCoords = (mat3(normal)) * aNormalCoords;
        fragPos = vec3(model * vec4(aPos.xyz, 1.0f));
    }
    )vertex"s;

    const auto fragment_shader = R"fragment(
    #version 330 core
    in vec3 vColor;
    in vec2 texCoords;
    in vec3 normalCoords;
    in vec3 fragPos;
    out vec4 fragColor;

    uniform sampler2D texture0;
    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 cameraPos;

    void main()
    {
        vec3 lightDir = normalize(lightPos - fragPos);        
        vec3 norm = normalize(normalCoords);
        float diff = max(dot(norm, lightDir), 0.f);
        vec3 diffuse = diff * lightColor;

        const float specularStrenght = 1.0f;
        vec3 viewDir = normalize(cameraPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.f), 256);
        vec3 specular = specularStrenght * spec * lightColor;

        const float ambientStrength = 0.1f;
        vec3 ambient = ambientStrength * lightColor;

        vec4 outTexture = texture(texture0, texCoords);
        vec3 color = vec3(outTexture.xyz) * vColor * (ambient + diffuse + specular);
        //vec3 color = vec3(outTexture.xyz) * vColor * specular;
        fragColor = vec4(color.xyz, 1.0f);
    }
    )fragment"s;
}

CubeDrawable::CubeDrawable(const Scene& scene, glm::vec3 pos)
    : Drawable(scene)
    , m_model(1.f)
    , m_pos(pos)
    , m_lightpos(pos)
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(6 * sizeof(float)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(8 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    m_texture = &TextureManager::instance()["awesomeface"];
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
    auto scalar = duration.count() / 2000.f;
    auto rot = glm::rotate(glm::mat4(1.f), scalar, glm::vec3(0.0f, 1.0f, 0.0f));
    //m_model = glm::translate(m_model, m_pos);
    m_model = glm::translate(glm::mat4(1.f), m_pos);
    m_lightpos = glm::vec4(0.0f, 0.f, 30.f, 1.0f) * rot; 
    //m_model = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
}

void CubeDrawable::render() const
{
    m_shaderProgram->Use();
    m_shaderProgram->SetUniformMatrix4("model", glm::value_ptr(m_model));
    m_shaderProgram->SetUniformMatrix4("view", glm::value_ptr(scene().getCamera().View()));
    m_shaderProgram->SetUniformMatrix4("projection", glm::value_ptr(scene().Projection()));

    auto inverse_model = glm::inverse(m_model);
    auto normal_model = glm::transpose(inverse_model);

    m_shaderProgram->SetUniformMatrix4("normal", glm::value_ptr(normal_model));
    m_shaderProgram->SetUniform("texture0", 0);
    m_shaderProgram->SetUniform("lightColor", 1.f, 1.f, 1.f);
    m_shaderProgram->SetUniform("lightPos", m_lightpos.x, m_lightpos.y, m_lightpos.z);
    auto view = scene().getCamera().View();
    auto cameraTransform = glm::inverse(view);
    m_shaderProgram->SetUniform("cameraPos", cameraTransform[3].x, cameraTransform[3].y, cameraTransform[3].z); 

    m_texture->Use(GL_TEXTURE0);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
