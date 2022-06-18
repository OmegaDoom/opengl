#include "Scene.hpp"

#include "ShaderProgramDrawable.hpp"
#include "SceneLightingDrawable.hpp"
#include "SceneCameraDrawable.hpp"
#include "QuadDrawable.hpp"
#include "CubeDrawable.hpp"
#include "TriangleDrawable.hpp"
#include "LightingMapsDrawable.hpp"
#include "TextureManager.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene()
    : m_directionalLight{ glm::vec3{0.f, 0.f, 0.f}
                        , glm::vec3{0.f, 0.f, 0.f}
                        , glm::vec3{0.05f, 0.05f, 0.05f}, glm::vec3{0.05f, 0.05f, 0.05f} }
    , m_spotLight{ glm::vec3(0.f)
            , glm::vec3(0.f, 0.f, -1.f)
            , glm::vec3(0.2f)
            , glm::vec3(1.f)
            , glm::vec3(1.f)
            , glm::cos(glm::radians(12.5f))
            , glm::cos(glm::radians(17.5f))
            , 1.f
            , 0.022f
            , 0.0019f }
{
    MakeAndAddSceneObject<ShaderProgramDrawable>("triangle");
    m_triangle0 = &MakeAndAddSceneObject<TriangleDrawable>(1, 1, TextureManager::instance()["wall"]);
    m_triangle0->SetTextureCoordinates({0.f, 0.f}, {0.f, 1.f}, {1.f, 0.f});
    auto& quad_drawable = MakeAndAddSceneObject<QuadDrawable>(2.f, 2.f);
    m_triangle1 = &MakeAndAddSceneObject<TriangleDrawable>(1, 1, TextureManager::instance()["wall"]);
    m_triangle1->SetTextureCoordinates({0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f});
    m_drawables.push_back(quad_drawable);

    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 1.f, 0.09f, 0.032f });
    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 1.f, 0.09f, 0.032f });    

    m_lightInitialPositions.push_back(glm::vec4(0.f, 0.f, 11.f, 1.f));
    m_lightInitialPositions.push_back(glm::vec4(0.f, 0.f, -11.f, 1.f));

    m_lightRotationAxis.push_back(glm::vec3(0.f, 1.f, 0.f));
    m_lightRotationAxis.push_back(glm::vec3(1.f, 0.f, 0.f));
}

void Scene::Update()
{
    m_camera.Update();

    const auto epoch = std::chrono::time_point<std::chrono::steady_clock>{};
    const auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - epoch);
    auto scalar = duration.count() / 1000.f;

    const auto viewLightDir = m_camera.View() * glm::vec4(-1.f, 0.f, 0.f, 0.f);
    m_directionalLight.direction.x = viewLightDir.x;
    m_directionalLight.direction.y = viewLightDir.y;
    m_directionalLight.direction.z = viewLightDir.z;

    for (int i = 0; i < m_pointLights.size(); ++i)
    {
        const auto lightTransform = glm::rotate(glm::mat4(1.f), scalar, m_lightRotationAxis[i]); 
        const auto viewLightPos = m_camera.View() * lightTransform * m_lightInitialPositions[i]; 
        m_pointLights[i].position.x = viewLightPos.x;
        m_pointLights[i].position.y = viewLightPos.y;
        m_pointLights[i].position.z = viewLightPos.z;
    }

    auto rotation = glm::rotate(glm::mat4(1.f), scalar, glm::vec3(0.f, 0.f, 1.f));
    auto point0 = glm::vec4{-0.5f, -0.5f, 0.f, 1.f} * rotation;
    auto point1 = glm::vec4{-0.5f, 0.5f, 0.f, 1.f} * rotation;
    auto point2 = glm::vec4{0.5f, -0.5f, 0.f, 1.f} * rotation;
    m_triangle0->SetCoordinates(glm::vec2{point0.x, point0.y}, glm::vec2{point1.x, point1.y}, glm::vec2{point2.x, point2.y});

    auto point3 = glm::vec4{-0.5f, 0.5f, 0.f, 1.f} * rotation;
    auto point4 = glm::vec4{0.5f, 0.5f, 0.f, 1.f} * rotation;
    auto point5 = glm::vec4{0.5f, -0.5f, 0.f, 1.f} * rotation;
    m_triangle1->SetCoordinates(glm::vec2{point3.x, point3.y}, glm::vec2{point4.x, point4.y}, glm::vec2{point5.x, point5.y});

    for (auto& updateable : m_updateables)
    {
        updateable.get().Update();
    }
}

void Scene::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& drawable : m_drawables)
    {
        drawable.get().Render();
    }
}

template<typename T, typename ... Args>
T& Scene::MakeAndAddSceneObject(Args&&... args)
{
    auto object = std::make_unique<T>(*this, args...);
    auto &ref = *object;

    if constexpr(std::is_base_of_v<Drawable, T>)
    {
        m_drawables.push_back(ref);
    }

    if constexpr(std::is_base_of_v<Updateable, T>)
    {
        m_updateables.push_back(ref);
    }

    m_scene_objects.push_back(std::move(object));
    return ref;
}

void Scene::ChangeDimension(int width, int height)
{
    m_projection = glm::perspective(glm::radians(45.f), static_cast<float>(width) / height, 0.1f, 1000.f);
    m_triangle0->SetScreenSize(width, height);
    m_triangle1->SetScreenSize(width, height);
}

Camera& Scene::getCamera()
{
    return m_camera;
}

const Camera& Scene::getCamera() const
{
    return m_camera;
}

const glm::mat4& Scene::Projection() const
{
    return m_projection;
}

const DirectionalLight& Scene::getDirectionalLight() const
{
    return m_directionalLight;
}

const SpotLight& Scene::getSpotLight() const
{
    return m_spotLight;
}

const std::vector<PointLight>& Scene::getPointLights() const
{
    return m_pointLights;
}

SceneState& Scene::getSceneState()
{
    return m_sceneState;
}