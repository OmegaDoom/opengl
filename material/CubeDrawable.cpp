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
        normalCoords = mat3(normal) * aNormalCoords;
        fragPos = vec3(view * model * vec4(aPos.xyz, 1.0f));
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

    struct Material
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
    };

    uniform Material material;

    struct Light
    {
        vec3 position; //in view space
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    uniform Light light;

    void main()
    {
        //ambient
        vec3 ambient = light.ambient * material.ambient;

        //duffuse
        vec3 lightDir = normalize(light.position - fragPos);        
        vec3 norm = normalize(normalCoords);
        float diff = max(dot(norm, lightDir), 0.f);
        vec3 diffuse = (diff * material.diffuse) * light.diffuse;

        //specular
        const float specularStrenght = 0.5f;
        vec3 viewDir = normalize(-fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
        vec3 specular = material.specular * spec * light.specular;

        vec4 outTexture = texture(texture0, texCoords);
        vec3 color = vec3(outTexture.xyz) * vColor * (ambient + diffuse + specular);
        fragColor = vec4(color.xyz, 1.0f);
    }
    )fragment"s;
}

CubeDrawable::CubeDrawable(const Scene& scene, glm::vec3 pos)
    : Drawable(scene)
    , m_model(1.f)
    , m_pos(pos)
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
    auto scalar = duration.count() / 1000.f;
    //m_model = glm::rotate(glm::mat4(1.f), scalar, glm::vec3(1.0f, 1.0f, 0.0f));
    //m_model = glm::translate(m_model, m_pos);
    m_model = glm::translate(glm::mat4(1.f), m_pos);
    m_lightTransform = glm::rotate(glm::mat4(1.f), scalar, glm::vec3(0.0f, 1.0f, 0.0f));
    //lightTransform = glm::translate(lightTransform, glm::vec3(0.f, 0.f, 20.f));
}

void CubeDrawable::render() const
{
    m_shaderProgram->Use();
    m_shaderProgram->SetUniformMatrix4("model", glm::value_ptr(m_model));
    m_shaderProgram->SetUniformMatrix4("view", glm::value_ptr(scene().getCamera().View()));
    m_shaderProgram->SetUniformMatrix4("projection", glm::value_ptr(scene().Projection()));

    auto view_model = scene().getCamera().View() * m_model;
    auto inverse_view_model = glm::inverse(view_model);
    auto normal_transform = glm::transpose(inverse_view_model);

    m_shaderProgram->SetUniformMatrix4("normal", glm::value_ptr(normal_transform));
    m_shaderProgram->SetUniform("texture0", 0);
    
    //setting material
    m_shaderProgram->SetUniform("material.ambient", 1.f, 1.f, 1.f);
    m_shaderProgram->SetUniform("material.diffuse", 1.f, 1.f, 1.f);
    m_shaderProgram->SetUniform("material.specular", 0.5f, 0.5f, 0.5f);
    m_shaderProgram->SetUniform("material.shininess", 128.f); 

    //setting light
    auto viewLightPos = scene().getCamera().View() * m_lightTransform * glm::vec4(0.f, 0.f, 20.f, 1.f);
    m_shaderProgram->SetUniform("light.position", viewLightPos.x, viewLightPos.y, viewLightPos.z);
    m_shaderProgram->SetUniform("light.ambient", 0.1f, 0.1f, 0.1f); 
    m_shaderProgram->SetUniform("light.diffuse", 0.5f, 0.5f, 0.5f); 
    m_shaderProgram->SetUniform("light.specular", 1.f, 1.f, 1.f); 

    m_texture->Use(GL_TEXTURE0);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
