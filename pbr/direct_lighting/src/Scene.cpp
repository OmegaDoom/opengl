#include "Scene.hpp"

#include "ShaderProgramDrawable.hpp"
#include "SceneLightingDrawable.hpp"
#include "SceneCameraDrawable.hpp"
#include "MaterialDrawable.hpp"
#include "CratesDrawable.hpp"
#include "ModelDrawable.hpp"
#include "ModelNormalTransformDrawable.hpp"
#include "SphereDrawable.hpp"
#include "ModelLoader.hpp"
#include "CallbackDrawable.hpp"
#include "PbrMaterialDrawable.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene()
    : m_directionalLight{ glm::vec3{0.f, 0.f, -1.f}
                        , glm::vec3{0.1f, 0.1f, 0.1f}
                        , glm::vec3{0.2f, 0.2f, 0.2f}, glm::vec3{0.05f, 0.05f, 0.05f} }
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
    MakeAndAddSceneObject<ShaderProgramDrawable>("pbr_material");
    MakeAndAddSceneObject<SceneCameraDrawable>();
    MakeAndAddSceneObject<SceneLightingDrawable>();

    const float radius = 0.15;
    const float x_count = 8;
    const float y_count = 8;

    auto& sphere = MakeSceneObject<SphereDrawable>(radius);

    for (int y = 0; y < y_count; ++y)
    {
        for (int x = 0; x < x_count; ++x)
        {
            const float x_coord = -((x_count * radius * 2) / 2.0f) + (x * radius * 2) + radius;
            const float y_coord = -((y_count * radius * 2) / 2.0f) + (y * radius * 2) + radius;
            auto transform = glm::translate(glm::mat4{1.0}, glm::vec3{x_coord, y_coord, 0.0f});
            MakeAndAddSceneObject<PbrMaterialDrawable>(PbrMaterial{glm::vec3(1.f, 0.0f, 0.0f), y / (y_count - 1), std::max(x / (x_count - 1), 0.05f), 0.1f});
            MakeAndAddSceneObject<ModelNormalTransformDrawable>(transform);
            AddSceneObject<SphereDrawable>(sphere);
        }
    }

    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(1.f), 1.f, 0.09f, 0.032f });
    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(1.f), 1.f, 0.09f, 0.032f });

    m_lightInitialPositions.push_back(glm::vec4(0.f, 0.f, 4.f, 1.f));
    m_lightInitialPositions.push_back(glm::vec4(0.f, 0.f, -4.f, 1.f));

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

    for (auto& updateable : m_updateables)
    {
        updateable.get().Update();
    }
}

void Scene::Render() const
{
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& drawable : m_drawables)
    {
        drawable.get().Render();
    }
}

template<typename T, typename ... Args>
T& Scene::MakeSceneObject(Args&&... args)
{
    auto scene_object = std::make_unique<T>(*this, args...);
    auto& scene_object_ref = *scene_object;
    m_scene_objects.push_back(std::move(scene_object));
    return scene_object_ref;
}

template<typename T, typename ... Args>
T& Scene::MakeAndAddSceneObject(Args&&... args)
{
    auto scene_object = std::make_unique<T>(*this, args...);
    auto& scene_object_ref = *scene_object;
    AddSceneObject(scene_object_ref);
    m_scene_objects.push_back(std::move(scene_object));
    return scene_object_ref;
}

template<typename T>
void Scene::AddSceneObject(T& object)
{
    if constexpr(std::is_base_of_v<Drawable, T>)
    {
        m_drawables.push_back(object);
    }

    if constexpr(std::is_base_of_v<Updateable, T>)
    {
        m_updateables.push_back(object);
    }
}

template<typename T>
T& Scene::AddSceneObject(std::unique_ptr<T> object)
{
    auto& object_ref = *object;
    AddSceneObject(*object);
    m_scene_objects.push_back(std::move(object));
    return object_ref;
}

void Scene::ChangeDimension(int width, int height)
{
    m_projection = glm::perspective(glm::radians(45.f), static_cast<float>(width) / height, 0.1f, 1000.f);
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