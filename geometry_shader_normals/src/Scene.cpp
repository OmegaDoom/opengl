#include "Scene.hpp"

#include "ShaderProgramDrawable.hpp"
#include "SceneLightingDrawable.hpp"
#include "SceneCameraDrawable.hpp"
#include "MaterialDrawable.hpp"
#include "CratesDrawable.hpp"
#include "ModelDrawable.hpp"
#include "ModelLoader.hpp"
#include <GL/glew.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Scene::Scene()
    : m_directionalLight{ glm::vec3{0.f, 1.f, 0.f}
                        , glm::vec3{0.1f, 0.1f, 0.1f}
                        , glm::vec3{0.1f, 0.1f, 0.1f}, glm::vec3{0.05f, 0.05f, 0.05f} }
    , m_spotLight{ glm::vec3(0.f)
            , glm::vec3(0.f, 0.f, -1.f)
            , glm::vec3(0.2f)
            , glm::vec3(1.f)
            , glm::vec3(1.f)
            , glm::cos(glm::radians(12.5f))
            , glm::cos(glm::radians(17.5f))
            , 1.f
            , 0.001f
            , 0.00002f }
{
    ModelLoader modelLoader;

    MakeAndAddSceneObject<SceneCameraDrawable>();
    MakeAndAddSceneObject<SceneLightingDrawable>();

    MakeAndAddSceneObject<ShaderProgramDrawable>("textureMaterial");
    auto& model = AddSceneObject(modelLoader.Load(*this, "backpack/backpack.obj"));

    MakeAndAddSceneObject<ShaderProgramDrawable>("textureMaterialNormals");
    AddSceneObject(model);
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

void Scene::Update()
{
    m_camera.Update();

    const auto epoch = std::chrono::time_point<std::chrono::steady_clock>{};
    const auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - epoch);
    auto scalar = duration.count() / 1000.f;

    const auto viewLightDir = m_camera.View() * glm::vec4(0.f, 1.f, 0.f, 0.f);
    m_directionalLight.direction.x = viewLightDir.x;
    m_directionalLight.direction.y = viewLightDir.y;
    m_directionalLight.direction.z = viewLightDir.z;

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

void Scene::ChangeDimension(int width, int height)
{
    m_projection = glm::perspective(glm::radians(45.f), static_cast<float>(width) / height, 0.1f, 3000.f);
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

const std::deque<PointLight>& Scene::getPointLights() const
{
    return m_pointLights;
}

PointLight& Scene::AddPointLight(const PointLight& light)
{
    m_pointLights.push_back(light);
    return m_pointLights.back();
}

SceneState& Scene::getSceneState()
{
    return m_sceneState;
}
