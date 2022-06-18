#include "Scene.hpp"

#include "TriangleDrawable.hpp"
#include <GL/glew.h> 

Scene::Scene()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        throw exception("glewInit failed");
    }

    MakeAndAddSceneObject<TriangleDrawable>();
}

void Scene::Update()
{
    for (auto& updateable : m_updateables)
    {
        updateable.get().Update();
    }
}

void Scene::Render() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
