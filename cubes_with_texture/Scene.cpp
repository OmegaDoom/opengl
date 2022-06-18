#include "Scene.hpp"

#include "CubeDrawable.hpp"
#include <GL/glew.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <random>

Scene::Scene()
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-10.f ,10.f);
    for (int i = 0; i < 40; ++i)
    {
        float x = distribution(generator);
        float y = distribution(generator);
        float z = distribution(generator);
        glm::vec3 v(x, y, z);
        MakeAndAddSceneObject<CubeDrawable>(*this, v);
    }
}

void Scene::Update()
{
    m_camera.Update();
    for (auto& updateable : m_updateables)
    {
        updateable.get().Update();
    }
}

void Scene::Render() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
void Scene::MakeAndAddSceneObject(Args&&... args)
{
    auto object = std::make_unique<T>(args...);

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
