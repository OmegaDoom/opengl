#include "Scene.hpp"

#include <GL/glew.h> 

Scene::Scene()
{
}

void Scene::Update()
{
}

void Scene::Draw() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
