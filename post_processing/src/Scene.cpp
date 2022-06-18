#include "Scene.hpp"

#include "ShaderProgramDrawable.hpp"
#include "SceneLightingDrawable.hpp"
#include "SceneCameraDrawable.hpp"
#include "MaterialDrawable.hpp"
#include "TextureMaterialDrawable.hpp"
#include "CubeDrawable.hpp"
#include "QuadDrawable.hpp"
#include "SphereDrawable.hpp"
#include "PlaneDrawable.hpp"
#include "BillboardQuadDrawable.hpp"
#include "CallbackDrawable.hpp"
#include "FramebufferDrawable.hpp"
#include "TextureMaterial.hpp"
#include "DiffuseTextureMaterialDrawable.hpp"
#include "ConvolutionDrawable.hpp"
#include "TextureManager.hpp"
#include <GL/glew.h> 
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene()
    : m_directionalLight{ glm::vec3{0.f, 0.f, 1.f}
                        , glm::vec3{0.1f, 0.1f, 0.1f}
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
    auto& textureManager = TextureManager::instance();
    auto& container2diff = textureManager["container2"];
    auto& container2spec = textureManager["container2specular"];
    auto& grassTexture = textureManager["grass"];
    auto& stoneTexture = textureManager["stone"];
    auto& noneTexture = textureManager["none"];
    auto& framebufferTexture = textureManager["framebuffer"];

    MakeAndAddSceneObject<FramebufferDrawable>(1, 1);

    MakeAndAddSceneObject<SceneCameraDrawable>();
    MakeAndAddSceneObject<SceneLightingDrawable>();

    MakeAndAddSceneObject<ShaderProgramDrawable>("material");
    MakeAndAddSceneObject<MaterialDrawable>(0);
    MakeAndAddSceneObject<SphereDrawable>(0.5f, glm::translate(glm::mat4{1.f }, glm::vec3{0.f, 0.5f, 0.f }));
    MakeAndAddSceneObject<MaterialDrawable>(4);
    MakeAndAddSceneObject<SphereDrawable>(0.5f, glm::translate(glm::mat4{1.f }, glm::vec3{2.f, 1.5f, 0.f }));
    
    MakeAndAddSceneObject<ShaderProgramDrawable>("discardAlpha");
    MakeAndAddSceneObject<TextureMaterialDrawable>(grassTexture, noneTexture, 1.f);

    MakeAndAddSceneObject<BillboardQuadDrawable>(1.f, 1.f
                            , glm::rotate(glm::translate( glm::mat4{ 1.f }, glm::vec3{ 0.f, 0.5f, -1.f})
                            , glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f }));

    MakeAndAddSceneObject<BillboardQuadDrawable>(1.f, 1.f
                            , glm::rotate(glm::translate( glm::mat4{ 1.f }, glm::vec3{ -3.f, 0.5f, 0.f})
                            , glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f }));

    MakeAndAddSceneObject<BillboardQuadDrawable>(1.f, 1.f
                            , glm::rotate(glm::translate( glm::mat4{ 1.f }, glm::vec3{ 3.f, 0.5f, 0.f})
                            , glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f }));

    MakeAndAddSceneObject<BillboardQuadDrawable>(1.f, 1.f
                            , glm::rotate(glm::translate( glm::mat4{ 1.f }, glm::vec3{ 0.f, 0.5f, 1.f})
                            , glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f }));

    MakeAndAddSceneObject<ShaderProgramDrawable>("textureMaterial");
    MakeAndAddSceneObject<TextureMaterialDrawable>(stoneTexture, noneTexture, 1.f);
    MakeAndAddSceneObject<PlaneDrawable>(160.f, 160.f, 64);

    MakeAndAddSceneObject<TextureMaterialDrawable>(container2diff, container2spec, 1.f);
    MakeAndAddSceneObject<CubeDrawable>(0.5f, glm::translate(glm::mat4{ 1.f }, glm::vec3{ 2.f, 0.5f, 0.f }));
    MakeAndAddSceneObject<CubeDrawable>(0.5f, glm::translate(glm::mat4{ 1.f }, glm::vec3{ -2.f, 0.5f, 0.f }));
    MakeAndAddSceneObject<CubeDrawable>(0.5f, glm::translate(glm::mat4{ 1.f }, glm::vec3{ -2.f, 1.5f, 0.f }));

    MakeAndAddPostProcessSceneObject<ShaderProgramDrawable>("blurConvolution");
    MakeAndAddPostProcessSceneObject<ConvolutionDrawable>(1, 1, framebufferTexture);
    MakeAndAddPostProcessSceneObject<QuadDrawable>(2.f, 2.f);

    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 1.f, 0.09f, 0.032f });    
    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 1.f, 0.09f, 0.032f });    

    m_lightInitialPositions.push_back(glm::vec4(0.f, 1.f, 11.f, 1.f));
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for (auto& drawable : m_drawables)
    {
       drawable.get().Render();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    for (auto& drawable : m_post_process_drawables)
    {
       drawable.get().Render();
    }
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

template<typename T, typename ... Args>
T& Scene::MakeAndAddPostProcessSceneObject(Args&&... args)
{
    auto scene_object = std::make_unique<T>(*this, args...);
    auto& scene_object_ref = *scene_object;
    AddPostProcessSceneObject(scene_object_ref);
    m_scene_objects.push_back(std::move(scene_object));
    return scene_object_ref;
}

template<typename T>
void Scene::AddPostProcessSceneObject(T& object)
{
    if constexpr(std::is_base_of_v<Drawable, T>)
    {
        m_post_process_drawables.push_back(object);
    }

    if constexpr(std::is_base_of_v<Updateable, T>)
    {
        m_updateables.push_back(object);
    }
}

void Scene::ChangeDimension(int width, int height)
{
    m_projection = glm::perspective(glm::radians(45.f), static_cast<float>(width) / height, 0.1f, 1000.f);
    auto framebuffer = std::make_unique<FramebufferDrawable>(*this, width, height);
    auto& framebuffer_ref = *framebuffer;
    m_drawables[0] = framebuffer_ref;
    m_scene_objects[0] = std::move(framebuffer);
    auto convolution_drawable = std::make_unique<ConvolutionDrawable>(*this, width, height, TextureManager::instance()["framebuffer"]);
    auto& convolution_drawable_ref = *convolution_drawable;
    m_post_process_drawables[1] = convolution_drawable_ref;
    m_scene_objects[22] = std::move(convolution_drawable);
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