#include "CubeDrawable.hpp"

#include "Scene.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "TextureManager.hpp"
#include "MaterialManager.hpp"
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

    #define NR_POINT_LIGHTS 2

    struct Material
    {
        sampler2D diffuse;
        sampler2D specular;
        float shininess;
    };

    uniform Material material;

    struct SpotLight
    {
        vec3 position; //in view space
        vec3 direction;
        float cutOff;
        float outerCutOff;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

    struct DirectionalLight
    {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    struct PointLight
    {
        vec3 position; //in view space
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

    uniform SpotLight spotLight;
    uniform PointLight pointLights[NR_POINT_LIGHTS];
    uniform DirectionalLight directionalLight;

    vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos);
    vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos);
    vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos);

    void main()
    {
        vec3 color = calcDirectionalLight(directionalLight, normalCoords, fragPos); 

        for(int i = 0; i < NR_POINT_LIGHTS; i++)
            color += calcPointLight(pointLights[i], normalCoords, fragPos);

        color += calcSpotLight(spotLight, normalCoords, fragPos); 
        fragColor = vec4(color.xyz, 1.0f);
    }

    vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos)
    {
        vec3 lightDir = normalize(-fragPos);        
        float theta = dot(lightDir, normalize(-spotLight.direction));
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        //attenuation
        float distance = length(light.position - fragPos);
        float attenuation = 1.f / (light.constant + light.linear * distance
                             + light.quadratic * (distance * distance));

        //ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

        //duffuse
        vec3 norm = normalize(normal);
        float diff = max(dot(norm, lightDir), 0.f);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

        //specular
        vec3 viewDir = normalize(-fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
        vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));

        ambient *= attenuation * intensity;
        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;

        return ambient + diffuse + specular;
    }

    vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos)
    {
        vec3 lightDir = normalize(-light.direction);        

        //ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

        //duffuse
        vec3 norm = normalize(normal);
        float diff = max(dot(norm, lightDir), 0.f);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

        //specular
        vec3 viewDir = normalize(-fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
        vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));

        return ambient + diffuse + specular;
    }

    vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos)
    {
        //attenuation
        float distance = length(light.position - fragPos);
        float attenuation = 1.f / (light.constant + light.linear * distance
                             + light.quadratic * (distance * distance));

        //ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

        //duffuse
        vec3 lightDir = normalize(light.position - fragPos);        
        vec3 norm = normalize(normalCoords);
        float diff = max(dot(norm, lightDir), 0.f);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

        //specular
        vec3 viewDir = normalize(-fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
        vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        return ambient + diffuse + specular;
    }
    )fragment"s;
}

CubeDrawable::CubeDrawable(const Scene& scene, glm::vec3 pos, int material)
    : Drawable(scene)
    , m_model(1.f)
    , m_pos(pos)
    , m_material(material)
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, coords)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, color)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, tex_coords))); 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, normal_coords)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    m_diffuse = &TextureManager::instance()["container2"];
    m_specular = &TextureManager::instance()["container2specular"];
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
    m_lightTransform0 = glm::rotate(glm::mat4(1.f), scalar, glm::vec3(0.0f, 1.0f, 0.0f));
    m_lightTransform1 = glm::rotate(glm::mat4(1.f), scalar, glm::vec3(1.0f, 0.0f, 0.0f));
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
    m_shaderProgram->SetUniform("material.diffuse", 0);
    m_shaderProgram->SetUniform("material.specular", 1);
    
    //setting material
    auto& material = MaterialManager::instance()[static_cast<MATERIAL_ID>(m_material)];
    m_shaderProgram->SetUniform("material.ambient", material.ambient.x, material.ambient.y, material.ambient.z);
    m_shaderProgram->SetUniform("material.diffuse", material.diffuse.x, material.diffuse.y, material.diffuse.z);
    m_shaderProgram->SetUniform("material.specular",material.specular.x, material.specular.y, material.specular.z); 
    m_shaderProgram->SetUniform("material.shininess", material.shininess * 128); 

    //setting light
    
    auto& camera = scene().getCamera();
    m_shaderProgram->SetUniform("spotLight.position", 0.f, 0.f, 0.f); 
    m_shaderProgram->SetUniform("spotLight.direction", 0.f, 0.f, -1.f); 
    m_shaderProgram->SetUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f))); 
    m_shaderProgram->SetUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f))); 
    m_shaderProgram->SetUniform("spotLight.ambient", 0.2f, 0.2f, 0.2f); 
    m_shaderProgram->SetUniform("spotLight.diffuse", 1.f, 1.f, 1.f); 
    m_shaderProgram->SetUniform("spotLight.specular", 1.f, 1.f, 1.f); 

    m_shaderProgram->SetUniform("spotLight.constant", 1.f); 
    m_shaderProgram->SetUniform("spotLight.linear", 0.022f); 
    m_shaderProgram->SetUniform("spotLight.quadratic", 0.0019f); 

    const auto viewLightDir = camera.View() * glm::vec4(-1.f, 0.f, 0.f, 0.f);
    m_shaderProgram->SetUniform("directionalLight.direction", viewLightDir.x, viewLightDir.y, viewLightDir.z);
    m_shaderProgram->SetUniform("directionalLight.ambient", 0.f, 0.f, 0.f); 
    m_shaderProgram->SetUniform("directionalLight.diffuse", 0.01f, 0.01f, 0.01f); 
    m_shaderProgram->SetUniform("directionalLight.specular", 0.01f, 0.01f, 0.01f); 

    {
        auto viewLightPos0 = camera.View() * m_lightTransform0 * glm::vec4(0.f, 0.f, 11.f, 1.f);
        m_shaderProgram->SetUniform("pointLights[0].position", viewLightPos0.x, viewLightPos0.y, viewLightPos0.z);
        m_shaderProgram->SetUniform("pointLights[0].ambient", 0.1f, 0.1f, 0.1f); 
        m_shaderProgram->SetUniform("pointLights[0].diffuse", 1.f, 1.f, 1.f); 
        m_shaderProgram->SetUniform("pointLights[0].specular", 1.f, 1.f, 1.f); 

        m_shaderProgram->SetUniform("pointLights[0].constant", 1.f); 
        m_shaderProgram->SetUniform("pointLights[0].linear", 0.09f); 
        m_shaderProgram->SetUniform("pointLights[0].quadratic", 0.032f); 

        auto viewLightPos1 = camera.View() * m_lightTransform1 * glm::vec4(0.f, 0.f, -11.f, 1.f);
        m_shaderProgram->SetUniform("pointLights[1].position", viewLightPos1.x, viewLightPos1.y, viewLightPos1.z);
        m_shaderProgram->SetUniform("pointLights[1].ambient", 0.1f, 0.1f, 0.1f); 
        m_shaderProgram->SetUniform("pointLights[1].diffuse", 1.f, 1.f, 1.f); 
        m_shaderProgram->SetUniform("pointLights[1].specular", 1.f, 1.f, 1.f); 

        m_shaderProgram->SetUniform("pointLights[1].constant", 1.f); 
        m_shaderProgram->SetUniform("pointLights[1].linear", 0.09f); 
        m_shaderProgram->SetUniform("pointLights[1].quadratic", 0.032f); 
    }

    m_diffuse->Use(GL_TEXTURE0);
    m_specular->Use(GL_TEXTURE1);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
