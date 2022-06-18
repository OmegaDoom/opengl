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
#include "CallbackDrawable.hpp"
#include "CompoundDrawable.hpp"
#include "ShadowMapDrawable.hpp"
#include "CubemapShadowMapDrawable.hpp"
#include "CubemapTexture.hpp"
#include "TextureMaterialDrawable.hpp"
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
                        , glm::vec3{0.01f, 0.01f, 0.01f}, glm::vec3{0.05f, 0.05f, 0.05f} }
    , m_spotLight{ glm::vec3(0.f)
            , glm::vec3(0.f, 0.f, -1.f)
            , glm::vec3(0.2f)
            , glm::vec3(1.f)
            , glm::vec3(1.f)
            , glm::cos(glm::radians(12.5f))
            , glm::cos(glm::radians(17.5f))
            , 1.f
            , 0.14f
            , 0.07f }
    , m_lightView{ glm::lookAt(glm::vec3{ 0.f, 50.f, 0.f }, glm::vec3{ 0.f }, glm::vec3{ 0.f, 0.f, -1.f }) }
{
    m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.5f), 1.f, 0.14f, 0.07f });    
    //m_pointLights.push_back({ glm::vec3(0.f), glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.5f), 1.f, 0.14f, 0.07f });    

    m_lightInitialPositions.push_back(glm::vec4(0.f, 6.f, 11.f, 1.f));
    //m_lightInitialPositions.push_back(glm::vec4(0.f, 0.f, -11.f, 1.f));

    m_lightRotationAxis.push_back(glm::vec3(0.f, 1.f, 0.f));
    //m_lightRotationAxis.push_back(glm::vec3(1.f, 0.f, 0.f));

    auto& textureManager = TextureManager::instance();
    auto& container2diff = textureManager["container2"];
    auto& container2spec = textureManager["container2specular"];
    auto& grassTexture = textureManager["grass"];
    auto& stoneTexture = textureManager["stone"];
    auto& noneTexture = textureManager["none"];
    auto& skyboxTexture = textureManager["skybox"];
    auto& cubemapShadowMapTexture = static_cast<CubemapTexture&>(textureManager["cubemapDepthMap"]);

    m_pipeline.Add(MakeAndAddSceneObject<SceneLightingDrawable>(), RenderPipeline::PASS0 | RenderPipeline::PASS1);

    using CallbackDrawableFuncT = void (*) (Scene& scene);

    auto shadow_map_pass = [] (Scene& scene)
    {  
        glViewport(0, 0, 1024, 1024);
    };
    m_pipeline.Add(MakeAndAddSceneObject<CallbackDrawable<CallbackDrawableFuncT>>(shadow_map_pass), RenderPipeline::PASS0);

    auto render_pass = [] (Scene& scene)
    {  
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, scene.m_width, scene.m_height);
    };
    m_pipeline.Add(MakeAndAddSceneObject<CallbackDrawable<CallbackDrawableFuncT>>(render_pass), RenderPipeline::PASS1);
    m_pipeline.Add(MakeAndAddSceneObject<SceneCameraDrawable>(), RenderPipeline::PASS1);

    m_pipeline.Add(MakeAndAddSceneObject<CubemapShadowMapDrawable>(cubemapShadowMapTexture), RenderPipeline::PASS0);

    auto& shadowMapTextureMaterialShaderProgram = MakeAndAddSceneObject<ShaderProgramDrawable>("shadowMapTextureMaterial");

    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("simpleCubemapDepth"), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<PointLightShadowMapTransformationsDrawable>(m_shadowPointLightDir), RenderPipeline::PASS0);
    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("textureMaterial"), RenderPipeline::PASS1);

    auto& containerTextureMaterialDrawable = MakeAndAddSceneObject<TextureMaterialDrawable>(container2diff, container2spec, 1.f);

    auto& container = MakeAndAddSceneObject<CubeDrawable>(0.5f);

    auto& contTransform0 = MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{ 1.f } , glm::vec3{ 2.f, 2.5f, 0.f }));
    auto& contTransform1 = MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{ 1.f } , glm::vec3{ -2.f, 2.5f, 0.f }));
    auto& contTransform2 = MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{ 1.f } , glm::vec3{ -2.f, 2.5f, 2.f }));

    m_pipeline.Add(containerTextureMaterialDrawable, RenderPipeline::PASS1);
    m_pipeline.Add(contTransform0, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(container, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(contTransform1, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(container, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(contTransform2, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(container, RenderPipeline::PASS0 | RenderPipeline::PASS1);

    auto& materialShaderDrawable = MakeAndAddSceneObject<ShaderProgramDrawable>("material");
    auto& materialDrawable0 = MakeAndAddSceneObject<MaterialDrawable>(0);
    auto& materialDrawable1 = MakeAndAddSceneObject<MaterialDrawable>(4);
    auto& sphere = MakeAndAddSceneObject<SphereDrawable>(0.5f);
    auto& sphereTransf0 = MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{ 1.f } , glm::vec3{ 0.f, 1.5f, 0.f }));
    auto& sphereTransf1 = MakeAndAddSceneObject<ModelNormalTransformDrawable>(glm::translate(glm::mat4{ 1.f } , glm::vec3{ 4.f, 1.5f, 0.f }));

    m_pipeline.Add(materialShaderDrawable, RenderPipeline::PASS1);
    m_pipeline.Add(materialDrawable0, RenderPipeline::PASS1);
    m_pipeline.Add(sphereTransf0, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(sphere, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(materialDrawable1, RenderPipeline::PASS1);
    m_pipeline.Add(sphereTransf1, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(sphere, RenderPipeline::PASS0 | RenderPipeline::PASS1);

    auto& discardAlphaShaderProgram = MakeAndAddSceneObject<ShaderProgramDrawable>("discardAlpha");
    
    auto& grassTextureMaterialDrawable = MakeAndAddSceneObject<TextureMaterialDrawable>(grassTexture, noneTexture, 1.f);

    auto& billboardTransformDrawable0 = MakeAndAddSceneObject<BillboardModelNormalTransformDrawable>(
                            glm::rotate(glm::rotate(glm::translate( glm::mat4{ 1.f }, glm::vec3{ 0.f, 0.5f, -1.f})
                            , glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f })
                            , glm::radians(180.f), glm::vec3{ 0.f, 0.f, 1.f }));

    auto& billboardTransformDrawable1 = MakeAndAddSceneObject<BillboardModelNormalTransformDrawable>(
                            glm::rotate(glm::rotate(glm::translate( glm::mat4{ 1.f }, glm::vec3{ 3.f, 0.5f, 0.f})
                            , glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f })
                            , glm::radians(180.f), glm::vec3{ 0.f, 0.f, 1.f }));

    auto& grassDrawable = MakeAndAddSceneObject<QuadDrawable>(1.f, 1.f);

    m_pipeline.Add(discardAlphaShaderProgram, RenderPipeline::PASS1);
    m_pipeline.Add(grassTextureMaterialDrawable, RenderPipeline::PASS1);
    m_pipeline.Add(billboardTransformDrawable0, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(grassDrawable, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(billboardTransformDrawable1, RenderPipeline::PASS0 | RenderPipeline::PASS1);
    m_pipeline.Add(grassDrawable, RenderPipeline::PASS0 | RenderPipeline::PASS1);

    m_pipeline.Add(shadowMapTextureMaterialShaderProgram, RenderPipeline::PASS1);

    auto& groundShadowMapTextureMaterialDrawable = MakeAndAddSceneObject<ShadowMapTextureMaterialDrawable>(stoneTexture, noneTexture, 1.f, cubemapShadowMapTexture);
    m_pipeline.Add(groundShadowMapTextureMaterialDrawable, RenderPipeline::PASS1);
    m_pipeline.Add(MakeAndAddSceneObject<PointLightShadowMapTransformationsDrawable>(m_shadowPointLightDir), RenderPipeline::PASS1);
    auto& ground = MakeAndAddSceneObject<PlaneDrawable>(160.f, 160.f, 64);
    m_pipeline.Add(ground, RenderPipeline::PASS1);

    m_pipeline.Add(MakeAndAddSceneObject<ShaderProgramDrawable>("skyBox"), RenderPipeline::PASS1);
    m_pipeline.Add(MakeAndAddSceneObject<DiffuseTextureMaterialDrawable>(skyboxTexture), RenderPipeline::PASS1);

    auto prepare_for_skybox = [] (Scene& scene)
    {  
        glCullFace(GL_FRONT);
    };

    m_pipeline.Add(MakeAndAddSceneObject<CallbackDrawable<CallbackDrawableFuncT>>(prepare_for_skybox), RenderPipeline::PASS1);
    m_pipeline.Add(MakeAndAddSceneObject<CubeDrawable>(1.f), RenderPipeline::PASS1);

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

    const auto shadowLightTransform = glm::rotate(glm::mat4(1.f), scalar, m_lightRotationAxis[0]); 
    const auto shadowLightPos = shadowLightTransform * m_lightInitialPositions[0]; 
    m_shadowPointLightDir.x = shadowLightPos.x;
    m_shadowPointLightDir.y = shadowLightPos.y;
    m_shadowPointLightDir.z = shadowLightPos.z;

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
    glClearDepth(1000.f);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);
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
