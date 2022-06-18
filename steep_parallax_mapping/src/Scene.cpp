#include "Scene.hpp"

#include "ShaderProgramDrawable.hpp"
#include "SceneLightingDrawable.hpp"
#include "SceneCameraDrawable.hpp"
#include "ViewProjectionDrawable.hpp"
#include "MaterialDrawable.hpp"
#include "CubeDrawable.hpp"
#include "SphereDrawable.hpp"
#include "QuadDrawable.hpp"
#include "PlaneDrawable.hpp"
#include "QuadDrawable.hpp"
#include "TangentDrawable.hpp"
#include "CallbackDrawable.hpp"
#include "CompoundDrawable.hpp"
#include "ShadowMapDrawable.hpp"
#include "CubemapShadowMapDrawable.hpp"
#include "CubemapTexture.hpp"
#include "ParallaxMappingTextureMaterialDrawable.hpp"
#include "ShadowMapTextureMaterialDrawable.hpp"
#include "ModelNormalTransformDrawable.hpp"
#include "DiffuseTextureMaterialDrawable.hpp"
#include "BillboardModelNormalTransformDrawable.hpp"
#include "TextureMaterial.hpp"
#include "TextureManager.hpp"
#include "Texture.hpp"
#include "LightSpaceShaderProgram.hpp"
#include "PointLightShadowMapTransformationsDrawable.hpp"
#include <GL/glew.h> 
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene()
    : m_directionalLight{ glm::vec3{0.f, 0.f, -1.f}
                        , glm::vec3{0.f, 0.f, 0.f}
                        , glm::vec3{0.02f, 0.02f, 0.02f}, glm::vec3{0.05f, 0.05f, 0.05f} }
    , m_spotLight{ glm::vec3(0.f)
            , glm::vec3(0.f, 0.f, -1.f)
            , glm::vec3(0.f)
            , glm::vec3(0.f)
            , glm::vec3(0.f)
            , glm::cos(glm::radians(12.5f))
            , glm::cos(glm::radians(17.5f))
            , 1.f
            , 0.14f
            , 0.07f }
    , m_lightView{ glm::lookAt(glm::vec3{ 0.f, 50.f, 0.f }, glm::vec3{ 0.f }, glm::vec3{ 0.f, 0.f, -1.f }) }
{
    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.2f), 2.0f, 0.8f, 1.8f });    
    //m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.5f), 1.f, 0.14f, 0.07f });    

    m_lightInitialPositions.push_back(glm::vec4(0.f, 1.f, 1.f, 1.f));
    //m_lightInitialPositions.push_back(glm::vec4(0.f, 0.f, -11.f, 1.f));

    m_lightRotationAxis.push_back(glm::vec3(0.f, 1.f, 0.f));
    //m_lightRotationAxis.push_back(glm::vec3(1.f, 0.f, 0.f));

    auto& textureManager = TextureManager::instance();

    auto& noneTexture = textureManager["none"];
    auto& bricksTexture = textureManager["bricks"];
    auto& bricksNormalTexture = textureManager["bricksNormalMap"];
    auto& bricksDisp = textureManager["bricksDisp"];

    m_pipeline.Add(MakeAndAddSceneObject<SceneCameraDrawable>(), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<SceneLightingDrawable>(), RenderPipeline::PASS0);

    auto& drawable = MakeAndAddSceneObject<QuadDrawable>(2.8f, 2.8f);
    auto& tangentDrawable = MakeAndAddSceneObject<TangentDrawable>(drawable.Tangent());
    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("parallaxMapTextureMaterial"), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<ParallaxMappingTextureMaterialDrawable>(bricksTexture, noneTexture, bricksNormalTexture, bricksDisp, 0.01f), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::rotate(glm::mat4(1.f), glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f))), RenderPipeline::PASS0);
    m_pipeline.Add(tangentDrawable, RenderPipeline::PASS0);
    m_pipeline.Add(drawable, RenderPipeline::PASS0);
}

void Scene::Update()
{
    m_camera.Update();

    const auto epoch = std::chrono::time_point<std::chrono::steady_clock>{};
    const auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - epoch);
    auto scalar = duration.count() / 1000.f;

    auto lightPos = glm::rotate(glm::mat4(1.f), glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f)) * glm::vec4{ 0.f, 50.f, 0.f, 1.f };
    const auto viewLightDir = m_camera.View() * (glm::vec4(0.f, 0.f, 0.f, 0.f) - lightPos);
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pipeline.Render();
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
    if constexpr(std::is_base_of_v<Updateable, T>)
    {
        m_updateables.push_back(object);
    }
}

void Scene::ChangeDimension(int width, int height)
{
    m_projection = glm::perspective(glm::radians(45.f), static_cast<float>(width) / height, 0.1f, 1000.f);
    m_width = width;
    m_height = height;
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
