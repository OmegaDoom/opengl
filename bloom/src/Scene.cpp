#include "Scene.hpp"

#include "ShaderProgramDrawable.hpp"
#include "SceneLightingDrawable.hpp"
#include "SceneCameraDrawable.hpp"
#include "ViewProjectionDrawable.hpp"
#include "MaterialDrawable.hpp"
#include "TextureMaterialDrawable.hpp"
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
#include "FramebufferDrawable.hpp"
#include "NestedDrawable.hpp"
#include "ScreenDrawable.hpp"
#include "MaterialManager.hpp"
#include <array>
#include <GL/glew.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

Scene::Scene()
    : m_directionalLight{ glm::vec3{0.f, 0.f, -1.f}
                        , glm::vec3{0.f, 0.f, 0.f}
                        , glm::vec3{0.02f, 0.02f, 0.02f}, glm::vec3{0.05f, 0.05f, 0.05f} }
    , m_spotLight{ glm::vec3(0.f)
            , glm::vec3(0.f, 0.f, -1.f)
            , glm::vec3(0.f)
            , glm::vec3(0.4f)
            , glm::vec3(0.4f)
            , glm::cos(glm::radians(12.5f))
            , glm::cos(glm::radians(17.5f))
            , 1.f
            , 0.14f
            , 0.07f }
    , m_colors{{glm::vec3{0.f}, glm::vec3{0.f, 10.f, 0.f}, glm::vec3{0}, 0.f}
               , {glm::vec3{0.f}, glm::vec3{10.f, 0.f, 0.f}, glm::vec3{0}, 0.f}
               , {glm::vec3{0.f}, glm::vec3{0.f, 0.f, 10.f}, glm::vec3{0}, 0.f}}
{
    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.2f), 1.0f, 0.4f, 1.2f });    
    m_lightInitialPositions.push_back(glm::vec4(0.f, 3.f, 3.f, 1.f));
    m_lightRotationAxis.push_back(glm::vec3(0.f, 1.f, 0.f));

    auto lightPos0 = glm::vec4{-1.4f, 0.5f, 0.0, 1.f};
    auto lightPos1 = glm::vec4{1.4f, 0.5f, 0.0, 1.f};
    auto lightPos2 = glm::vec4{-1.4f, 0.5f, 2.f, 1.f};

    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.f, 4.f, 0.0f), glm::vec3(0.8f), glm::vec3(0.2f), 2.5f, 6.2f, 6.2f });    
    m_lightInitialPositions.push_back(lightPos0);
    m_lightRotationAxis.push_back(glm::vec3{lightPos0});

    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(4.0f, 0.f, 0.f), glm::vec3(0.8f), glm::vec3(0.2f), 2.5f, 6.2f, 6.2f });    
    m_lightInitialPositions.push_back(lightPos1);
    m_lightRotationAxis.push_back(glm::vec3{lightPos1});

    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.0f, 0.f, 4.f), glm::vec3(0.8f), glm::vec3(0.2f), 2.5f, 6.2f, 6.2f });    
    m_lightInitialPositions.push_back(lightPos2);
    m_lightRotationAxis.push_back(glm::vec3{lightPos2});

    auto& textureManager = TextureManager::instance();
    auto& materialManager = MaterialManager::instance();

    auto& container2diff = textureManager["container2"];
    auto& container2spec = textureManager["container2specular"];
    auto& grassTexture = textureManager["grass"];
    auto& stoneTexture = textureManager["stone"];
    auto& noneTexture = textureManager["none"];
    auto& skyboxTexture = textureManager["skybox"];

    auto& pebblesTexture = textureManager["pebbles"];
    auto& pebblesNormalTexture = textureManager["pebblesNormalMap"];
    auto& pebblesDisp = textureManager["pebblesDisp"];

    const auto& framebufferTexture0 = textureManager["framebuffer0"];
    const auto& depthAndStencilTexture = textureManager["depth_and_stencil"];
    auto& framebufferDrawable0 = MakeAndAddSceneObject<FramebufferDrawable>(std::array<std::reference_wrapper<const Texture>, 1>{framebufferTexture0}, depthAndStencilTexture);
    m_pipeline.Add(framebufferDrawable0, RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<SceneCameraDrawable>(), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<SceneLightingDrawable>(), RenderPipeline::PASS0);

    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("material"), RenderPipeline::PASS0);

    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{1.0}, glm::vec3{-0.5, 0.0, 0.0}))
        , RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<MaterialDrawable>(materialManager[MATERIAL_ID::EMERALD]), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<SphereDrawable>(0.5f), RenderPipeline::PASS0);

    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{1.0}, glm::vec3{0.5, 0.0, 0.0}))
        , RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<MaterialDrawable>(materialManager[MATERIAL_ID::RUBY]), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<SphereDrawable>(0.5f), RenderPipeline::PASS0);

    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("diffuseColor"), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{1.0}, glm::vec3{lightPos0}))
        , RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<MaterialDrawable>(m_colors[0]), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<CubeDrawable>(0.1f), RenderPipeline::PASS0);

    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{1.0}, glm::vec3{lightPos1}))
        , RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<MaterialDrawable>(m_colors[1]), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<CubeDrawable>(0.1f), RenderPipeline::PASS0);

    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{1.0}, glm::vec3{lightPos2}))
        , RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<MaterialDrawable>(m_colors[2]), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<CubeDrawable>(0.1f), RenderPipeline::PASS0);

    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("textureMaterial"), RenderPipeline::PASS0);

    auto& containerTextureMaterialDrawable = MakeAndAddSceneObject<TextureMaterialDrawable>(container2diff, container2spec, 1.f);

    auto& container = MakeAndAddSceneObject<CubeDrawable>(0.5f);

    auto& contTransform0 = MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{ 1.f } , glm::vec3{ 2.f, 0.f, 0.f }));
    auto& contTransform1 = MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{ 1.f } , glm::vec3{ -2.f, 0.f, 0.f }));
    auto& contTransform2 = MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{ 1.f } , glm::vec3{ -2.f, 0.f, 2.f }));

    m_pipeline.Add(containerTextureMaterialDrawable, RenderPipeline::PASS0);
    m_pipeline.Add(contTransform0, RenderPipeline::PASS0);
    m_pipeline.Add(container, RenderPipeline::PASS0);
    m_pipeline.Add(contTransform1, RenderPipeline::PASS0);
    m_pipeline.Add(container, RenderPipeline::PASS0);
    m_pipeline.Add(contTransform2, RenderPipeline::PASS0);
    m_pipeline.Add(container, RenderPipeline::PASS0);

    auto& drawable = MakeAndAddSceneObject<QuadDrawable>(28.f, 28.f, 8);
    auto& tangentDrawable = MakeAndAddSceneObject<TangentDrawable>(drawable.Tangent());
    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("parallaxMapTextureMaterial"), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<ParallaxMappingTextureMaterialDrawable>(pebblesTexture, noneTexture, pebblesNormalTexture, pebblesDisp, 0.01f), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::rotate(glm::translate(glm::mat4{1.f}, glm::vec3{ 0.f, -0.5f, 0.f}), glm::radians(-90.f), glm::vec3{1.f, 0.f, 0.f})), RenderPipeline::PASS0);
    m_pipeline.Add(tangentDrawable, RenderPipeline::PASS0);
    m_pipeline.Add(drawable, RenderPipeline::PASS0);

    const auto& framebufferTexture1 = textureManager["brightFramebuffer"];
    auto& framebufferDrawable1 = MakeAndAddSceneObject<FramebufferDrawable>(std::array<std::reference_wrapper<const Texture>, 1>{framebufferTexture1});
    m_pipeline.Add(framebufferDrawable1, RenderPipeline::PASS1);
    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("brightColor"), RenderPipeline::PASS1);
    m_pipeline.Add(MakeAndAddSceneObject<TextureMaterialDrawable>(framebufferTexture0, noneTexture, 1.f), RenderPipeline::PASS1);
    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::mat4{1.f}), RenderPipeline::PASS1);
    m_pipeline.Add(MakeAndAddSceneObject<QuadDrawable>(2.f, 2.f, 1), RenderPipeline::PASS1);

    const auto& framebufferTexture2 = textureManager["framebuffer1"];
    auto& framebufferDrawable2 = MakeAndAddSceneObject<FramebufferDrawable>(std::array<std::reference_wrapper<const Texture>, 1>{framebufferTexture2});
    const int vertGaussPass = RenderPipeline::PASS2 | RenderPipeline::PASS4 | RenderPipeline::PASS6 | RenderPipeline::PASS8 | RenderPipeline::PASS10 | RenderPipeline::PASS12 | RenderPipeline::PASS14;
    m_pipeline.Add(framebufferDrawable2, vertGaussPass);
    auto& verticalGaussDrawable = MakeAndAddSceneObject<ShaderProgramDrawable>("verticalGauss");
    m_pipeline.Add(verticalGaussDrawable, vertGaussPass);
    m_pipeline.Add(MakeAndAddSceneObject<TextureMaterialDrawable>(framebufferTexture1, noneTexture, 1.f), vertGaussPass);
    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::mat4{1.f}), vertGaussPass);
    m_pipeline.Add(MakeAndAddSceneObject<QuadDrawable>(2.f, 2.f, 1), vertGaussPass);

    const int horGaussPass = RenderPipeline::PASS3 | RenderPipeline::PASS5 | RenderPipeline::PASS7 | RenderPipeline::PASS9 | RenderPipeline::PASS11 | RenderPipeline::PASS13 | RenderPipeline::PASS15;
    m_pipeline.Add(framebufferDrawable1, horGaussPass);
    auto& horizontalGaussDrawable = MakeAndAddSceneObject<ShaderProgramDrawable>("horizontalGauss");
    m_pipeline.Add(horizontalGaussDrawable, horGaussPass);
    m_pipeline.Add(MakeAndAddSceneObject<TextureMaterialDrawable>(framebufferTexture2, noneTexture, 1.f), horGaussPass);
    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::mat4{1.f}), horGaussPass);
    m_pipeline.Add(MakeAndAddSceneObject<QuadDrawable>(2.f, 2.f, 1), horGaussPass);

    const int screenPass = RenderPipeline::PASS16;
    m_pipeline.Add(MakeAndAddSceneObject<ScreenDrawable>(), screenPass);
    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("hdr"), screenPass);
    m_pipeline.Add(MakeAndAddSceneObject<TextureMaterialDrawable>(framebufferTexture0, framebufferTexture1, 1.f), screenPass);
    m_pipeline.Add(MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::mat4{1.f}), screenPass);
    m_pipeline.Add(MakeAndAddSceneObject<QuadDrawable>(2.f, 2.f, 1), screenPass);
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
    //glDisable(GL_FRAMEBUFFER_SRGB);
    //glEnable(GL_FRAMEBUFFER_SRGB);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_pipeline.Render();
}

template<typename T, typename ... Args>
T& Scene::MakeAndAddSceneObject(Args&&... args)
{
    auto scene_object = std::make_unique<T>(*this, std::move(args)...);
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
    auto& textureManager = TextureManager::instance();
    auto& framebufferTexture0 = static_cast<Texture2D&>(textureManager["framebuffer0"]);
    framebufferTexture0.Resize(width, height);
    auto& framebufferTexture1 = static_cast<Texture2D&>(textureManager["brightFramebuffer"]);
    framebufferTexture1.Resize(width, height);
    auto& framebufferTexture2 = static_cast<Texture2D&>(textureManager["framebuffer1"]);
    framebufferTexture2.Resize(width, height);
    auto& depthTexture = static_cast<Texture2D&>(textureManager["depth_and_stencil"]);
    depthTexture.Resize(width, height);
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
