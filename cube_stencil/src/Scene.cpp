#include "Scene.hpp"

#include "ShaderProgramDrawable.hpp"
#include "SceneLightingDrawable.hpp"
#include "SceneCameraDrawable.hpp"
#include "MaterialDrawable.hpp"
#include "CubeDrawable.hpp"
#include "SphereDrawable.hpp"
#include "PlaneDrawable.hpp"
#include "CallbackDrawable.hpp"
#include <algorithm>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
    std::vector<std::unique_ptr<Drawable>> getDrawables(Scene& scene)
    {
        std::vector<std::unique_ptr<Drawable>> drawables;

        auto transform = glm::translate(glm::mat4{ 1.f }, glm::vec3{ 0.f, 1.01f, 0.f });
        drawables.push_back(std::make_unique<ShaderProgramDrawable>(scene, "material"));
        drawables.push_back(std::make_unique<SceneCameraDrawable>(scene));
        drawables.push_back(std::make_unique<MaterialDrawable>(scene, 0));
        drawables.push_back(std::make_unique<SceneLightingDrawable>(scene));

        using CallbackDrawableFuncT = void (*) (Scene& scene);
        auto write_to_stencil0 = [] (Scene& scene)
        {  
            glClear(GL_STENCIL_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);
            glColorMask(0, 0, 0, 0);
            glStencilMask(0xff);
            glStencilFunc(GL_ALWAYS, 1, 0xff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        };

        drawables.push_back(std::make_unique<CallbackDrawable<CallbackDrawableFuncT>>(scene, write_to_stencil0));
        drawables.push_back(std::make_unique<SphereDrawable>(scene, 1.1f, transform));

        drawables.push_back(std::make_unique<ShaderProgramDrawable>(scene, "textureMaterial"));
        drawables.push_back(std::make_unique<SceneCameraDrawable>(scene));
        drawables.push_back(std::make_unique<SceneLightingDrawable>(scene));

        auto write_to_stencil1 = [] (Scene& scene)
        {  
            glStencilFunc(GL_EQUAL, 1, 0xff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
        };

        drawables.push_back(std::make_unique<CallbackDrawable<CallbackDrawableFuncT>>(scene, write_to_stencil1));
        drawables.push_back(std::make_unique<CubeDrawable>(scene, 1.0f, transform));

        auto use_stencil = [] (Scene& scene)
        {  
            glClear(GL_DEPTH_BUFFER_BIT);
            glColorMask(0xff, 0xff, 0xff, 0xff);
            glStencilFunc(GL_EQUAL, 0, 0xff);
        };

        drawables.push_back(std::make_unique<CallbackDrawable<CallbackDrawableFuncT>>(scene, use_stencil));
        drawables.push_back(std::make_unique<CubeDrawable>(scene, 1.0f, transform));
        return drawables;
    }
}

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
    
    using CallbackDrawableFuncT = void (*) (Scene& scene);

    auto depth_greater = [] (Scene& scene)
    {  
        glDepthFunc(GL_GREATER);
        glClearDepth(0.f);
        glClear(GL_DEPTH_BUFFER_BIT);
    };

    auto depth_less = [] (Scene& scene)
    {  
        glDepthFunc(GL_LESS);
        glClearDepth(1.f);
        glClear(GL_DEPTH_BUFFER_BIT);
    };

    MakeAndAddSceneObject<ShaderProgramDrawable>(*this, "textureMaterial");
    MakeAndAddSceneObject<SceneCameraDrawable>(*this);
    MakeAndAddSceneObject<SceneLightingDrawable>(*this);
    MakeAndAddSceneObject<PlaneDrawable>(*this, 10.f, 10.f, 4);

    MakeAndAddSceneObject<CallbackDrawable<CallbackDrawableFuncT>>(*this, depth_greater);
    auto drawables0 = getDrawables(*this);
    std::for_each(drawables0.begin(), drawables0.end(), [this] (auto&& e) { this->AddSceneObject(std::move(e)); });
    MakeAndAddSceneObject<CallbackDrawable<CallbackDrawableFuncT>>(*this, depth_less);

    auto drawables1 = getDrawables(*this);
    std::for_each(drawables1.begin(), drawables1.end(), [this] (auto&& e) { this->AddSceneObject(std::move(e)); });

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
    glClearStencil(0);
    glStencilMask(0xff);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for (auto& drawable : m_drawables)
    {
        drawable.get().Render();
    }
}

template<typename T, typename ... Args>
void Scene::MakeAndAddSceneObject(Args&&... args)
{
    AddSceneObject(std::make_unique<T>(args...));
}

template<typename T>
void Scene::AddSceneObject(std::unique_ptr<T> object)
{
    if constexpr(std::is_base_of_v<Drawable, T>)
    {
        m_drawables.push_back(*object);
    }

    if constexpr(std::is_base_of_v<Updateable, T>)
    {
        m_updateables.push_back(*object);
    }

    m_scene_objects.push_back(std::move(object));
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
