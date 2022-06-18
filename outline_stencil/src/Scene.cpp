#include "Scene.hpp"

#include "ShaderProgramDrawable.hpp"
#include "SceneLightingDrawable.hpp"
#include "SceneCameraDrawable.hpp"
#include "ModelNormalDrawable.hpp"
#include "MaterialDrawable.hpp"
#include "TextureMaterialDrawable.hpp"
#include "CubeDrawable.hpp"
#include "SphereDrawable.hpp"
#include "PlaneDrawable.hpp"
#include "CallbackDrawable.hpp"
#include <algorithm>
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
    auto enable_stencil = [] (Scene& scene)
    {
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
        glStencilFunc(GL_ALWAYS, 1, 0xff);
    };

    auto disable_stencil = [] (Scene& scene)
    {
        glStencilMask(0x0);
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    };

    MakeAndAddSceneObject<ShaderProgramDrawable>("textureMaterial");
    MakeAndAddSceneObject<SceneCameraDrawable>();
    MakeAndAddSceneObject<SceneLightingDrawable>();
    MakeAndAddSceneObject<TextureMaterialDrawable>("stone", "none", 1.0);
    MakeAndAddSceneObject<PlaneDrawable>(10.f, 10.f, 4);

    MakeAndAddSceneObject<CallbackDrawable<decltype(enable_stencil)>>(enable_stencil);
    auto& mat0 = MakeAndAddSceneObject<TextureMaterialDrawable>("container2", "container2specular", 128.0);
    auto model0 = glm::translate(glm::mat4{1.f}, glm::vec3{2.f, 1.0f, 0.5f});
    MakeAndAddSceneObject<ModelDrawable>(model0);
    auto& cube0 = MakeAndAddSceneObject<CubeDrawable>(1.0f);
    auto& mat1 = MakeAndAddSceneObject<TextureMaterialDrawable>("marble", "none", 1.0);
    auto model1 = glm::translate(glm::mat4{1.f}, glm::vec3{-2.f, 1.0f, -0.5f});
    MakeAndAddSceneObject<ModelDrawable>(model1);
    auto& cube1 = MakeAndAddSceneObject<CubeDrawable>(1.0f);

    MakeAndAddSceneObject<CallbackDrawable<decltype(disable_stencil)>>(disable_stencil);
    MakeAndAddSceneObject<ShaderProgramDrawable>("material_color");
    MakeAndAddSceneObject<SceneCameraDrawable>();
    MakeAndAddSceneObject<SceneLightingDrawable>();
    MakeAndAddSceneObject<MaterialDrawable>(0);
    const float outline_coef = 1.1;
    AddSceneObject(mat0);
    auto model2= glm::scale(glm::translate(glm::mat4{1.0f}, glm::vec3{2.f, 1.0f, 0.5f}), glm::vec3{outline_coef, outline_coef, outline_coef});
    MakeAndAddSceneObject<ModelDrawable>(model2);
    AddSceneObject(cube0);
    AddSceneObject(mat1);
    auto model3 = glm::scale(glm::translate(glm::mat4{1.0f}, glm::vec3{-2.f, 1.0f, -0.5f}), glm::vec3{outline_coef, outline_coef, outline_coef});
    MakeAndAddSceneObject<ModelDrawable>(model3);
    AddSceneObject(cube1);

    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 1.f, 0.09f, 0.032f });
    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 1.f, 0.09f, 0.032f });    

    m_lightInitialPositions.push_back(glm::vec4(0.f, 0.f, 11.f, 1.f));
    m_lightInitialPositions.push_back(glm::vec4(0.f, 0.f, -11.f, 1.f));

    m_lightRotationAxis.push_back(glm::vec3(0.f, 1.f, 0.f));
    m_lightRotationAxis.push_back(glm::vec3(1.f, 0.f, 0.f));
}

Scene::~Scene()
{
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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glStencilMask(0xFF);
    glClearStencil(0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilMask(0x0);

    for (auto& drawable : m_drawables)
    {
        drawable.get().Render();
    }
}

template<typename T, typename ... Args>
T& Scene::MakeAndAddSceneObject(Args&&... args)
{
    auto scene_object = std::make_unique<T>(*this, args...);
    auto& scene_object_ref = *scene_object;
    AddSceneObject(*scene_object);
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
