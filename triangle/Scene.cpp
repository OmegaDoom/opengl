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

    AddDrawable(std::make_unique<TriangleDrawable>());
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
        drawable->Render();
    }
}

void Scene::AddDrawable(std::unique_ptr<Drawable>&& drawable)
{
    m_drawables.push_back(std::move(drawable));
}

void Scene::AddDrawable(std::unique_ptr<UpdateableDrawable>&& drawable)
{
    m_drawables.push_back(std::move(drawable));
    m_updateables.push_back(*drawable);
}
